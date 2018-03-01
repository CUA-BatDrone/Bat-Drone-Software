#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "packet_elements.hpp"

class Commands {
  virtual void control(ControlPacketElement *e) {};
  virtual void lwirFrame(SWIRFrame *frame) {};
  virtual void swirFrame(SWIRFrame *frame) {};
};

#endif
