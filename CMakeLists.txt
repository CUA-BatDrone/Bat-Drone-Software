cmake_minimum_required(VERSION 3.0)
project(Bat-Drone-Software)

set(CMAKE_CXX_STANDARD 11)

# SDL config
if(WIN32)
  # include_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows/SDL2-2.0.7/include)
  # link_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows/SDL2-2.0.7/lib/x64)
  set(SDL2_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/windows/SDL2-2.0.7/lib/x64/SDL2.lib ${CMAKE_CURRENT_SOURCE_DIR}/windows/SDL2-2.0.7/lib/x64/SDL2main.lib)
  set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/windows/SDL2-2.0.7/include)
  # LIST(APPEND CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/windows/SDL2-2.0.7/lib/x64")
  # LIST(APPEND CMAKE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/windows/SDL2-2.0.7/include")
else()
  find_library(SDL2_LIBRARY SDL2)
  set(SDL2_LIBRARIES ${SDL2_LIBRARY})
  find_path(SDL2_INCLUDE_DIRS SDL.h PATH_SUFFIXES SDL2)
endif(WIN32)

# Libraries
add_subdirectory(libs)

# Programs
if(UNIX)
  add_subdirectory(fsw)
endif(UNIX)
add_subdirectory(pt1cap)
add_subdirectory(gse)
