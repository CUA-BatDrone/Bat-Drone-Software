#include "commands.hpp"
#include "cmd_tlm.hpp"

CmdTlm::CmdTlm(PacketReader *r, PacketWriter *w) {
  packetReader = r;
  packetWriter = w;
}

void CmdTlm::telemetry(Commands *callback) {
  packetReader->read_packet();
  uint8_t packet_id;
  *packetReader >> packet_id;
  switch(packet_id) {
  case 0:
    {
      ControlPacketElement c;
      *packetReader >> c;
      callback->control(c);
    }
    break;
  case 1:
    {
      uint16_t frame[60][80];
      *packetReader >> frame;
      // TODO
    }
  }
}

void CmdTlm::control(const ControlPacketElement &c) {
  uint8_t packet_id = 0;
  *packetWriter << packet_id << c;
  packetWriter->write_packet();
}

void CmdTlm::lwirFrame(SWIRFrame *f) {
  uint8_t packet_id = 0;
  *packetWriter << packet_id << *f;
  packetWriter->write_packet();
}

void CmdTlm::swirFrame(SWIRFrame *frame) {
  // TODO
}
