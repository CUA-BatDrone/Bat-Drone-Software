#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "packet_elements.hpp"

class Commands {
public:
  virtual void control(const ControlPacketElement &e) {}
  virtual void lwirFrame(const uint16_t frame[60][80]) {}
};

#endif
