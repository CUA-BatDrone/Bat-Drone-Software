
# Description

Writes a number of frames captured from the Lepton camera using the Pure Thermal 1 board to a file in binary format.

# Usage

`pt1cap <device> <number_of_frames> [filename]`

The default filename is `capture.bin`

# Example 

Saves about 10 seconds of video to capture.bin . The Lepton camera records at 8.6 frames per second.

`pt1cap /dev/video1 86 capture.bin`


Captures about 10 seconds to a file named "capture" + unix timestamp + ".bin". e.g. cap1510084031.bin

`pt1cap /dev/video1 86 cap$(date +%s).bin`


# File Format

Each frame is stored one after the other in sequence. A frame is 80x60 pixels. Each pixel is stored as a 16 bit unsigned integer with a maximum value of 0x3FFF (14 bits). 
