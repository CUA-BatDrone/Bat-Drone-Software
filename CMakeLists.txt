cmake_minimum_required(VERSION 3.0)
project(thermal-tracker)

# Libraries
add_subdirectory(pt1)

# Programs
add_subdirectory(capture)
add_subdirectory(fsw)
add_subdirectory(gse)
