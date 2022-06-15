
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

#include <cstdint>
#include <SDL2/SDL.h>

namespace RgbToHires {

  struct Color : public SDL_Color
  {
    inline bool operator==(const Color& rhs) const
    {
      return r == rhs.r && g == rhs.g && b == rhs.b;
    }
    inline bool operator==(const SDL_Color& rhs) const
    {
      return r == rhs.r && g == rhs.g && b == rhs.b;
    }
  };

constexpr Color WHITE {0xFF, 0xFF, 0xFF, 0xFF};
constexpr Color BLACK {0x00, 0x00, 0x00, 0xFF};
constexpr Color BLUE  {0x07, 0xA8, 0xE0, 0xFF};
constexpr Color GREEN {0x43, 0xC8, 0x00, 0xFF};
constexpr Color ORANGE{0xF9, 0x56, 0x1D, 0xFF};
constexpr Color VIOLET{0xBB, 0x36, 0xFF, 0xFF};

constexpr unsigned WIDTH = 140u;
constexpr unsigned HEIGHT = 192u;

}

#endif
