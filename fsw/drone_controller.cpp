#include "drone_controller.hpp"

DroneController::DroneController(steady_clock::duration timeout) : timeout(timeout) {}
DroneController::DroneController(steady_clock::duration timeout, const char * device) : pwm(device), timeout(timeout) {}

void DroneController::control(Control & control) {
  unique_lock<mutex> ul(controlMutex);
  pendingControl = control;
  controlIsPending = true;
  controlNotification.notify_all();
}

void DroneController::mainLoop(bool & run) {
  setMotorsOffControls();
  unique_lock<mutex> ul(controlMutex);
  while (!controlIsPending) controlNotification.wait(ul);
  setPendingControls();
  controlIsPending = false;
  while (run) {
    if (failsafeActive) {
      setFailsafeControls();
      while (!controlIsPending) controlNotification.wait(ul);
      failsafeActive = false;
    } else {
      steady_clock::time_point t = steady_clock::now() + timeout;
      while (!controlIsPending) {
        if (controlNotification.wait_until(ul, t) == cv_status::timeout) {
          failsafeActive = true;
          break;
        } else {
          setPendingControls();
          controlIsPending = false;
        }
      }
    }
  }
  setMotorsOffControls();
}



void DroneController::setFailsafeControls() {
  // AETR
  pwm.setPosition(4, 0.0f);
  pwm.setPosition(5, 0.0f);
  pwm.setPosition(6, 0.0f);
  pwm.setPosition(7, 0.0f);
}
void DroneController::setMotorsOffControls() {
  // AETR
  pwm.setPosition(4, 0.0f);
  pwm.setPosition(5, 0.0f);
  pwm.setPosition(6, -1.0f);
  pwm.setPosition(7, 0.0f);
}
void DroneController::setPendingControls() {
  // AETR
  pwm.setPosition(4, pendingControl.aileron);
  pwm.setPosition(5, pendingControl.elevator);
  pwm.setPosition(6, pendingControl.thrust);
  pwm.setPosition(7, pendingControl.rudder);
}