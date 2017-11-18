#include "pt1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <libv4l2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

int fd;
int n_buffers;
struct v4l2_buffer buf;
struct buffer *buffers;

static void xioctl(int fh, int request, void *arg) {
  int r;

  do {
    r = v4l2_ioctl(fh, request, arg);
  } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

  if (r == -1) {
    fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void pt1_init(char* device) {
  struct v4l2_format              fmt;
  struct v4l2_requestbuffers      req;
  unsigned int                    i;

  /* Open camera */
  fd = v4l2_open(device, O_RDWR, 0);
  if (fd < 0) {
    perror("Cannot open device");
    exit(EXIT_FAILURE);
  }
  /* Set format */
  CLEAR(fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = 80;
  fmt.fmt.pix.height      = 60;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_Y16;
  fmt.fmt.pix.field       = V4L2_FIELD_NONE;
  xioctl(fd, VIDIOC_S_FMT, &fmt);
  if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_Y16) {
    puts("Pixel format error");
    exit(EXIT_FAILURE);
  }
  if ((fmt.fmt.pix.width != 80) || (fmt.fmt.pix.height != 60)) {
    printf("Image size error. Expected 80x60 but received %dx%d\n",
    fmt.fmt.pix.width, fmt.fmt.pix.height);
    exit(EXIT_FAILURE);
  }

  /* Request buffers */
  CLEAR(req);
  req.count = 2;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  xioctl(fd, VIDIOC_REQBUFS, &req);

  /* Save buffer addresses */
  buffers = calloc(req.count, sizeof(*buffers));
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    CLEAR(buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;
    xioctl(fd, VIDIOC_QUERYBUF, &buf);
    buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
      PROT_READ | PROT_WRITE, MAP_SHARED,
      fd, buf.m.offset);

      if (MAP_FAILED == buffers[n_buffers].start) {
        perror("mmap");
        exit(EXIT_FAILURE);
      }
    }

    for (i = 0; i < n_buffers -1; ++i) {
      CLEAR(buf);
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;
      buf.index = i;
      xioctl(fd, VIDIOC_QBUF, &buf);
    }
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

}

void pt1_start() {
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  xioctl(fd, VIDIOC_STREAMON, &type);
}

void pt1_stop() {
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  xioctl(fd, VIDIOC_STREAMOFF, &type);
}

void pt1_get_frame(struct frame *frame) {
  /* queue last buffer */
  xioctl(fd, VIDIOC_QBUF, &buf);

  /* dequeue next available buffer */
  CLEAR(buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  xioctl(fd, VIDIOC_DQBUF, &buf);

  /* return data */
  frame->start = buffers[buf.index].start;
  frame->length = buf.bytesused;
  frame->timestamp = buf.timestamp;
  frame->sequence = buf.sequence;
}

void pt1_deinit() {
  for (int i = 0; i < n_buffers; ++i)
    v4l2_munmap(buffers[i].start, buffers[i].length);
  v4l2_close(fd);
}