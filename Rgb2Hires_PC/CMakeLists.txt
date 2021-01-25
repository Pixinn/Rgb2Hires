cmake_minimum_required (VERSION 3.10)
project (Rgb2Hires)

# The version number.
set (Rgb2Hires_VERSION_MAJOR 1)
set (Rgb2Hires_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 17)     
set(CMAKE_VERBOSE_MAKEFILE ON)

# dependencies
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

# library
add_library(${PROJECT_NAME} src/Common.h
							src/HiRes.cpp
							src/HiRes.h
							src/ImageQuantized.cpp
							src/ImageQuantized.h
							src/Picture.h
							src/Picture.cpp
							src/Tile.h
							src/Tile.cpp
)
target_compile_definitions(${PROJECT_NAME} PRIVATE MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(${PROJECT_NAME} PRIVATE ${ImageMagick_Magick++_INCLUDE_DIRS})


# Application Picture
add_executable(Picture src/App_Picture.cpp)
target_compile_definitions(Picture PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(Picture PRIVATE src ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(Picture ${ImageMagick_LIBRARIES} ${PROJECT_NAME})    

# Application Tile
add_executable(Tile src/App_Tile.cpp)
target_compile_definitions(Tile PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(Tile PRIVATE src ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(Tile ${ImageMagick_LIBRARIES} ${PROJECT_NAME})    
