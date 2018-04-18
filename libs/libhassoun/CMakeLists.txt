cmake_minimum_required(VERSION 3.0)

project(Michael-Hassoun's-Library)

# Compile library ciaran using ciaran.cpp or ciaran.c
add_library(hassoun hassoun)

# include headers from the current directory '.' for the library. Directories listed after PUBLIC will be included by those using the library as well. Directories listad after PRIVATE will only be used by the library itself.
target_link_library(hassoun ciaran)
target_include_directories(hassoun PUBLIC .)
