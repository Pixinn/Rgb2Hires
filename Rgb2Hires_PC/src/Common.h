
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


#ifndef __COMMON_H__
#define __COMMON_H__


#include <Magick++.h>

#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 1

namespace RgbToHires {

	constexpr std::size_t SHIFT = sizeof(Magick::Quantum)*8 - 8;

	#define WHITE Magick::Color{0xFF << SHIFT, 0xFF << SHIFT, 0xFF << SHIFT}
	#define BLACK Magick::Color{0x00 << SHIFT, 0x00 << SHIFT, 0x00 << SHIFT}
	#define BLUE Magick::Color{0x07 << SHIFT, 0xA8 << SHIFT, 0xE0 << SHIFT}
	#define GREEN Magick::Color{0x43 << SHIFT, 0xC8 << SHIFT, 0x00 << SHIFT}
	#define ORANGE Magick::Color{0xF9 << SHIFT, 0x56 << SHIFT,0x1D << SHIFT}
	#define VIOLET Magick::Color{0xBB << SHIFT, 0x36 << SHIFT, 0xFF << SHIFT}


	constexpr unsigned WIDTH = 140u;
	constexpr unsigned HEIGHT = 192u;

}

#endif
