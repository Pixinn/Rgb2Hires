#include <sstream>
#include <iomanip>
#include "Tile.h"

RgbToHires::Tile::Tile(const ImageQuantized& source, const unsigned col, const unsigned line)
{
	constexpr unsigned w_tile = 14;
	constexpr unsigned h_tile = 16;
  auto pixel_src = source.getConstPixels(col * w_tile, line * h_tile, w_tile, h_tile);

	//Filling the storage with BlockHrs
	for (auto& line : _blob) {
		line.reserve(NB_BLOCKS_PER_TILE);
		//Useful data
		for (auto blockNr = 0u; blockNr < NB_BLOCKS_PER_TILE; ++blockNr) {
			BlockPixel blockPxRgb;
			for (auto& pxRgb : blockPxRgb) {
				pxRgb = *pixel_src++;
			}
			line.emplace_back(BlockHr{ blockPxRgb });
		}
	}

}

std::string RgbToHires::Tile::getHiresAsm() const
{
	std::string assembly;
	for (const auto& line : _blob)
	{
		std::stringstream stream;
		stream << ".byte ";
		for (const auto& block :line)
		{
			for (const auto byte : block) {
				stream << '$'  << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << static_cast<int>(byte) << ", ";
			}
		}

		assembly += stream.str();
		assembly.pop_back(); //removing the last coma
		assembly.pop_back();
		assembly += "\n";
	}
	assembly.pop_back(); // removing the final CR
	return assembly;
}
