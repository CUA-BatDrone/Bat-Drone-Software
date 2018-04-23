#ifndef SENDER_HPP
#define SENDER_HPP

#include <cmd_tlm.hpp>
#include <mutex>

#include "frame.hpp"
#include "control.hpp"

class Sender {
protected:
  mutex sendMutex;
  CmdTlm & cmdtlm;
public:
  Sender(CmdTlm & cmdtlm);
  void sendFrame(Frame frame);
  void sendControl(Control control);
};

#endif