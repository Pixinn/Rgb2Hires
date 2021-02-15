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
set(CMAKE_CXX_STANDARD 17)     # required to use <filesystem>
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)


# dependencies
if(WIN32)	
	# sdl2
	if(NOT DEFINED ENV{SDL2_HOME})
		message(FATAL_ERROR "Please set a SDL2_HOME environment variable to the root directory of SDL2")		
	endif()
	set(sdl2_DIR $ENV{SDL2_HOME})
	# imagemagik
	if(NOT DEFINED ENV{MAGICK_HOME})
		message(FATAL_ERROR "Please set a MAGICK_HOME environment variable to the root directory of ImageMagick6")		
	endif()
	find_package(ImageMagick HINTS $ENV{MAGICK_HOME} REQUIRED COMPONENTS Magick++)
else()
	find_package(ImageMagick REQUIRED COMPONENTS Magick++ )
endif(WIN32)
find_package(sdl2 REQUIRED)
find_package(Threads REQUIRED)

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
							src/Display.h
							src/Display.cpp
)
target_compile_definitions(${PROJECT_NAME} PRIVATE MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(${PROJECT_NAME} PRIVATE ${ImageMagick_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})


# Application Picture
add_executable(Picture src/App_Picture.cpp)
if(NOT WIN32)
		set_target_properties(Picture PROPERTIES COMPILE_FLAGS -pthread LINK_FLAGS -pthread)
endif()
target_compile_definitions(Picture PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(Picture PRIVATE src ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(Picture ${ImageMagick_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME} ${SDL2_LIBRARIES})  # SDL2 must be at the end
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "9")
	target_link_libraries(Picture stdc++fs) # filesystem lib not included in stdc++ for gcc < 9
endif()
set_property(TARGET Picture PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})

# Application Tile
add_executable(Tile src/App_Tile.cpp)
target_compile_definitions(Tile PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(Tile PRIVATE src ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(Tile ${ImageMagick_LIBRARIES} ${PROJECT_NAME})    
set_property(TARGET Tile PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})


# Windows only: copy dlls
if(WIN32)
		set(DEBUG_EXE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_im_db_dlls.bat")
		set(RELEASE_EXE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy_im_rl_dlls.bat")
		string (REPLACE "/" "\\" WIN_BIN_DIR "$<$<CONFIG:debug>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}>$<$<CONFIG:release>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>")
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND "$<$<CONFIG:debug>:${DEBUG_EXE_PATH}>$<$<CONFIG:release>:${RELEASE_EXE_PATH}>" ${WIN_BIN_DIR})			
endif(WIN32)
