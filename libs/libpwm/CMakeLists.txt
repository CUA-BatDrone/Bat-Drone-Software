# Required by CMake
cmake_minimum_required(VERSION 3.0)

# Optional project name
project(libpwm)

# Compile library pt1 using pt1.c, pt1.cpp, pt1.cc etc.
add_library(pwm pwm)

# include headers from the current directory '.' for the pt1 library. Directories listed after PUBLIC will be included by those using the library as well. Directories listad after PRIVATE will only be used by the library itself.
target_include_directories(pwm PUBLIC .)
