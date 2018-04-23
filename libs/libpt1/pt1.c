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
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

int fd;
int n_buffers;
// Currently read buffer
struct v4l2_buffer buf;

/**
 * Holds the address and length of a video buffer.
 */
struct buffer {
  void *start;
  size_t length;
} buffers[3];

static int xioctl(int fh, int request, void *arg) {
  int r;

  do {
    r = v4l2_ioctl(fh, request, arg);
  } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

  if (r == -1) {
    fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
    return -1;
  }
  return 0;
}

int pt1_perform_ffc() {
  __u8 a = 0;
  struct uvc_xu_control_query q = {
      .unit = 5, .selector = 12, .query = UVC_SET_CUR, .size = 1, .data = &a};
  if (xioctl(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;
  return 0;
}

int pt1_disable_ffc() {
  __u8 a[32];
  struct uvc_xu_control_query q = {
      .unit = 6, .selector = 16, .query = UVC_GET_CUR, .size = 32, .data = a};
  if (xioctl(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;
  a[0] = 0;
  q.query = UVC_SET_CUR;
  if (xioctl(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;
  return 0;
}

int pt1_init(const char *device) {
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  unsigned int i;

  /* Open camera */
  fd = v4l2_open(device, O_RDWR, 0);
  if (fd < 0) {
    perror("Cannot open device");
    return -1;
  }
  /* Set format */
  CLEAR(fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = 80;
  fmt.fmt.pix.height = 60;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_Y16;
  fmt.fmt.pix.field = V4L2_FIELD_NONE;
  if (xioctl(fd, VIDIOC_S_FMT, &fmt) < 0) return -1;
  if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_Y16) {
    puts("Pixel format error");
    return -1;
  }
  if ((fmt.fmt.pix.width != 80) || (fmt.fmt.pix.height != 60)) {
    printf("Image size error. Expected 80x60 but received %dx%d\n",
           fmt.fmt.pix.width, fmt.fmt.pix.height);
    return -1;
  }

  /* Request buffers */
  CLEAR(req);
  req.count = 3;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if (xioctl(fd, VIDIOC_REQBUFS, &req) < 0) return -1;
  if (req.count > 3) {
    perror("Too many buffers");
    return -1;
  }

  /* Save buffer addresses */
  // buffers = calloc(req.count, sizeof(*buffers));
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = n_buffers;
    if (xioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) return -1;
    buffers[n_buffers].start = v4l2_mmap(
        NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

    if (MAP_FAILED == buffers[n_buffers].start) {
      perror("mmap");
      return -1;
    }
    return 0;
  }

  /* Queue the buffers */
  for (i = 0; i < n_buffers - 1; ++i) {
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (xioctl(fd, VIDIOC_QBUF, &buf) < 0) return -1;
  }
  /* Don't queue the last buffer. Will be queued in pt1_get_frame() */
  CLEAR(buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = i;
  return 0;
}

int pt1_start() {
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(fd, VIDIOC_STREAMON, &type) < 0) return -1;
  return 0;
}

int pt1_stop() {
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(fd, VIDIOC_STREAMOFF, &type) < 0) return -1;
  return 0;
}

int pt1_get_frame(struct pt1_frame *frame) {
  /* queue last buffer */
  if (xioctl(fd, VIDIOC_QBUF, &buf) < 0) return -1;

  /* dequeue next available buffer */
  CLEAR(buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (xioctl(fd, VIDIOC_DQBUF, &buf) < 0) return -1;

  /* return data */
  frame->start = buffers[buf.index].start;
  frame->length = buf.bytesused;
  frame->timestamp = buf.timestamp;
  frame->sequence = buf.sequence;
  return 0;
}

int pt1_deinit() {
  for (int i = 0; i < n_buffers; ++i)
    v4l2_munmap(buffers[i].start, buffers[i].length);
  v4l2_close(fd);
  return 0;
}
