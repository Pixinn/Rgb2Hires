
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


#ifndef __HIRES_H__
#define __HIRES_H__

#include <cstdint>
#include <array>
#include <vector>
#include <string>

#include "Common.h"


namespace RgbToHires
{

	constexpr std::array<const uint16_t, 192 / 8> LineAdresses = {
		 0x0000, 0x0080, 0x0100, 0x0180, 0x0200, 0x0280, 0x0300, 0x0380,
		 0x0028, 0x00a8, 0x0128, 0x01a8, 0x0228, 0x02a8, 0x0328, 0x03a8,
		 0x0050, 0x00d0, 0x0150, 0x01d0, 0x0250, 0x02d0, 0x0350, 0x03d0
	};

	constexpr std::array<const uint16_t, 8> LineOffsets = {
	0x0, 0x400, 0x800, 0xc00, 0x1000, 0x1400, 0x1800, 0x1c00
	};

	using BlockPixel = std::array<Magick::PixelPacket, 7u>;
	
	/// @brief A block of 7 pixels
	class BlockHr
	{
	public:
		/// @brief Default constructor: all black
		BlockHr();
		/// @brief Construction from 7 pixels
		BlockHr(const BlockPixel& );
		/// @brief returns the position of the first element
		inline std::array<uint8_t, 2>::const_iterator begin() const {
			return _data.begin();
		}
		/// @brief returns the position after the last element
		inline std::array<uint8_t, 2>::const_iterator end() const {
			return _data.end();
		}
	private:
		/// @brief color group as defined in Apple's documentation
		enum eColorGroup {
			GROUP_1,
			GROUP_2,
			UNDEF	// black and white can be group1 or group2
		};
		/// @brief Returns the color group of these two 3.5 pixel blocks
		std::pair<eColorGroup, eColorGroup> getGroup(const BlockPixel&) const;
		/// @brief Returns the bit pait corresponding to the given color
		uint8_t getDibit(const Magick::Color&) const;

		std::array<uint8_t, 2> _data;
	};
	


}
#endif