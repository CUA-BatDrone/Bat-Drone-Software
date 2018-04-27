#include "sender.hpp"

Sender::Sender(CmdTlm & cmdtlm) : cmdtlm(cmdtlm) {}

void Sender::sendFrame(uint16_t frame[60][80]) {
  unique_lock<mutex> ul(m);
  cmdtlm.lwirFrame(frame);
}

void Sender::sendAutonomyBlob(int x, int y) {
  unique_lock<mutex> ul(m);
  cmdtlm.blob(x, y);
}

void Sender::sendAutonomyBlobs(vector<Commands::Blob> &blobs) {
  unique_lock<mutex> ul(m);
  cmdtlm.blobs(blobs);
}

void Sender::sendAutonomyControl(Control control) {
  unique_lock<mutex> ul(m);
  cmdtlm.control(control.aileron, control.elevator, control.thrust, control.rudder);
}
