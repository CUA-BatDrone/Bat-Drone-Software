#include "receiver.hpp"
#include "commands.hpp"

Receiver::Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter) : cmdtlm(cmdtlm), controlArbiter(controlArbiter) {}

void Receiver::mainLoop(bool & run) {
  class Listener : public Commands {
  protected:
    ControlArbiter & controlArbiter;
  public:
    // Control the drone
    virtual void control(float aileron, float elevator, float thrust, float rudder) {
      Control c;
      c.aileron = aileron;
      c.elevator = elevator;
      c.thrust = thrust;
      c.rudder = rudder;
      controlArbiter.manualControl(c);
    }
    // Switch to remote control mode
    virtual void manual() {
      controlArbiter.setState(ControlArbiter::State::MANUAL);
    }
    // Switch to tracking mode
    virtual void autonomous() {
      controlArbiter.setState(ControlArbiter::State::AUTONOMOUS);
    }
    Listener(ControlArbiter & controlArbiter) : controlArbiter(controlArbiter) {}
  } listener (controlArbiter);
  while (run) {
    cmdtlm.telemetry(listener);
  }
}
