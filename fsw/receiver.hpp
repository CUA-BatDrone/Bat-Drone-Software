#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <cmd_tlm.hpp>
#include <commands.hpp>
#include "control_arbiter.hpp"
#include "autonomy2.hpp"

class Receiver {
protected:
  ControlArbiter & controlArbiter;
  CmdTlm & cmdtlm;
  Autonomy & autonomy;
public:
  Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter, Autonomy & autonomy);
  void mainLoop(bool & run);
};

#endif