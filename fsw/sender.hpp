#ifndef SENDER_HPP
#define SENDER_HPP

#include <stdint.h>
#include <cmd_tlm.hpp>
#include <mutex>

#include "frame.hpp"
#include "control.hpp"

class Sender {
protected:
  mutex m;
  CmdTlm & cmdtlm;
public:
  Sender(CmdTlm & cmdtlm);
  void sendFrame(uint16_t frame[60][80]);
  void sendAutonomyBlob(int x, int y);
  void sendAutonomyControl(Control control);
};

#endif