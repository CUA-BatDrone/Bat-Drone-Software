#include "pt1.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int run = 1;

void handle_sigint(int signum) {
  run = 0;
}

int main(int argc, char* argv[]) {
  if (argc > 4 || argc < 2) {
    printf(
      "Captures frames to filename until Ctrl+C is pressed.\n"\
      "\n"\
      "Usage: %s <device> [filename] [number_of_frames]\n"\
      "\tdevice is the path of the pure thermal 1 video device. Usually /dev/video1 .\n"\
      "\tfilename defaults to the current unix timestamp with a bin suffix. \n"\
      "\te.g. 1510084031.bin\n"\
      "\tnumber_of_frames defaults to 0.\n"\
      "\tNote: Lepton records at 8.6 frames per second\n"\
      "\n"\
      "Example: %s /dev/video1\n"\
      "\tCaptures frames to a file named whatever the current unix timestamp is with\n"\
      "\ta .bin suffix until Ctrl+C is pressed.\n"\
      "\n"\
      "Example: %s /dev/video1 capture.bin\n"\
      "\tCaptures frames to capture.bin until user presses Ctrl+C\n"\
      "\n"\
      "Example: %s /dev/video1 capture.bin 86\n"\
      "\tCaptures frames to capture.bin for about 10 seconds or until user presses"\
      "\tCtrl+C\n"\
      "\n"\
    , argv[0], argv[0], argv[0], argv[0]);
    return -1;
  }

  sigaction(SIGINT, &((struct sigaction) {.sa_handler = &handle_sigint}), NULL);

  char* device_name = argv[1];
  char* filename;
  int num_frames;

  if (argc > 2) {
    filename = argv[2];
  } else {
    filename = (char*) ((char[24]) {});
    sprintf(filename, "%llu.bin", (long long unsigned int) time(NULL));
  }
  if (argc > 3) {
    num_frames = atoi(argv[3]);
  } else {
    num_frames = 0;
  }

  printf("Writing to %s\n", filename);

  pt1_init(device_name);
  pt1_disable_ffc();

  FILE *fd = fopen(filename, "w");
  pt1_start();
  struct frame frame;
  int count;
  for (count = 0; count < num_frames || num_frames == 0 && run; count++) {
    pt1_get_frame(&frame);
    fwrite(frame.start, 1, frame.length, fd);
  }
  printf("Wrote %d frames\n", count);
  pt1_stop();

  fclose(fd);
  pt1_deinit();
}
