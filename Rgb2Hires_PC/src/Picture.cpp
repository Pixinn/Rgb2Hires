#include <memory>

#include "Picture.h"


using namespace std;

namespace RgbToHires
{

	Picture::Picture(const CImg<uint8_t>& source)
	{
		//Filling the storage with BlockHrs
		int lineNr = 0;
		for (auto& line : _blobRgb)
		{
			int colNr = 0;
			line.reserve(NB_BLOCKS_PER_LINE);
			//Useful data
			for (auto blockNr = 0u; blockNr < NB_BLOCKS_PER_LINE; ++blockNr)
			{
				BlockPixel blockPxRgb;
				for (auto& pxRgb : blockPxRgb)
				{
					pxRgb = {
						source(colNr, lineNr, 0, 0),
						source(colNr, lineNr, 0, 1),
						source(colNr, lineNr, 0, 2)
					};
					++colNr;
				}
				line.emplace_back(BlockHr{ blockPxRgb });
			}
			++lineNr;
		}

		//Constructing the map used to interleave the lines
		auto i = 0u;
		for (const auto& line : _blobRgb) {
			auto addr_interleaved = LineAdresses[i / 8] + LineOffsets[i % 8];
			_hrOrderedLines.insert(pair<const uint16_t, const LineHr*>(addr_interleaved, &line));
			++i;
		}
		//Adding the 8 byte "memory holes"
		for (auto line : _hrOrderedLines) {
			if ((line.first & 0xFF) == 0x50 || (line.first & 0xFF) == 0xD0) {
				for (auto i = 0u; i < 4u; ++i) {
					const_cast<LineHr*>(line.second)->emplace_back(BlockHr{});
				}
			}
		}
	}



	unique_ptr<array<uint8_t, Picture::FRAME_SIZE>> Picture::getBlob() const
	{
		auto blob = unique_ptr<array<uint8_t, FRAME_SIZE>>{ new array<uint8_t, FRAME_SIZE> };
		auto byte_blob = begin(*blob);
		for (const auto& line : _hrOrderedLines) {
			for (const auto& block : *(line.second)) {
				for (const auto byte_block : block) {
					*byte_blob++ = byte_block;
				}
			}
		}
		return blob;
	}



	string Picture::getAsm() const
	{
		string assembly{ "Picture:\n" };
		for (const auto& line : _hrOrderedLines) {
			assembly += "\t.byte\t";
			for (const auto& block : *(line.second)) {
				for (const auto byte : block) {
					assembly += to_string(byte) + ", ";
				}
			}
			assembly.pop_back(); //removing the last coma
			assembly.pop_back();
			assembly += "\n";
		}
		return assembly;
	}




	void Picture::Quantize()
	{
		/*const auto dim = size();
		if (dim.height() != HEIGHT || dim.width() != WIDTH) {
			throw std::runtime_error("Image dimension must be 140x192 pixels.");
		}
		auto pixelpacket = getPixels(0u, 0u, WIDTH, HEIGHT);
		for (auto i = 0u; i < HEIGHT; ++i)
		{
			for (auto j = 0u; j < WIDTH; ++j)
			{
				auto color = *pixelpacket;
				const auto distBlack = Distance(BLACK, color);
				const auto distWhite = Distance(WHITE, color);
				const auto distBlue = Distance(BLUE, color);
				const auto distGreen = Distance(GREEN, color);
				const auto distOrange = Distance(ORANGE, color);
				const auto distViolet = Distance(VIOLET, color);
				const auto distMin = std::min({ distBlack, distWhite, distBlue, \
												distGreen, distOrange, distViolet });

				if (distMin == distBlack) {
					*pixelpacket++ = BLACK;
				}
				else if (distMin == distWhite) {
					*pixelpacket++ = WHITE;
				}
				else if (distMin == distBlue) {
					*pixelpacket++ = BLUE;
				}
				else if (distMin == distGreen) {
					*pixelpacket++ = GREEN;
				}
				else if (distMin == distOrange) {
					*pixelpacket++ = ORANGE;
				}
				else {
					*pixelpacket++ = VIOLET;
				}
			}
		}*/
	}




	double Picture::Distance(const RGB_t& color1, const RGB_t& color2)
	{

		static constexpr double LUMA_RED = 0.299;
		static constexpr double LUMA_GREEN = 0.587;
		static constexpr double LUMA_BLUE = 0.114;

		const auto y1 = LUMA_RED * color1.r + LUMA_GREEN * color1.g + LUMA_BLUE * color1.b;
		const auto u1 = 0.492 * (color1.b - y1);
		const auto v1 = 0.877 * (color1.r - y1);
		const auto y2 = LUMA_RED * color2.r + LUMA_GREEN * color2.g + LUMA_BLUE * color2.b;
		const auto u2 = 0.492 * (color2.b - y2);
		const auto v2 = 0.877 * (color2.r - y2);

		const auto dy = (y1 - y2);
		const auto du = (u1 - u2);
		const auto dv = (v1 - v2);

		return (dy * dy + du * du + dv * dv);
	}

}
