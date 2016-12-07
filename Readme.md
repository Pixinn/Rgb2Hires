#Rgb2Hires
This program coverts an RGB image to the Apple II's "HiRes" format. Provided it complies with its constraints:

* Source image must be 140x280. Pixels are anamorphic: twice wider than tall.
*  Source image must contains six colors : BLACK, WHITE, ORANGE, GREEN, BLUE and PURPLE

For more information about the "Hires" format and its limitations, please refer to Wikipedia: https://en.wikipedia.org/wiki/Apple_II_graphics#High-Resolution_.28Hi-Res.29_graphics

## Compiling

### Rgb2Hires
The source is provided as a Visual Studio project. It's standard C++11 without any Windows dependency so it should compile on Linux and macOs without any modification.

#### Dependencies
* Magick++ : On Windows, you have provide an environment variable called *MAGICK_HOME* and pointing to the ImageMagick folder.

### Loader (for Apple II)
This program will compile as an Apple II executable that you can run on the actual hardware.
#### Dependencies
* CC65: Please provide an environment variable, *CC65_HOME*, pointing to your CC65 folder.

# Running
A correct source image, *test.png*, is provided as an example. Convert it into a file named *test.picture* as the Apple II Loader will try to load and display a file called like that.

