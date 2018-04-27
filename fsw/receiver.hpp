#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <cmd_tlm.hpp>
#include <commands.hpp>
#include "control_arbiter.hpp"

class Receiver {
protected:
  ControlArbiter & controlArbiter;
  CmdTlm & cmdtlm;
public:
  Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter);
  void mainLoop(bool & run);
};

#endif