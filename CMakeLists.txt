cmake_minimum_required(VERSION 3.0)
project(Bat-Drone-Software)

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
  find_library(SDL2MAIN_LIBRARY SDL2main)
  set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
  set(SDL2_INCLUDE_DIRS)
endif(WIN32)

# Libraries
add_subdirectory(libs)

# Programs
if(UNIX)
  add_subdirectory(pt1cap)
  add_subdirectory(fsw)
endif(UNIX)
add_subdirectory(gse)
