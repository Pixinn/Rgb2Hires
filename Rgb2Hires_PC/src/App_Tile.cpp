
/* Rgb2Hires
* Copyright (C) 2020 Christophe Meneboeuf <christophe@xtof.info>
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
#include <sys/stat.h>

#include <SDL2/SDL_image.h>
#include <tclap/CmdLine.h>

#include "ImageQuantized.h"
#include "Tile.h"

using namespace std;
using namespace RgbToHires;




/// @brief Returns true if a file exists
inline bool exists(const std::string& path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}


void ExitOnError(const std::string& message,
	std::vector<SDL_Surface*> surfaces = {})
{
	std::cout << "Error\n" << message << '\n';

	for (auto surface : surfaces)
	{
		SDL_FreeSurface(surface);
	}

	SDL_Quit();
	IMG_Quit();
	exit(-1);
}

/// @brief Program entry point
int main( int argc, char *argv[] )
{

	//Parsing command line
	TCLAP::CmdLine cmd("Tile - by Christophe Meneboeuf <christophe@xtof.info>", ' ', "0");
	TCLAP::ValueArg<string> imagePath("i", "image", "Source image path", true, "", "string");
	TCLAP::ValueArg<string> outputPath("o", "output", "Output path", true, "", "string");
	TCLAP::ValueArg<unsigned> column("c", "column", "Column number in the tile sheet. Starts at 0.", true, 0u, "integer");
	TCLAP::ValueArg<unsigned> line("l", "line", "Line number in the tile sheet. Starts at 0.", true, 0u, "integer");
	cmd.add(imagePath);
	cmd.add(outputPath);
	cmd.add(column);
	cmd.add(line);
	cmd.parse(argc, argv);

	if (imagePath.getValue().size() == 0 || outputPath.getValue().size() == 0) {
		std::cout << "No input or output path provided." << std::endl;
		return -1;
	}
	if (column.getValue() > 9) {
		std::cout << "Column number shall be < 9" << std::endl;
		return -1;
	}
	if (line.getValue() > 11) {
		std::cout << "Line number shall be < 11" << std::endl;
		return -1;
	}
    
	const auto filepath = imagePath.getValue();
	if (!exists(filepath)) {
		ExitOnError("Cannot read " + filepath);
	}

	std::vector<SDL_Surface*> surfaces;
	try
	{
		SDL_Surface* surfaceRgb = IMG_Load(filepath.c_str());
		surfaces.push_back(surfaceRgb);
		if (surfaceRgb == nullptr)
		{
			ExitOnError("Cannot decode " + filepath, surfaces);
		}
		const ImageQuantized imageHiRes{ surfaceRgb };
		const auto tileHiRes = imageHiRes.getTile(column.getValue(), line.getValue());

		// Always output in asm
		ofstream output(outputPath.getValue());
		output << tileHiRes;
	}
	catch (const std::exception& e)
	{
		ExitOnError(e.what(), surfaces);
	}

	for (auto surface : surfaces)
	{
		SDL_FreeSurface(surface);
	}


	SDL_Quit();
	IMG_Quit();

    return 0;
}

