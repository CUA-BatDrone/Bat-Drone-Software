#include "receiver.hpp"
#include "commands.hpp"

Receiver::Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter, Autonomy & autonomy) : cmdtlm(cmdtlm), controlArbiter(controlArbiter), autonomy(autonomy) {}

void Receiver::mainLoop(bool & run) {
  class Listener : public Commands {
  protected:
    ControlArbiter & controlArbiter;
    Autonomy & autonomy;
  public:
    // Control the drone
    virtual void control(float aileron, float elevator, float thrust, float rudder) {
      Control c;
      c.aileron = aileron;
      c.elevator = elevator;
      c.thrust = thrust;
      c.rudder = rudder;
      controlArbiter.manualControl(c);
      autonomy.giveControl(c);
    }
    // Switch to remote control mode
    virtual void manual() {
      controlArbiter.setState(ControlArbiter::State::MANUAL);
    }
    // Switch to tracking mode
    virtual void autonomous() {
      //controlArbiter.setState(ControlArbiter::State::AUTONOMOUS);
    }
    virtual void track(uint8_t x, uint8_t y) {
      autonomy.giveTarget(x, y);
    }
    Listener(ControlArbiter & controlArbiter, Autonomy & autonomy) : controlArbiter(controlArbiter), autonomy(autonomy) {}
  } listener (controlArbiter, autonomy);
  while (run) {
    cmdtlm.telemetry(listener);
  }
}
