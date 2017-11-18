cmake_minimum_required(VERSION 3.0)
project(thermal-tracker)

# Libraries
add_subdirectory(libs)

# Programs
add_subdirectory(pt1cap)
add_subdirectory(fsw)
add_subdirectory(gse)
