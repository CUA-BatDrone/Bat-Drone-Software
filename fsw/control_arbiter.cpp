#include "control_arbiter.hpp"

ControlArbiter::ControlArbiter(DroneController & droneController) : droneController(droneController), state(State::MANUAL) {}

void ControlArbiter::autonomousControl(Control & control) {
  if (state == AUTONOMOUS) {
    droneController.control(control);
  }
}

void ControlArbiter::manualControl(Control & control) {
  if (state == MANUAL) {
    droneController.control(control);
  }
}

void ControlArbiter::setState(State state) {
  ControlArbiter::state = state;
}
