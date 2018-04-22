
# Bat-Drone-Software

Contains all software needed for the Bat Drone project.

Each component has its own directory containing a README.md documenting its function.

# Compiling Each project

## Install Build Dependencies

Currently the dependencies for all projects are:
* cmake
* libv4l
* libsdl2

### Installing Dependencies For Fedora

`sudo dnf groupinstall -y "Development Tools" && sudo dnf install -y git cmake libv4l-devel`

### Installing Dependencies For Ubuntu

`sudo apt install -y git cmake libv4l-dev`

### Installing Dependencies for Windows

No action needed. Library included.

## Set up environment

Clone repository.

`git clone https://github.com/CUA-BatDrone/Tracking-Algorithm`

Enter the source directory.

`cd Tracking-Algorithm`

Create and enter a build directory

`mkdir build`

`cd build`

Generate a makefile

`cmake ..`

Enter the directory for your project if you wish to compile only one project. e.g. pt1cap

`cd pt1cap`

## Building

`make`

# Running

`./pt1cap /dev/video1 86 capture$(date +%s).bin`

Captures about 10 seconds to a file named capture and the current unix time stamp and .bin e.g. capture1510084031.bin
