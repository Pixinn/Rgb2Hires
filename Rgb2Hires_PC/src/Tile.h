#ifndef _TILE_H_
#define _TILE_H_

#include "ImageQuantized.h"
#include "HiRes.h"

namespace RgbToHires
{

  static constexpr unsigned NB_BLOCKS_PER_TILE = 2u;
  static constexpr unsigned NB_LINES_PER_TILE = 16u;

  /// @brief A 14x16 tile in hires
  class Tile
  {
  public:
    /// @brief Constructs the tile from the upper left corner of a 140x192 quantized image
    Tile(const ImageQuantized&);
    ~Tile() = default;

    /// @brief Returns asm code corresponding to the tile: the lines are not interleaved!
    std::string getAsm() const;

  private:
    using LineHr = std::vector<BlockHr>;
    using Blob = std::array<LineHr, NB_LINES_PER_TILE>;

    Blob    _blob;	///< A tile of ordered hires data
  };

}




#endif


