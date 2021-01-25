
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


	#define WHITE Magick::Color{"white"}
	#define BLACK Magick::Color{"black"}
	#define BLUE Magick::Color{"blue"}
	#define GREEN Magick::Color{"green"}
	#define ORANGE Magick::Color{"orange"}
	#define VIOLET Magick::Color{"purple"}


	constexpr unsigned WIDTH = 140u;
	constexpr unsigned HEIGHT = 192u;

}

#endif
