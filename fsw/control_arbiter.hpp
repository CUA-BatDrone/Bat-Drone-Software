#ifndef CONTROL_ARBITER_HPP
#define CONTROL_ARBITER_HPP

#include "control.hpp"
#include "drone_controller.hpp"

using namespace std;

class ControlArbiter {
public:
  enum State { AUTONOMOUS, MANUAL };
protected:
  State state;
  DroneController & droneController;
public:
  ControlArbiter(DroneController & droneController);
  void autonomousControl(Control & control);
  void manualControl(Control & control);
  void setState(State state);
};

#endif