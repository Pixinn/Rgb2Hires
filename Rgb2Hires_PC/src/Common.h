
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


namespace RgbToHires {

	static Magick::Color WHITE("white");
	static Magick::Color BLACK("black");
	static Magick::Color BLUE("blue");
	static Magick::Color GREEN("green");
	static Magick::Color ORANGE("orange");
	static Magick::Color VIOLET("purple");

	static constexpr unsigned WIDTH = 140u;
	static constexpr unsigned HEIGHT = 192u;

}

#endif
