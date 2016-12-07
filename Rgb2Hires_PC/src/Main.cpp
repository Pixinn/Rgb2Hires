
/* Rgb2Hires
* Copyright (C) 2016 Christophe Meneboeuf <christophe@xtof.info>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include <exception>
#include <iostream>
#include <fstream>
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

/// \brief Program entry point
int main( int argc, char *argv[] )
{
	Magick::InitializeMagick(*argv);

	//Parsing command line
	TCLAP::CmdLine cmd("rgbtohires", ' ', "0");
	TCLAP::ValueArg<string> imagePath("i", "image", "Source image path", true, "", "path_to_image");
	TCLAP::ValueArg<string> outputPath("o", "output", "Output path", true, "", "path_to_output");
	TCLAP::SwitchArg assembly("a", "asm", "Output asm format");
	cmd.add(imagePath);
	cmd.add(outputPath);
	cmd.add(assembly);
	cmd.parse(argc, argv);

	if (imagePath.getValue().size() == 0 || outputPath.getValue().size() == 0) {
		return -1;
	}

	try {
		const auto filepath = imagePath.getValue();
		if (!exists(filepath)) {
			throw runtime_error("Cannot read " + filepath);
		}
		const auto imageRgb = Magick::Image{ filepath };
		auto imageQuantized = ImageQuantized{ imageRgb };
		const auto imageHiRes = HiRes{ imageQuantized };		
		if (assembly.getValue() == true) {    //Ouput in ASM
			ofstream output(outputPath.getValue());
			output << imageHiRes.getAsm();
		}
		else {	//Binary output
			ofstream output(outputPath.getValue(), ios::binary);
			const auto bytes = imageHiRes.getBlob();
			output.write(reinterpret_cast<const char*>(bytes.get()), bytes->size());
		}
	}

	//Fatal error
	catch (const exception& e) {
		cout << e.what();
		return -1;
	}

    return 0;
}

