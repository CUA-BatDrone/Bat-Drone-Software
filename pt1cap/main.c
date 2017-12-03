#include "pt1.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s <device> <number_of_frames> [filename]\nNote: Lepton records at 8.6 frames per second\n\nExample: %s /dev/video1 86\nCaptures about 10 seconds of video", argv[0], argv[0]);
    return -1;
  }
  char* device_name = argv[1];
  int num_frames = atoi(argv[2]);
  char* filename = argc < 4 ? "capture.bin" : argv[3];
  pt1_init(device_name);
  FILE *fd = fopen(filename, "w");

  pt1_start();
  struct frame frame;
  for (int i = 0; i < num_frames || num_frames == 0; i++) {
    pt1_get_frame(&frame);
    fwrite(frame.start, 1, frame.length, fd);
  }
  pt1_stop();

  fclose(fd);
  pt1_deinit();
}
