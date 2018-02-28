#include "commands.hpp"
#include "cmd_tlm.hpp"

CmdTlm::CmdTlm(uint16_t sid, PacketReader *r, PacketWriter *w) {
  sender_id = sid;
  count = 0;
  packetReader = r;
  packetWriter = w;
}

void CmdTlm::telemetry(Commands *callback) {}

void CmdTlm::control(ControlPacketElement *e) {}

void CmdTlm::lwirFrame(SWIRFrame *frame) {}

void CmdTlm::swirFrame(SWIRFrame *frame) {}
