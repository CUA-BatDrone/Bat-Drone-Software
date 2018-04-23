#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <cmd_tlm.hpp>
#include <commands.hpp>
#include "control_arbiter.hpp"

class Receiver {
protected:
  class Listener : public Commands {
  protected:
    ControlArbiter & controlArbiter;
    // Control the drone
    virtual void control(float roll, float pitch, float thrust, float yaw);
    // Switch to tracking mode
    virtual void track();
  public:
    Listener(ControlArbiter & controlArbiter);
  } listener;
  CmdTlm & cmdtlm;
public:
  Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter);
  void mainLoop(bool & run);
};

#endif