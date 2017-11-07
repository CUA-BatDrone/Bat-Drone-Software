#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define PT1_WIDTH = 80;
#define PT1_HEIGHT = 60;
#define PT1_PMAX = 0x3FFF
typedef uint16_t pt1p_t;

/**
 * Initialize the camera.
 */
void pt1_init();

/**
 * Enable video capture.
 */
 void pt1_start();

 /**
  * Disable video capture.
  */
  void pt1_stop();

/**
 * Holds the address and length of a video buffer.
 */
struct buffer {
  void *start;
  size_t length;
};

/**
 * Contains the buffer, sequence, and timing of a frame.
 */
struct frame {
  void *start;
  size_t length;
  struct timeval timestamp;
	long sequence;
};

/**
 *
 */
void pt1_get_frame(struct frame *frame);

/**
 * Deinitialize the camera.
 */
void pt1_deinit();

#ifdef __cplusplus
}
#endif
