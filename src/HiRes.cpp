
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
		const auto group = getGroup(source);
		//Init data, depending on the group
		for (auto& byte : _data) {
			//Hi bit depending on color group
			group == GROUP_1 ? byte = 0x0 : byte = 0x80;
		}
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


	BlockHr::eColorGroup BlockHr::getGroup(const BlockPixel& block) const
	{
		auto group = GROUP_1;
		for (const auto& color : block) {
			if (color == GREEN || color == VIOLET) {
				break;
			}
			if (color == ORANGE || color == BLUE) {
				group = GROUP_2;
				break;
			}
		}
		return group;
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

		for (auto& line : _blobHr) {
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