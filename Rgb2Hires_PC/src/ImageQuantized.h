
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


#ifndef __IMAGEQUANTIZED_H__
#define __IMAGEQUANTIZED_H__

#include <memory>
#include <array>
#include "Common.h"


namespace RgbToHires {

    /// @brief Image quantized to the HIRES colors.
    /// @details Quantization works with a nearest distance algorithm.
	class ImageQuantized :
		public Magick::Image
	{
	public:

		using Block = std::array<Magick::PixelPacket,7>;
		using Line = std::array<Block, 20>;
		using Blob = std::array<Line, 192>;

		ImageQuantized(const Magick::Image& src);
		~ImageQuantized()=default;

        /// @brief Returns an array of bytes forming the RGB quantized image
		std::unique_ptr<Blob> getBlob() const;

	private:
        /// @brief Computes the euclidian distance between two colors
		double Distance(const Magick::Color&, const Magick::Color&);

	};

}


#endif
