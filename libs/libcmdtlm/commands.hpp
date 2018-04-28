#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "packet_elements.hpp"
#include <stdint.h>
#include <vector>

using namespace std;

class Commands {
public:
  // Old control command, do not use
  virtual void control(const ControlPacketElement &e) {}
  // Control the drone
  virtual void control(float roll, float pitch, float thrust, float yaw) {};
  // Send a single lwir frame
  virtual void lwirFrame(const uint16_t frame[60][80]) {}
  // Send the blob coordinates
  class Blob {
  public:
    int x, y, size;
    Blob(int x, int y, int size = 1) : x(x), y(y), size(size) {}
    Blob() {}
  };
  virtual void blob(uint16_t x, uint16_t y) {}
  virtual void pid(float p1, float i1, float d1, float p2, float i2, float d2, float p3, float i3, float d3, float p4, float i4, float d4) {}
  // Set PID values for tracker
  virtual void blobs(vector<Blob> & blobs) {}
  virtual void manual() {}
  virtual void autonomous() {}
  virtual void track(uint8_t x, uint8_t y) {};
  virtual void threshold(int16_t offset) {};
};

#endif
