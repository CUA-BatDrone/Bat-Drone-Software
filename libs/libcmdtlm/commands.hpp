#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "packet_elements.hpp"
#include <stdint.h>

class Commands {
public:
  // Old control command, do not use
  virtual void control(const ControlPacketElement &e) {}
  // Control the drone
  virtual void control(float roll, float pitch, float thrust, float yaw) {};
  // Send a single lwir frame
  virtual void lwirFrame(const uint16_t frame[60][80]) {}
  // Send the blob coordinates
  virtual void blob(uint16_t x, uint16_t y) {}
  // Switch to tracking mode
  virtual void track() {}
};

#endif
