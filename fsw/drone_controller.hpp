#ifndef DRONE_CONTROLLER_HPP
#define DRONE_CONTROLLER_HPP

#include "control.hpp"

class DroneController {
protected:
  const char * device;
public:
  DroneController(const char * device = "/dev/i2c-1");
  void control(Control c);
};

#endif
