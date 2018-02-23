cmake_minimum_required(VERSION 3.0)
project(ground-station-equipment)

include_directories(${SDL2_INCLUDE_DIRS})
add_executable(gse main.cpp)
target_link_libraries(gse ${SDL2_LIBRARIES})

# Copy over SDL2.dll if on windows
if(WIN32)
  math(EXPR BITS "${CMAKE_SIZEOF_VOID_P} * 8")
  configure_file(${CMAKE_SOURCE_DIR}/windows/SDL2-2.0.7/lib/x${BITS}/SDL2.dll SDL2.dll COPYONLY)
endif(WIN32)
