#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <cmd_tlm.hpp>
#include "control_arbiter.hpp"

class Receiver {
protected:
  const ControlArbiter & controlArbiter;
  const CmdTlm & cmdtlm;
public:
  Receiver(CmdTlm & cmdtlm, ControlArbiter & controlArbiter);
  void mainLoop(bool & run);
};

#endif