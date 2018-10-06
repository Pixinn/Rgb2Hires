cmake_minimum_required (VERSION 2.6)
project (Rgb2Hires)

# The version number.
set (Rgb2Hires_VERSION_MAJOR 1)
set (Rgb2Hires_VERSION_MINOR 0)

#Warning level
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()


add_executable(${PROJECT_NAME} src/Common.h
                                src/HiRes.cpp
                                src/HiRes.h
                                src/ImageQuantized.cpp
                                src/ImageQuantized.h
                                src/Main.cpp
)
                                