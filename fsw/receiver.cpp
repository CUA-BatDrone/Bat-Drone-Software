#include "receiver.hpp"
#include "commands.hpp"


Receiver::Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter) : cmdtlm(cmdtlm), listener(controlArbiter) {}

Receiver::Listener::Listener(ControlArbiter & controlArbiter) : controlArbiter(controlArbiter) {}

void Receiver::Listener::control(float aileron, float elevator, float thrust, float rudder) {
  controlArbiter.setState(ControlArbiter::State::MANUAL);
  Control c;
  c.aileron = aileron;
  c.elevator = elevator;
  c.thrust = thrust;
  c.rudder = rudder;
  controlArbiter.manualControl(c);
}

void Receiver::Listener::track() {
  controlArbiter.setState(ControlArbiter::State::AUTONOMOUS);
}
void Receiver::mainLoop(bool & run) {
  while (run) {
    cmdtlm.telemetry(listener);
  }
}