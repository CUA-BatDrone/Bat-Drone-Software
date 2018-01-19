#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define PT1_WIDTH = 80;
#define PT1_HEIGHT = 60;
#define PT1_PMAX = 0x3FFF

/**
 * Initialize the camera.
 */
void pt1_init();

/**
 * Run FFC.
 */
void pt1_perform_ffc();

/**
 * Disable FFC.
 */
void pt1_disable_ffc();


/**
 * Enable video capture.
 */
 void pt1_start();

 /**
  * Disable video capture.
  */
  void pt1_stop();

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
