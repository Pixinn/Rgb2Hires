
/* Rgb2Hires
* Copyright (C) 2016 Christophe Meneboeuf <christophe@xtof.info>
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


#include <iterator>
#include "ImageQuantized.h"

using namespace std;

namespace RgbToHires {


	ImageQuantized::ImageQuantized(const Magick::Image& src):
		Magick::Image(src)
	{
		const auto dim = size();
		if (dim.height() != HEIGHT || dim.width() != WIDTH) {
			throw std::runtime_error("Image dimension must be 140x192 pixels.");
		}
		auto pixelpacket = getPixels(0u, 0u, WIDTH, HEIGHT);
		for (auto i = 0u; i < HEIGHT; ++i)
		{
			for (auto j = 0u; j < WIDTH; ++j)
			{
				auto color =*pixelpacket;
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
		}
		syncPixels();
	}


	std::unique_ptr<ImageQuantized::Blob> ImageQuantized::getBlob() const
	{
		auto blob = std::unique_ptr<Blob>(new Blob);
		auto pixels = getConstPixels(0u, 0u, WIDTH, HEIGHT);
		for (auto& line : *blob) {
			for (auto& block : line) {
				for (auto pixel: block ) {
					pixel = *pixels++;
				}
			}
		}
		return blob;
	}




	double ImageQuantized::Distance(const Magick::Color& color1, const Magick::Color& color2)
	{
		return	  pow(color1.redQuantum() - color2.redQuantum(), 2) \
			    + pow(color1.greenQuantum() - color2.greenQuantum(), 2) \
			    + pow(color1.blueQuantum() - color2.blueQuantum(), 2);
	}


}