
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
#include <map>
#include <string>

#include "Common.h"
#include "HiRes.h"

namespace RgbToHires {

    /// @brief Image quantized to the HIRES colors.
    /// @details Quantization works with a nearest distance algorithm.
	class ImageQuantized
	{
	public:

		// rgb
		using BlockRgb = std::array<Color, NB_PIXEL_PER_BLOCK>;
		using Line = std::array<BlockRgb, NB_BLOCK_PER_LINE>;
		using BlobRgb = std::array<Line, NB_LINES_PER_SCREEN>;
		// hires
		using LineHr = std::vector<BlockHr>;
		using BlobHr = std::array<LineHr, NB_LINES_PER_SCREEN>;

		static constexpr unsigned FRAME_SIZE = 192 * 40 + 512; ///< Frame size in byte

		ImageQuantized(SDL_Surface* const source);
		~ImageQuantized() = default;

		/// @brief Returns the binary hires picture
		std::unique_ptr <std::array<uint8_t, FRAME_SIZE>> getHiresBuffer() const;
		/// @brief Returns asm code corresponding to the image in memory (CA65 format)
		std::string getHiresAsm() const;
	  /// @brief Returns an HIRES block 

	private:
		Color Quantize(const Color& color);
    /// @brief Computes the euclidian distance between two colors
		double Distance(const Color&, const Color&);


	private:
		const  SDL_PixelFormatEnum _format;
		BlobHr  _blobHr;
		std::map<const uint16_t, const LineHr*> _hrOrderedLines; ///< map<adress,line's data>
	};

}


#endif
