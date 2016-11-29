
#include <exception>
#include <iostream>
#include <string>

#include <Magick++.h>
#include <tclap/CmdLine.h>

#include "ImageQuantized.h"
#include "HiRes.h"

using namespace std;
using namespace RgbToHires;

/// \brief Returns true if a file exists
inline bool exists(const std::string& path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

int main( int argc, char *argv[] )
{
	Magick::InitializeMagick(*argv);

	//Parsing command line
	TCLAP::CmdLine cmd("rgbtohires", ' ', "0");
	TCLAP::ValueArg<string> imagePath("i", "image", "Image path", true, "", "path_to_image");
	cmd.add(imagePath);
	cmd.parse(argc, argv);

	try {
		const auto filepath = imagePath.getValue();
		if (!exists(filepath)) {
			throw runtime_error("Cannot read " + filepath);
		}
		const auto imageRgb = Magick::Image{ filepath };
		auto imageQuantized = ImageQuantized{ imageRgb };
		const auto imageHiRes = HiRes{ imageQuantized };
	}

	//Fatal error
	catch (const exception& e) {
		cout << e.what();
		return -1;
	}

    return 0;
}

