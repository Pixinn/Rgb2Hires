
/* Rgb2Hires
* Copyright (C) 2016-2022 Christophe Meneboeuf <christophe@xtof.info>
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

#define SDL_MAIN_HANDLED

#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

#include <SDL2/SDL_image.h>
#include <tclap/CmdLine.h>

#include "ImageQuantized.h"
#include "Display.h"

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

	// Init
	int inited = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	if (inited != (IMG_INIT_JPG | IMG_INIT_PNG))
	{
		std::cerr << "IMG_Init: Failed to init required jpg and png support!\n"
		          << "IMG_Init: " <<  IMG_GetError() << '\n';
		return -1;
	}
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "There was an error initing SDL2: " << SDL_GetError() << std::endl;
		return -1;
	}

	//Parsing command line
	TCLAP::CmdLine cmd("Picture - by Christophe Meneboeuf <christophe@xtof.info>", ' ', "0");
	TCLAP::ValueArg<string> imagePath("i", "image", "Source image path", true, "", "path_to_image");
	TCLAP::ValueArg<string> outputPath("o", "output", "Output path", true, "", "path_to_output");
	TCLAP::SwitchArg assembly("a", "asm", "Output asm format");
	TCLAP::SwitchArg preview("p", "preview", "Open a window to display a preview.");
	cmd.add(imagePath);
	cmd.add(outputPath);
	cmd.add(assembly);
	cmd.add(preview);
	cmd.parse(argc, argv);

	if (imagePath.getValue().size() == 0 || outputPath.getValue().size() == 0) {
		std::cout << "No input or output path provided." << std::endl;				
		IMG_Quit();
		return -1;
	}

    
	const auto filepath = imagePath.getValue();
	if (!exists(filepath)) {
		ExitOnError("Cannot read " + filepath);
	}

	std::vector<SDL_Surface*> surfaces;
	SDL_Surface* surfaceRgb = IMG_Load(filepath.c_str());
	surfaces.push_back(surfaceRgb);
	if (surfaceRgb == nullptr)
	{
		ExitOnError("Cannot decode " + filepath, surfaces);
	}
	const ImageQuantized imageHiRes{ surfaceRgb };

	if (preview.getValue()) {
		const auto bytes = imageHiRes.getHiresBuffer();
		Display::Window::GetInstance()->display(filepath, bytes->data());
	}
	else
	{
		if (assembly.getValue() == true) {    //Ouput in ASM
			ofstream output(outputPath.getValue());
			output << imageHiRes.getHiresAsm();
		}
		else {	//Binary output
			ofstream output(outputPath.getValue(), ios::binary);
			const auto bytes = imageHiRes.getHiresBuffer();
			output.write(reinterpret_cast<const char*>(bytes.get()), bytes->size());
		}
	}

	for (auto surface : surfaces)
	{
		SDL_FreeSurface(surface);
	}

	SDL_Quit();
	IMG_Quit();

    return 0;
}

