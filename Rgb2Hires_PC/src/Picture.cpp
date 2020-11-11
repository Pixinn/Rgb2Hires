#include "Picture.h"


using namespace std;

namespace RgbToHires
{

	Picture::Picture(const ImageQuantized& source)
	{
		auto pixel_src = source.getConstPixels(0u, 0u, WIDTH, HEIGHT);
		//Filling the storage with BlockHrs
		for (auto& line : _blob) {
			line.reserve(NB_BLOCKS_PER_LINE);
			//Useful data
			for (auto blockNr = 0u; blockNr < NB_BLOCKS_PER_LINE; ++blockNr) {
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



	unique_ptr<array<uint8_t, Picture::FRAME_SIZE>> Picture::getBlob() const
	{
		auto blob = unique_ptr<array<uint8_t, FRAME_SIZE>>{ new array<uint8_t, FRAME_SIZE> };
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



	string Picture::getAsm() const
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
