#ifndef __IMAGEQUANTIZED_H__
#define __IMAGEQUANTIZED_H__

#include <memory>
#include <array>
#include "Common.h"


namespace RgbToHires {

	class ImageQuantized :
		public Magick::Image
	{
	public:

		using Block = std::array<Magick::PixelPacket,7>;
		using Line = std::array<Block, 20>;
		using Blob = std::array<Line, 192>;

		ImageQuantized(const Magick::Image& src);
		~ImageQuantized()=default;

		std::unique_ptr<Blob> getBlob() const;

	private:

		double Distance(const Magick::Color&, const Magick::Color&);

	};

}


#endif
