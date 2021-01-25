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

#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "ImageQuantized.h"
#include "HiRes.h"

namespace RgbToHires
{

	/// @brief A fullscreen HIRES picture
	class Picture
	{
		static constexpr unsigned NB_BLOCKS_PER_LINE = 20u;
		static constexpr unsigned NB_LINES_PER_SCREEN = 192u;

	public:
		static constexpr unsigned FRAME_SIZE = 192 * 40 + 512; ///< Frame size in byte

		Picture(const ImageQuantized&);
		~Picture() = default;

		/// @brief Returns the binary hires picture
		std::unique_ptr <std::array<uint8_t, FRAME_SIZE>> getBlob() const;
		/// @brief Returns asm code corresponding to the image in memory (CA65 format)
		std::string getAsm() const;
		 
	private:

		using LineHr = std::vector<BlockHr>;
		using Blob = std::array<LineHr, NB_LINES_PER_SCREEN>;

		Blob    _blob;	///< A frame ordered buffer of hires data		
		static constexpr std::array<const uint16_t, 192 / 8> _lineAdresses = {
			0x2000, 0x2080, 0x2100, 0x2180, 0x2200, 0x2280, 0x2300, 0x2380,
			0x2028, 0x20a8, 0x2128, 0x21a8, 0x2228, 0x22a8, 0x2328, 0x23a8,
			0x2050, 0x20d0, 0x2150, 0x21d0, 0x2250, 0x22d0, 0x2350, 0x23d0
		};
		static constexpr std::array<const uint16_t, 8> _lineOffsets = {
			0x0, 0x400, 0x800, 0xc00, 0x1000, 0x1400, 0x1800, 0x1c00
		};
		std::map<const uint16_t, const LineHr*> _hrOrderedLines; ///< map<adress,line's data>
	};

}

#endif

