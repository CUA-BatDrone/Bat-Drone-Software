# Required by CMake
cmake_minimum_required(VERSION 3.0)

# Optional project name
project(command-telemetry)

# Compile library pt1 using cmd_tlm.cpp
add_library(cmdtlm cmd_tlm packet_elements packet_accessor)

# include headers from the current directory '.' for the pt1 library. Directories listed after PUBLIC will be included by those using the library as well. Directories listad after PRIVATE will only be used by the library itself.
target_include_directories(cmdtlm PUBLIC .)
