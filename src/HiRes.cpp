
#include <exception>
#include "ImageQuantized.h"
#include "HiRes.h"


using namespace std;

namespace RgbToHires {


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
		_data[0] |= (((byte>>1) & 0x1) << 6);
		_data[1] |=  (byte & 0x1);
	}


	std::pair<BlockHr::eColorGroup, BlockHr::eColorGroup> BlockHr::getGroup(const BlockPixel& block) const
	{
		std::pair<eColorGroup, eColorGroup> groups{ GROUP_1, GROUP_1 };
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
			msg += to_string(color.redQuantum()) + to_string(color.greenQuantum()) + to_string(color.blueQuantum());
			throw(runtime_error(msg));
		}
	}


	HiRes::HiRes(const ImageQuantized& source)
	{
		auto pixel_src = source.getConstPixels(0u, 0u, WIDTH, HEIGHT);

		for (auto& line : _blob) {
			for (auto& block : line) {
				BlockPixel blockPxRgb;
				for (auto& pxRgb : blockPxRgb) {
					pxRgb = *pixel_src++;
				}
				block = BlockHr{ blockPxRgb };
			}
		}

	}






}