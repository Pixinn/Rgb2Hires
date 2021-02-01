cmake_minimum_required (VERSION 3.12)
cmake_policy(SET CMP0074 NEW)

project (Rgb2Hires)

# flags not compatibles with magick++
if(WIN32)
	string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	string (REPLACE "/DWIN32" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif(WIN32)

# The version number.
set (Rgb2Hires_VERSION_MAJOR 1)
set (Rgb2Hires_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)     
set(CMAKE_VERBOSE_MAKEFILE ON)

# dependencies
if(WIN32)	
	# imagemagik
	if(NOT DEFINED ENV{MAGICK_HOME})
		message(FATAL_ERROR "Please set a MAGICK_HOME environment variable to the root directory of ImageMagick6")		
	endif()
	find_package(ImageMagick HINTS $ENV{MAGICK_HOME} REQUIRED COMPONENTS Magick++)
else()
	find_package(ImageMagick 6.9 EXACT REQUIRED COMPONENTS Magick++ )
endif(WIN32)

# directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)

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
target_include_directories(${PROJECT_NAME} PRIVATE ${ImageMagick_INCLUDE_DIRS})


# Application Picture
add_executable(Picture src/App_Picture.cpp)
target_compile_definitions(Picture PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(Picture PRIVATE src ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(Picture ${ImageMagick_LIBRARIES} ${PROJECT_NAME})    
set_property(TARGET Picture PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})

# Application Tile
add_executable(Tile src/App_Tile.cpp)
target_compile_definitions(Tile PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(Tile PRIVATE src ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(Tile ${ImageMagick_LIBRARIES} ${PROJECT_NAME})    
set_property(TARGET Tile PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})


# Windows only: copy dlls
if(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES DEBUG)
		string (REPLACE "/" "\\" WIN_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
		add_custom_command(TARGET Picture POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_im_db_dlls.bat ${WIN_BIN_DIR})
		add_custom_command(TARGET Tile POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_im_db_dlls.bat ${WIN_BIN_DIR})
	else()
		string (REPLACE "/" "\\" WIN_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
		add_custom_command(TARGET Picture POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_im_rl_dlls.bat ${WIN_BIN_DIR})
		add_custom_command(TARGET Tile POST_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_im_rl_dlls.bat ${WIN_BIN_DIR})
	endif()
			
endif(WIN32)