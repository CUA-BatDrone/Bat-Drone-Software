# Required by CMake
cmake_minimum_required(VERSION 3.0)

# Optional project name
project(libpt1)

# Compile library pt1 using pt1.c, pt1.cpp, pt1.cc etc.
add_library(pt1 pt1)

# include headers from the current directory '.' for the pt1 library. Directories listed after PUBLIC will be included by those using the library as well. Directories listad after PRIVATE will only be used by the library itself.
target_include_directories(pt1 PUBLIC .)

# link pt1 library with v4l2 library. Libraries listed after PUBLIC will also be linked by those using the library as well. Libraries listad after PRIVATE will only be linked by the library itself.
target_link_libraries(pt1 PRIVATE v4l2)
