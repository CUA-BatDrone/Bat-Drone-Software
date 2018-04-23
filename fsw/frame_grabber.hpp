#ifndef FRAME_GRABBER_HPP
#define FRAME_GRABBER_HPP

class FrameGrabber {
protected:
  Autonomy * autonomy;
  Sender * sender;
  const char * device;
public:
  FrameGrabber(bool & run, Autonomy * autonomy, Sender * sender, const char * device = "/dev/video0");
  virtual ~FrameGrabber();
  void mainLoop();
};

#endif