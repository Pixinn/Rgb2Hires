#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <chrono>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Picture.h"
#include "Display.h"


using namespace std;

namespace RgbToHires
{
  namespace Display
  {

		//! @brief	 Output the colors from a 14-dot block
		void UpdateHiResRGBCell(const int x, const uint8_t* pLineAddr, rgba8Bits_t* pOut);
		std::unique_ptr<Screen> ComputeRgbBuffer(const uint8_t* hires);


		Window* Window::S_pInstance = nullptr;

		Window::~Window()
		{
			if (_pThread != nullptr) {
				if (_pThread->joinable()) {
					_stopFileSurvey.store(true);
					_pThread->join();
				}
				delete _pThread;
			}
			if (_pTexture != nullptr) {
				SDL_DestroyTexture(_pTexture);
			}
			if (_pRenderer != nullptr) {
				SDL_DestroyRenderer(_pRenderer);
			}
			if (_pWindow != nullptr) {
				SDL_DestroyWindow(_pWindow);
			}
			SDL_Quit();
		}

		Window* Window::GetInstance()
		{
			if (S_pInstance == nullptr) {
				S_pInstance = new Window;
				S_pInstance->init();
			}
			return S_pInstance;
		}


		bool Window::init()
		{
			// init
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				sdlError("cannot initialise the preview window.");
			}
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // nearest neighbor

			// window
			_pWindow = SDL_CreateWindow("Preview", SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				560 * SCALE,
				384 * SCALE,
				SDL_WINDOW_SHOWN
			);
			if (_pWindow == nullptr) {
				sdlError("cannot initialise the preview window.");
			}
			// renderer
			_pRenderer = SDL_CreateRenderer(_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (_pRenderer == nullptr) {
				sdlError("cannot initialise the preview window.");
			}
			// texture
			_pTexture = SDL_CreateTexture(_pRenderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_STATIC, 560, 384);
			if (_pTexture == nullptr) {
				sdlError("cannot initialise the preview window.");
			}

			return true;
		}

		void Window::sdlError(const std::string& msg)
		{
			throw std::runtime_error{ "Error: " + msg };
		}

		void Window::display(const std::string& path, const uint8_t* hiresblob)
		{		

			struct {
				bool happened = false;
				std::string what;
			} error;

			auto pViewport = ComputeRgbBuffer(hiresblob);
			SDL_UpdateTexture(_pTexture, nullptr, pViewport->data(), sizeof(rgba8Bits_t) * 560);
			SDL_RenderClear(_pRenderer);
			SDL_RenderCopy(_pRenderer, _pTexture, NULL, NULL);
			SDL_RenderPresent(_pRenderer);

			// launch thread to survey file modifications
			_pThread = new std::thread( [this, &path, &pViewport, &error]  // freed in ~Window()
			{	
				auto timeModified = std::filesystem::last_write_time(path);
				while (!this->_stopFileSurvey.load())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					const bool isImgModified(timeModified != std::filesystem::last_write_time(path));					
					if (isImgModified)
					{
						bool isDone = false;
						int nbErrors = 0;
						while (!isDone) // several attempts as the file may be marked modified before being written
						{
							try
							{
								this_thread::sleep_for(std::chrono::milliseconds(500)); // 500ms between each attempt
								// update hires image
								const auto imageRgb = Magick::Image{ path };
								auto imageQuantized = ImageQuantized{ imageRgb };
								const auto imageHiRes = Picture{ imageQuantized };

								// rgb conversion from hires data
								std::lock_guard<std::mutex> lock{ this->_mutex }; // protecting pViewport
								pViewport = ComputeRgbBuffer(imageHiRes.getBlob()->data());								
								isDone = true;
								timeModified = std::filesystem::last_write_time(path);
								this->_isFileModified.store(isImgModified);
							}
							catch (Magick::Error& e)
							{
								++nbErrors;
								if (nbErrors >= 5) { // 5 atttemps
									isDone = true;
									error.happened = true;
									error.what = e.what();
									this->_stopFileSurvey.store(true);
								}
							}
						}
					}
				}
			}); // thread
			_pThread->detach();

			// event loop
			SDL_Event e;
			while (true)
			{
				if (_isFileModified.load())
				{
					// update the display with rgb data
					{
						std::lock_guard<std::mutex> lock{ this->_mutex }; // protecting pViewport
						SDL_UpdateTexture(_pTexture, nullptr, pViewport->data(), sizeof(rgba8Bits_t) * 560);
					}
					SDL_RenderClear(_pRenderer);
					SDL_RenderCopy(_pRenderer, _pTexture, NULL, NULL);
					SDL_RenderPresent(_pRenderer);
				}

				if (SDL_WaitEventTimeout(&e, 100))
				{
					if (e.type == SDL_QUIT) { break; }
				}

				if (error.happened) {
					throw std::runtime_error{ error.what };
				}
			}

		}

		void SdlError(const std::string& error,
			SDL_Window* const pWindow = nullptr,
			SDL_Renderer* const pRenderer = nullptr
		)
		{
			std::cout << "Error: " << error << '\n';

			if (pRenderer != nullptr) {
				SDL_DestroyRenderer(pRenderer);
			}
			if (pWindow != nullptr) {
				SDL_DestroyWindow(pWindow);
			}
			SDL_Quit();

			exit(1);
		}


		constexpr std::array<rgba8Bits_t, 7> Palette = {
			rgba8Bits_t{0x00,0x00,0x00, 0xFF}, // black
			rgba8Bits_t{0xFF,0xFF,0xFF, 0xFF}, // white
			rgba8Bits_t{0x07,0xA8,0xE0, 0xFF}, // blue
			rgba8Bits_t{0xF9,0x56,0x1D, 0xFF}, // orange
			rgba8Bits_t{0x43,0xC8,0x00, 0xFF}, // green
			rgba8Bits_t{0xBB,0x36,0xFF, 0xFF}, // violet
			rgba8Bits_t{0x80,0x80,0x80, 0xFF} // dummy as AppleWin's code can overflow :( (no time to correct it)
		};


		//===========================================================================
		// RGB videocards HGR
		
		//! @details Adapted from AppleWin: https://github.com/AppleWin/AppleWin
		//!					 A 14-dot block is used to draw a 7-pixel block (2 * 3.5-pixel blocks)
		//!					 Each dot of a 3.5-pixel block can be delayed so we draw on 2 subdots.
		//!					 28 subdots = 14 dots = 7 pixels (in 2 * 3.5-pixel blocks)
		//! @param	 x Vertical position of the 14-dot block 
		//! @param   pLineAddr pointer to the start of the line
		//! @param	 pOut pointer to the 28-subdot block to draw 
		void UpdateHiResRGBCell(const int x, const uint8_t* pLineAddr, rgba8Bits_t* pOut)
		{
			const int xpixel = x * 14;
			int xoffset = x & 1; // offset to start of the 2 bytes
			pLineAddr -= xoffset;

			// We need all 28 bits because each pixel needs a three bit evaluation
			uint8_t byteval1 = (x < 2 ? 0 : *(pLineAddr - 1));
			uint8_t byteval2 = *pLineAddr;
			uint8_t byteval3 = *(pLineAddr + 1);
			uint8_t byteval4 = (x >= 38 ? 0 : *(pLineAddr + 2));

			// all 28 bits chained
			uint32_t dwordval = (byteval1 & 0x7F) | ((byteval2 & 0x7F) << 7) | ((byteval3 & 0x7F) << 14) | ((byteval4 & 0x7F) << 21);

			// Extraction of 14 color pixels
			rgba8Bits_t colors[14];
			int idxColor = 0;
			uint32_t dwordval_tmp = dwordval;
			dwordval_tmp = dwordval_tmp >> 7;
			bool offset = (byteval2 & 0x80) ? true : false;
			for (int i = 0; i < 14; i++)
			{
				if (i == 7) offset = (byteval3 & 0x80) ? true : false;
				idxColor = dwordval_tmp & 0x3;
				// Two cases because AppleWin's palette is in a strange order
				if (offset)
					colors[i] = Palette[1 + idxColor];
				else
					colors[i] = Palette[6 - idxColor];
				if (i % 2) dwordval_tmp >>= 2;
			}
			// Black and White
			rgba8Bits_t bw[2];
			bw[0] = Palette[0];
			bw[1] = Palette[1];

			constexpr uint32_t mask = 0x01C0; //  00|000001 1|1000000
			constexpr uint32_t chck1 = 0x0140; //  00|000001 0|1000000
			constexpr uint32_t chck2 = 0x0080; //  00|000000 1|0000000

			// HIRES render in RGB works on a pixel-basis (1-bit data in framebuffer)
			// The pixel can be 'color', if it makes a 101 or 010 pattern with the two neighbour bits
			// In all other cases, it's black if 0 and white if 1
			// The value of 'color' is defined on a 2-bits basis
			if (xoffset)
			{
				// Second byte of the 14 pixels block
				dwordval = dwordval >> 7;
				xoffset = 7;
			}

			for (int i = xoffset; i < xoffset + 7; i++)
			{
				if (((dwordval & mask) == chck1) || ((dwordval & mask) == chck2))
				{
					// Color pixel
					*(pOut) = colors[i];
					*(pOut + 1) = *(pOut);
					pOut += 2;
				}
				else
				{
					// B&W pixel
					*(pOut) = bw[(dwordval & chck2 ? 1 : 0)];
					*(pOut + 1) = *(pOut);
					pOut += 2;
				}
				// Next pixel
				dwordval = dwordval >> 1;
			}
		}


		std::unique_ptr<Screen> ComputeRgbBuffer(const uint8_t* hires)
		{
			// Getting a RGB framebuffer
			// Converting line per line, pixel-block per pixel-block
			auto pViewport = make_unique<Screen>();
			auto itLine = std::begin(*pViewport);
			for (const auto lineBlockOffset : LineAdresses)
			{
				for (const auto lineOffset : LineOffsets)
				{
					const uint8_t* pHires = hires + lineBlockOffset + lineOffset;	// interleaved HIRES source line
					for (std::size_t x = 0; x < itLine->size(); ++x)
					{
						auto& block = (*itLine)[x];
						UpdateHiResRGBCell(static_cast<int>(x), pHires++, block.data());
					}
					*(itLine + 1) = *itLine;	// Doubling the destination lines
					itLine += 2;							// Next dest line
				}
			}

			return pViewport;
		}


  }
}
