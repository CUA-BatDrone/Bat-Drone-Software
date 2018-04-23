#include "sender.hpp"

Sender::Sender(CmdTlm & cmdtlm) : cmdtlm(cmdtlm) {}

void Sender::sendFrame(uint16_t frame[60][80]) {
  unique_lock<mutex> ul(m);
  cmdtlm.lwirFrame(frame);
}

void Sender::sendAutonomyControl(Control control) {
  unique_lock<mutex> ul(m);
  cmdtlm.control(control.aileron, control.elevator, control.thrust, control.rudder);
}
