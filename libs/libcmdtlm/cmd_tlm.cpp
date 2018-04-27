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
    case 0: {
      float roll, pitch, thrust, yaw;
      *packetReader >> roll >> pitch >> thrust >> yaw;
      callback.control(roll, pitch, thrust, yaw);
      ControlPacketElement c(pitch, roll, yaw, thrust);
      callback.control(c);
    break;
    }
    case 1: {
      uint16_t frame[60][80];
      *packetReader >> frame;
      callback.lwirFrame(frame);
      break;
    }
    case 2: {
      uint16_t x, y;
      *packetReader >> x >> y;
      callback.blob(x, y);
      break;
    }
    case 3: {
      float p, i, d;
      *packetReader >> p >> i >> d;
      callback.pid(p, i, d);
      break;
    }
    case 4: {
      uint16_t size;
      *packetReader >> size;
      vector<Blob> blobs(size);
      for (Blob &blob : blobs) {
        *packetReader >> blob.x >> blob.y >> blob.size;
      }
      callback.blobs(blobs);
      break;
    }
    case 5: {
      callback.manual();
      break;
    }
    case 6: {
      callback.autonomous();
      break;
    }
    case 7: {
      uint8_t x, y;
      *packetReader >> x >> y;
      callback.track(x, y);
      break;
    }
    default: {
      break;
    }
  }
}

// Use control(float, float, float, float) instead
void CmdTlm::control(const ControlPacketElement &c) {
  control(c.roll, c.pitch, c.thrust, c.yaw);
}

void CmdTlm::control(float roll, float pitch, float thrust, float yaw) {
  uint8_t packet_id = 0;
  *packetWriter << (uint8_t)0 << roll << pitch << thrust << yaw;
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

void CmdTlm::pid(float p, float i, float d) {
  *packetWriter << (uint8_t) 3 << p << i << d;
  packetWriter->write_packet();
}

void  CmdTlm::blobs(vector<Blob> & blobs) {
  *packetWriter << (uint8_t) 4 << (uint16_t) blobs.size();
  for (Blob &blob : blobs) {
    *packetWriter << blob.x << blob.y << blob.size;
  }
  packetWriter->write_packet();
}

void CmdTlm::manual() {
  *packetWriter << (uint8_t) 5;
  packetWriter->write_packet();
}

void CmdTlm::autonomous() {
  *packetWriter << (uint8_t) 6;
  packetWriter->write_packet();
}

void CmdTlm::track(uint8_t x, uint8_t y) {
  *packetWriter << (uint8_t) 7 << x << y;
  packetWriter->write_packet();
}