#ifndef CONTROL_ARBITER_HPP
#define CONTROL_ARBITER_HPP

class ControlArbiter {
public:
  void autonomousControl(Control);
  void manualControl(Control);
};

#endif