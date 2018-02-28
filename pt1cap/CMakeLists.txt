# Optional, adds a name for the project that can be referenced later.
project(PT1Capture)

# Create an executable called pt1cap from main.c or main.cpp or main.cc etc.
add_executable(pt1cap main)

# Link with and add include headers from the pt1 library
target_link_libraries(pt1cap pt1)


add_executable(pt1play viewer.c)
target_include_directories(pt1play PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(pt1play ${SDL2_LIBRARIES})
