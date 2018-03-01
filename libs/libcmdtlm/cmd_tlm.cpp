#include "commands.hpp"
#include "cmd_tlm.hpp"

CmdTlm::CmdTlm(uint16_t sid, PacketReader *r, PacketWriter *w) {
  out_id = sid;
  in_seq = 0;
  out_seq = 0;
  packetReader = r;
  packetWriter = w;
}

void CmdTlm::telemetry(Commands *callback) {
  // packetReader->read_packet();
  // HeaderPacketElement h;
  // h.read(packetReader);
  // switch(h.packet_id) {
  // case 0:
  //   {
  //     ControlPacketElement c;
  //     c.read(PacketReader);
  //     callback->control(&c);
  //   }
  //   break;
  // case 1:
  //   {
  //     uint16_t (*buffer)[80][60];
  //     lwirFrame f;
  //   }
  // }
}

void CmdTlm::control(ControlPacketElement *c) {
  // HeaderPacketElement h(out_id, out_seq, 0)
  // h.write(packetWriter);
  // c->write(packetWriter);
  // packetWriter->write_packet();
}

void CmdTlm::lwirFrame(SWIRFrame *f) {
  // HeaderPacketElement h(out_id, out_seq, 1);
  // h.write(packetWriter);
  // frame->write(packetWriter);
  // packetWriter->write_packet();
}

void CmdTlm::swirFrame(SWIRFrame *frame) {
  // TODO
}
