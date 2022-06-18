cmake_minimum_required (VERSION 3.19)
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

# Conan init
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)

# Library
add_library(${PROJECT_NAME}
							src/HiRes.cpp
							src/HiRes.h
							src/ImageQuantized.cpp
							src/ImageQuantized.h
)
## dependencies
conan_set_find_library_paths(${PROJECT_NAME})
conan_target_link_libraries(${PROJECT_NAME})


# Application Picture
add_executable(	Picture src/App_Picture.cpp
							src/Display.h
							src/Display.cpp
)
if(NOT WIN32)
		set_target_properties(Picture PROPERTIES COMPILE_FLAGS -pthread LINK_FLAGS -pthread)
endif()
## dependencies
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "9")
	target_link_libraries(Picture stdc++fs) # filesystem lib not included in stdc++ for gcc < 9
endif()
conan_set_find_library_paths(Picture)
conan_target_link_libraries(Picture)
target_link_libraries(Picture ${PROJECT_NAME})
## output
set_property(TARGET Picture PROPERTY
						RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})


# Application Tile
add_executable(Tile src/App_Tile.cpp)
## custom definitions
target_compile_definitions(Tile PRIVATE cimg_use_png)
## dependencies
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "9")
	target_link_libraries(Tile stdc++fs) # filesystem lib not included in stdc++ for gcc < 9
endif()
conan_set_find_library_paths(Tile)
conan_target_link_libraries(Tile)
target_link_libraries(Tile ${PROJECT_NAME})
## output
set_property(TARGET Tile PROPERTY
						RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
)


