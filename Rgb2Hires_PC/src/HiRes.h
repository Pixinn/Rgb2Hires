
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


namespace RgbToHires {

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
			GROUP_2
		};
		/// @brief Returns the color group of these two 3.5 pixel blocks
		std::pair<eColorGroup, eColorGroup> getGroup(const BlockPixel&) const;
		/// @brief Returns the bit pait corresponding to the given color
		uint8_t getDibit(const Magick::Color&) const;

		std::array<uint8_t, 2> _data;
	};
	


}
#endif