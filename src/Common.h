#ifndef __COMMON_H__
#define __COMMON_H__


#include <Magick++.h>


namespace RgbToHires {

	static Magick::Color WHITE("white");
	static Magick::Color BLACK("black");
	static Magick::Color BLUE("blue");
	static Magick::Color GREEN("green");
	static Magick::Color ORANGE("orange");
	static Magick::Color VIOLET("purple");

	static constexpr unsigned WIDTH = 140u;
	static constexpr unsigned HEIGHT = 192u;

}

#endif
