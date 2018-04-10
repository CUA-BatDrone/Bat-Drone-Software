#include "cmd_tlm.hpp"
#include "commands.hpp"
#include "packet_accessor_2.hpp"
#include "packet_elements.hpp"

CmdTlm::CmdTlm(PacketReader *r, PacketWriter *w) {
  packetReader = r;
  packetWriter = w;
}

void CmdTlm::telemetry(Commands &callback) {
  packetReader->read_packet();
  uint8_t packet_id;
  *packetReader >> packet_id;
  switch(packet_id) {
  case 0:
    {
      ControlPacketElement c;
      *packetReader >> c;
      callback.control(c);
    }
    break;
  case 1:
    {
      uint16_t frame[60][80];
      *packetReader >> frame;
      callback.lwirFrame(frame);
    }
    break;
  case 2:
    {
      uint16_t x, y;
      *packetReader >> x >> y;
      callback.blob(x, y);
    }
  }
}

void CmdTlm::control(const ControlPacketElement &c) {
  uint8_t packet_id = 0;
  *packetWriter << packet_id << c;
  packetWriter->write_packet();
}

void CmdTlm::lwirFrame(const uint16_t frame[60][80]) {
  *packetWriter << (uint8_t) 1;
  packetWriter->write(frame, 80 * 60 * sizeof(uint16_t));
  packetWriter->write_packet();
}

void CmdTlm::blob(uint16_t x, uint16_t y) {
  *packetWriter << (uint8_t) 2 << x << y;
  packetWriter->write_packet();
}
