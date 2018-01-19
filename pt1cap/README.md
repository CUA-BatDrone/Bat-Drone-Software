
# Description

Writes frames captured from the Lepton camera using the Pure Thermal 1 board to a file in binary format.

# Usage

`pt1cap <device> [filename] [number_of_frames]``
device is the path of the pure thermal 1 video device. Typically /dev/video0 or /dev/video1... usually /dev/video1 .
filename defaults to the current unix timestamp with a bin suffix. e.g. 1510084031.bin
number_of_frames defaults to 0.
**Note:** Lepton records at 8.6 frames per second

# Examples
`pt1cap /dev/video1`
Captures frames to a file named the current unix timestamp until Ctrl+C is pressed.

`pt1cap /dev/video1 capture.bin`
Captures frames to capture.bin until user presses Ctrl+C

`pt1cap /dev/video1 capture.bin 86`
Captures frames to capture.bin for about 10 seconds or until user presses Ctrl+C


# File Format

Each frame is stored one after the other in sequence. A frame is 80x60 pixels. Each pixel is stored as a 16 bit unsigned integer with a maximum value of 0x3FFF (14 bits).
