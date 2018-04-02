cmake_minimum_required(VERSION 3.0)

project(Ciaran's-Library)

# Compile library ciaran using ciaran.cpp or ciaran.c
add_library(ciaran ciaran)

# include headers from the current directory '.' for the library. Directories listed after PUBLIC will be included by those using the library as well. Directories listad after PRIVATE will only be used by the library itself.
target_include_directories(cmdtlm PUBLIC .)
