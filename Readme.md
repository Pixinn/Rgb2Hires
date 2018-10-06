#Rgb2Hires
This program converts an RGB image to the Apple II's "HiRes" format. Provided it complies with this format's constraints:

* Source image must be 140x192. Pixels are anamorphic: twice wider than tall.
* Source image must contains six colors : BLACK, WHITE, ORANGE, GREEN, BLUE and PURPLE

For more information about the "Hires" format and its limitations, please refer to Wikipedia: https://en.wikipedia.org/wiki/Apple_II_graphics#High-Resolution_.28Hi-Res.29_graphics

## Compiling

This repository contains two projects:
* Rgb2Hires, the conversion programm running on a PC
* Loader, for testing purposes. This program runs on Apple II hardware and displays an HiRes binary file named *test.picture* and located on the same disk.

### Rgb2Hires
The source is provided as a Visual Studio project. Howerver, it is standard C++11 without any Windows dependency, so it should compile on Linux and macOs without any modification.

#### Dependencies
* Magick++
    * On Windows, you have provide an environment variable called *MAGICK_HOME* and pointing to the ImageMagick folder.
    * On Linux, install libmagick++-dev

### Loader (for Apple II)
This program will compile as an Apple II executable that you can run on the actual hardware.
It is a Makefile project and must be crosscompiled using **[CC65](https://cc65.github.io/cc65/)**

#### Dependencies
* CC65: The crosscompiler suit. Please provide an environment variable, *CC65_HOME* pointing to your CC65 folder.

# Running
A correct source image, *test.png*, is provided as an example. Convert it into a file named *test.picture* and copy it along the Apple II *Loader*, then execute on the actual hardware ;)


