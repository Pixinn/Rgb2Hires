#ifndef __HIRES_H__
#define __HIRES_H__

#include <cstdint>
#include <array>

#include "Common.h"


namespace RgbToHires {

	using BlockPixel = std::array<Magick::PixelPacket, 7>;
	using LinePixel = std::array<BlockPixel, 20>;
	
	class BlockHr
	{
	public:
		/// \brief Default constructor: all black
		BlockHr();
		/// \brief Construction from 7 pixels
		BlockHr(const BlockPixel& );
	private:
		/// \brief color group as defined in Apple's documentation
		enum eColorGroup {
			GROUP_1,
			GROUP_2
		};
		/// \brief Returns the color group of these two pixel blocks
		///		   Works on double blocks instead of single blocks
		eColorGroup getGroup(const BlockPixel&) const;
		/// \brief Returns the bit pait corresponding to the given color
		uint8_t getDibit(const Magick::Color&) const;

		std::array<uint8_t, 2> _data;
	};
	
	using LineHr = std::array<BlockHr, 10>;
	template<class T>
	using Blob = std::array<T, 192>;

	class HiRes
	{
	public:
		HiRes(const ImageQuantized&);
		~HiRes() = default;


	private:
		
		Blob<LinePixel> _blobPx;	///< A frame ordered buffer of pixels
		Blob<LineHr>    _blobHr;	///< A frame ordered buffer of hires data
	};


}
#endif