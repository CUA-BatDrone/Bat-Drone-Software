# Bat-Drone-Software

Contains all software needed for the Bat Drone project. Each directory has a README.md describing its function.

# Compiling each project

## Install Build Dependencies

### For Fedora

`sudo dnf install -y git cmake libv4l`

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

Enter the directory for your project. e.g. pt1cap

`cd pt1cap`

## Building

`make`

# Running

`./pt1cap /dev/video1 86 capture$(date +%s).bin`

Captures about 10 seconds to a file named capture and the current unix timestamp and .bin e.g. capture1510084031.bin
