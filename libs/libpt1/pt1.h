#ifndef PT1_H
#define PT1_H


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  struct pt1_timeval {
    long tv_sec;
    long tv_usec;
  };
#else
  #include <sys/time.h>
  #include <sys/types.h>
  #define pt1_timeval timeval
#endif
#include <stdint.h>

#define PT1_WIDTH 80
#define PT1_HEIGHT 60
#define PT1_PMAX 0x3FFF

/**
 * Initialize the camera.
 */
int pt1_init(const char *device);

/**
 * Run FFC.
 */
int pt1_perform_ffc();

/**
 * Disable FFC.
 */
int pt1_disable_ffc();

/**
 * Enable video capture.
 */
int pt1_start();

/**
 * Disable video capture.
 */
int pt1_stop();

/**
 * Contains the buffer, sequence, and timing of a frame.
 */
struct pt1_frame {
  void *start;
  size_t length;
  struct pt1_timeval timestamp;
  long sequence;
};

/**
 * Gets a new frame. Previous frame becomes invalid
 */
int pt1_get_frame(struct pt1_frame *frame);

/**
 * Deinitialize the camera.
 */
int pt1_deinit();

#ifdef __cplusplus
}
#endif

#endif
