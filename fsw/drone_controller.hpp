#ifndef DRONE_CONTROLLER_HPP
#define DRONE_CONTROLLER_HPP

#include <mutex>
#include <chrono>
#include <condition_variable>
#include <thread>

#include <pwm.hpp>

#include "control.hpp"

using namespace std;
using namespace chrono;

class DroneController {
protected:
  bool failsafeActive;
  mutex controlMutex;
  condition_variable controlNotification;
  bool controlIsPending;
  Control pendingControl;
  PWMDevice pwm;
  steady_clock::duration timeout;
  void setFailsafeControls();
  void setMotorsOffControls();
  void setPendingControls();
public:
  DroneController(steady_clock::duration timeout = seconds(2));
  DroneController(steady_clock::duration timeout, const char * device);
  void control(Control & c);
  void mainLoop(bool & run);
};

#endif
