
#include <exception>
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
		_data[0] |= (((byte>>1) & 0x1) << 6);
		_data[1] |=  (byte & 0x1);
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
			msg += to_string(color.redQuantum()) + to_string(color.greenQuantum()) + to_string(color.blueQuantum());
			throw(runtime_error(msg));
		}
	}



	/************************* CLASS HiRes **********************/




	HiRes::HiRes(const ImageQuantized& source)
	{
		auto pixel_src = source.getConstPixels(0u, 0u, WIDTH, HEIGHT);
		//Filling the storage with BlockHrs
		for (auto& line : _blob) {
			line.reserve(NB_BLOCK_PER_LINES);
			//Useful data
			for (auto blockNr = 0u; blockNr < NB_BLOCK_PER_LINES; ++blockNr ) {
				BlockPixel blockPxRgb;
				for (auto& pxRgb : blockPxRgb) {
					pxRgb = *pixel_src++;
				}
				line.emplace_back(BlockHr{ blockPxRgb });
			}
		}
		//Constructing the map used to interleave the lines
		auto i = 0u;
		for (const auto& line : _blob) {
			auto addr_interleaved = _lineAdresses[i / 8] + _lineOffsets[i % 8];
			_hrOrderedLines.insert(pair<const uint16_t, const LineHr*>(addr_interleaved, &line));
			++i;
		}
		//Adding the 8 byte "memory holes"
		for (auto line : _hrOrderedLines) {
			if ((line.first & 0xFF) == 0x50 || (line.first & 0xFF) == 0xD0) {
				for (auto i = 0u; i < 4u; ++i) {
					const_cast<LineHr*>(line.second)->emplace_back(BlockHr{});
				}
			}
		}
	}



	unique_ptr<array<uint8_t, HiRes::FRAME_SIZE>> HiRes::getBlob() const
	{
		auto blob = unique_ptr<array<uint8_t, FRAME_SIZE>>{ new array<uint8_t, FRAME_SIZE>};
		auto byte_blob = begin(*blob);
		for (const auto& line : _hrOrderedLines) {
			for (const auto& block : *(line.second)) {
				for (const auto byte_block : block) {
				*byte_blob++ = byte_block;
				}
			}
		}
		return blob;
	}



	string HiRes::getAsm() const 
	{
		string assembly{ "Picture:\n" };
		for (const auto& line : _hrOrderedLines) {
			assembly += "\t.byte\t";
			for (const auto& block : *(line.second)) {
				for (const auto byte : block) {
					assembly += to_string(byte) + ", ";
				}
			}
			assembly.pop_back(); //removing the last coma
			assembly.pop_back();
			assembly += "\n";
		}
		return assembly;
	}


}