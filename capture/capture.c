#include "pt1.h"
#include <stdio.h>
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
  if (argc < 2) {
    printf("Usage: %s <device>\n", argv[0]);
    return -1;
  }
  pt1_init(argv[1]);
  FILE *fd = fopen("capture.bin", "w");

  pt1_start();
  struct frame frame;
  for (int i = 0; i < 9 * 10; i++) {
    pt1_get_frame(&frame);
    fwrite(frame.start, 1, frame.length, fd);
  }
  pt1_stop();

  fclose(fd);
  pt1_deinit();
}
