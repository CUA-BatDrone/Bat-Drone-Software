# Tracking-Algorithm

# Compiling

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

## Building

`make`

# Running

From the build directory

`capture/capture /dev/video1 86 $(date +%s).bin`
