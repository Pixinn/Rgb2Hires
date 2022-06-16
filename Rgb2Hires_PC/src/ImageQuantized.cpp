
/*Rgb2Hires
* Copyright (C) 2016-2022 Christophe Meneboeuf <christophe@xtof.info>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdexcept>
#include <iterator>

#include "ImageQuantized.h"

using namespace std;

namespace RgbToHires {


	ImageQuantized::ImageQuantized(SDL_Surface* const src) :
		_format(SDL_PIXELFORMAT_BGRX8888)
	{
		if (src->h != HEIGHT || src->w != WIDTH) {
			throw std::runtime_error("Image dimension must be 140x192 pixels.");
		}

		SDL_Surface* rgb = SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_BGRX8888, 0);

		SDL_LockSurface(rgb);
		auto srcPx = reinterpret_cast<const uint32_t*>(rgb->pixels);
		SDL_UnlockSurface(rgb);

		// Construct HIRES image: lines are not properly ordered
		for (auto& line : _blobHr)
		{
			for (auto blockNr = 0u; blockNr < NB_BLOCK_PER_LINE; ++blockNr)
			{
				BlockRgb blockRgb;
				for (auto& pixel : blockRgb)
				{
					ColorRgb color;
					SDL_GetRGB(*srcPx, rgb->format, &color.r, &color.g, &color.b);
					pixel = Quantize(color);
					srcPx++;
				}
				line.emplace_back(BlockHr{ blockRgb });
			}
		}

		//Constructing the map used to interleave the lines
		auto i = 0u;
		for (const auto& line : _blobHr) 
		{
			uint16_t addr_interleaved = LineAdresses[i / 8] + LineOffsets[i % 8];
			_hrOrderedLines.insert(std::pair<uint16_t, const LineHr*>(addr_interleaved, &line));
			++i;
		}
		//Adding the 8 byte "memory holes"
		for (auto line : _hrOrderedLines) 
		{
			if ((line.first & 0xFF) == 0x50 || (line.first & 0xFF) == 0xD0) 
			{
				for (auto i = 0u; i < 4u; ++i) 
				{
					const_cast<LineHr*>(line.second)->emplace_back(BlockHr{});
				}
			}
		}
	}


	unique_ptr<array<uint8_t, ImageQuantized::FRAME_SIZE>> ImageQuantized::getHiresBuffer() const
	{
		auto blob = unique_ptr<array<uint8_t, FRAME_SIZE>>{ new array<uint8_t, FRAME_SIZE> };
		auto byte_blob = begin(*blob);
		for (const auto& line : _hrOrderedLines) 
		{
			for (const auto& block : *(line.second))
			{
				for (const auto byte_block : block) 
				{
					*byte_blob++ = byte_block;
				}
			}
		}
		return blob;
	}



	string ImageQuantized::getHiresAsm() const
	{
		string assembly{ "Picture:\n" };
		for (const auto& line : _hrOrderedLines) 
		{
			assembly += "\t.byte\t";
			for (const auto& block : *(line.second)) 
			{
				for (const auto byte : block) 
				{
					assembly += to_string(byte) + ", ";
				}
			}
			assembly.pop_back(); //removing the last coma
			assembly.pop_back();
			assembly += "\n";
		}
		return assembly;
	}




	double ImageQuantized::Distance(const ColorRgb& color1, const ColorRgb& color2)
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


	inline ColorRgb ImageQuantized::Quantize(const ColorRgb& color)
	{
		const auto distBlack = Distance(BLACK, color);
		auto distMin = distBlack;
		const auto distWhite = Distance(WHITE, color);
		if (distMin > distWhite) { distMin = distWhite; }
		const auto distBlue = Distance(BLUE, color);
		if (distMin > distBlue) { distMin = distBlue; }
		const auto distGreen = Distance(GREEN, color);
		if (distMin > distGreen) { distMin = distGreen; }
		const auto distOrange = Distance(ORANGE, color);
		if (distMin > distOrange) { distMin = distOrange; }
		const auto distViolet = Distance(VIOLET, color);
		if (distMin > distViolet) { distMin = distViolet; }

		if (distMin == distBlack) {
			return BLACK;
		}
		else if (distMin == distWhite) {
			return WHITE;
		}
		else if (distMin == distBlue) {
			return BLUE;
		}
		else if (distMin == distGreen) {
			return GREEN;
		}
		else if (distMin == distOrange) {
			return ORANGE;
		}
		else {
			return VIOLET;
		}
	}

}