
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

#include <stdexcept>
#include "ImageQuantized.h"
#include "HiRes.h"


using namespace std;

namespace RgbToHires {


	/************************* CLASS BlockHr **********************/

	BlockHr::BlockHr()
	{
		_data[0] = 0;
		_data[1] = 0;
	}

	BlockHr::BlockHr(const BlockPixel& source)
	{
		const auto groups = getGroup(source);
		//Init data, depending on the group
		groups.first == GROUP_1 ? _data[0] = 0x0 : _data[0] = 0x80;
		groups.second == GROUP_1 ? _data[1] = 0x0 : _data[1] = 0x80;
		//Getting the bit pairs
		//Left 7 bit group
		_data[0] |= getDibit(source[0]);
		_data[0] |= (getDibit(source[1]) << 2);
		_data[0] |= (getDibit(source[2]) << 4);
		//Right 7 bit group
		_data[1] |= getDibit(source[4])  << 1;
		_data[1] |= (getDibit(source[5]) << 3);
		_data[1] |= (getDibit(source[6]) << 5);
		//Middle pixel
		auto byte = getDibit(source[3]);
		_data[0] |= ((byte & 0x1) << 6);
		_data[1] |=  ((byte >> 1) & 0x1);
	}


	pair<BlockHr::eColorGroup, BlockHr::eColorGroup> BlockHr::getGroup(const BlockPixel& block) const
	{
		pair<eColorGroup, eColorGroup> groups{ GROUP_1, GROUP_1 };
		//1st block group, including the last semi-pixel
		for (auto i = 0u; i < 4u; ++i) {
			if (block[i] == GREEN || block[i] == VIOLET) {
				break;
			}
			else if (block[i] == ORANGE || block[i] == BLUE) {
				groups.first = GROUP_2;
				break;
			}
		}
		//2nd block group, excluding the first semi-pixel
		for (auto i = 4u; i < 7u; ++i) {
			if (block[i] == GREEN || block[i] == VIOLET) {
				break;
			}
			else if (block[i] == ORANGE || block[i] == BLUE) {
				groups.second = GROUP_2;
				break;
			}
		}
		return groups;
	}


	uint8_t BlockHr::getDibit(const Magick::Color& color) const
	{
		if (color == WHITE) {
			return 3;
		}
		else if (color == BLACK) {
			return 0;
		}
		else if (color == VIOLET || color == BLUE) {
			return 1;
		}
		else if (color == GREEN || color == ORANGE) {
			return 2;
		}
		else {
			auto msg = string("Unsupported color used as input for hires pixel\nRGB:");
			msg = msg +	to_string(color.redQuantum()) + to_string(color.greenQuantum()) + to_string(color.blueQuantum());
			throw(runtime_error(msg));
		}
	}

}