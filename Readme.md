# Rgb2Hires

Rgb2Hires is a set of tools to help converting a modern RGB image (JPEG, PNG) to the HIRES format for Apple II computers ; either as a binary export or an assembly listing.

The color of the RGB imlage can be approximative: Rgb2Hires will match them with the nearest HIRES color.

## Apple II colors

RGB values of the targeted colors:

* **BLACK**:  00 00 00
* **WHITE**:  FF FF FF
* **BLUE**:   07 A8 E0
* **ORANGE**: F9 56 1D
* **GREEN**:  43 C8 00
* **VIOLET**: BB 36 FF

## Projects

This repository contains three "PC" projects:
* **libRgb2Hires**: a library to convert RGB image to the Apple II's HIRES format
* **Picture**: a program to convert a RGB png to a binary or an ASM excerpt, that can be loaded to the HIRES memory pages of an Apple II. An optional **live preview** can be displayed: its window will simulate an RGB monitor and will show the result, including color clashing and artifacts.
  * Source image must be 140x192. Pixels are anamorphic: they will be displayed twice wider than tall.
  * Source image must contains six colors : BLACK, WHITE, ORANGE, GREEN, BLUE and PURPLE. The color may be approximation of the Apple II 6 colors. Please refer to the provided pic.
* **Tile**: given a RGB png tile sheet that satisfies the same requirements as above, it extracts a 14x16 tile and converts it to ASM data. This data **is not interleaved**.

And one Apple II project:
* **Loader**: a program that will load and display a picture generated by the *Picture* program. A bash scripts is provided as an exemple to load the picture and the loader on an image disk. [AppleCommander](https://applecommander.github.io/) is required to do so. Once in ProDOS, just type *-DISPLAY*.

__Note:__ For more information about the "Hires" format and its limitations, you can refer to my [website](https://www.xtof.info/hires-graphics-apple-ii.html).


## Build from sources

This project has been tested with VisualStudio 2019, Gcc 9 (Ubuntu 20.04) and Gcc8.3.0 (Debian 10).
### Dependencies

* **Magick++** from ImageMagick 6 and **SDL2**
    * On **Windows**
      * ImageMagick
		   * Download ImageMagick 6 from the [official website](https://legacy.imagemagick.org/script/install-source.php).
		   * Compile it in *Dynamic Multithreaded*.
		   * Provide an environment variable called *MAGICK_HOME* and pointing to the root ImageMagick folder.
		   * Copy *ImageMagick-config* from the *script/* folder to *MAGICK_HOME*.
	  * SDL2
	    * Download the latest version of the *development library* from the [official website](https://www.libsdl.org/download-2.0.php) and unarchive it.
		 * Provide an environment variable called *SDL2_HOME* and pointing to the root of the SDL2 library.

    * On **Linux**, install libmagick++-dev for version 6 and libsdl2-dev
	    
      > sudo apt install libmagick++-6.q16-dev libsdl2-dev
      
      Depending on your Linux distribution, you may have to set an environment variable *sdl2_DIR*, pointing to the directory hosting SDL2's cmake configuration, before running cmake.

      Example on Debian:

      > export sdl2_DIR=/usr/lib/x86_64-linux-gnu/cmake/SDL2/

### How to build

* **Linux** a CMakeList is provided:
  > mkdir build && cd build

  > cmake ..

  > cmake --build . --config release
  
* **Windows** a VisualStudio solution is provided

## Apple II project (Loader)

### Dependencies
* [CC65](https://cc65.github.io/cc65/)
    * The crosscompiler suit. Please provide an environment variable, *CC65_HOME* pointing to your CC65 folder.
	
### Build

  > make

## Test

A correct source image, *test.png*, is provided as an example. Convert it using Picture, then copy it on a dsk image along with the Apple II loader. You can use or refer to the provided script in the *Loader_Apple2* folder.

### Demo

The live preview can help to visualise color clashing and artifacts. You can edit your file using *Photoshop* or any other application. The *Preview* window will be ubdated each tile the file is saved.

<a href="https://pub.xtof.info/github/demo-rgb2hires.mp4">
<img src="https://pub.xtof.info/github/demo-rgb2hires.png"/>
</a>
