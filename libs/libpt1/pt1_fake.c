#include "pt1.h"
#include "stdio.h"
#include <stdlib.h>

uint16_t buffer[PT1_HEIGHT][PT1_WIDTH];

int pt1_perform_ffc() {
  printf("pt1_perform_ffc()\n");
  return 0;
}

int pt1_disable_ffc() {
  printf("pt1_disable_ffc()\n");
  return 0;
}

int pt1_init(const char *device) {
  printf("pt1_init(%s)\n", device);
  return 0;
}

int pt1_start() {
  printf("pt1_start()\n");
  return 0;
}

int pt1_stop() {
  printf("pt1_stop()\n");
  return 0;
}

int pt1_get_frame(struct pt1_frame *frame) {
  //printf("pt1_get_frame()\n");
  for (int i = 0; i < PT1_HEIGHT; i++) {
    for (int j = 0; j < PT1_WIDTH; j++) {
      buffer[i][j] = rand();
    }
  }
  frame->start = buffer;
  return 0;
}

int pt1_deinit() {
  printf("pt1_deinit()\n");
  return 0;
}
