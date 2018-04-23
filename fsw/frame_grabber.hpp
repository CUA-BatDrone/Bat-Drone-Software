#ifndef FRAME_GRABBER_HPP
#define FRAME_GRABBER_HPP

#include "autonomy2.hpp"
#include "sender.hpp"

class FrameGrabber {
protected:
  Autonomy & autonomy;
  Sender & sender;
  const char * device;
public:
  FrameGrabber(Autonomy2 & autonomy, Sender & sender, const char * device = "/dev/video0");
  void mainLoop(bool & run);
};

#endif