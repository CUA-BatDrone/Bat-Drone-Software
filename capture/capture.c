#include "pt1.h"
#include <stdio.h>
#include <stdlib.h>
//   sprintf(out_name, "out%03d.ppm", i);
//   fout = fopen(out_name, "w");
//   if (!fout) {
//     perror("Cannot open image");
//     exit(EXIT_FAILURE);
//   }
//   fprintf(fout, "P6\n%d %d 255\n",
//   fmt.fmt.pix.width, fmt.fmt.pix.height);
//   fwrite(buffers[buf.index].start, buf.bytesused, 1, fout);
//   fclose(fout);


int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s <device> <number_of_frames> [filename]\nNote: Lepton records at 8.6 frames per second\n\nExample: %s /dev/video1 90\nCaptures about 10 seconds of video", argv[0], argv[0]);
    return -1;
  }
  char* device_name = argv[1];
  int num_frames = atoi(argv[2]);
  char* filename = argc < 4 ? "capture.bin" : argv[3];
  pt1_init(device_name);
  FILE *fd = fopen(filename, "w");

  pt1_start();
  struct frame frame;
  for (int i = 0; i < num_frames; i++) {
    pt1_get_frame(&frame);
    fwrite(frame.start, 1, frame.length, fd);
  }
  pt1_stop();

  fclose(fd);
  pt1_deinit();
}
