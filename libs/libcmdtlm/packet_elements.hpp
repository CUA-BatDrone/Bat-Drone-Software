#ifndef PACKET_ELEMENTS_HPP
#define PACKET_ELEMENTS_HPP

#include "packet_accessor.hpp"
#include <stdint.h>

class PacketElement {
  virtual void write(Writer *) = 0;
  virtual void read(Reader *) = 0;
};

class HeaderPacketElement : public PacketElement {
public:
  uint16_t sender_id, sequence;
  uint8_t packet_id;
  HeaderPacketElement(uint16_t sid, uint16_t seq, uint8_t pid);
  virtual void write(Writer *);
  virtual void read(Reader *);
};

class ControlPacketElement : public PacketElement {
public:
  uint8_t pitch, roll, yaw, thrust;
  ControlPacketElement(uint8_t p, uint8_t r, uint8_t y, uint8_t t);
  virtual void write(Writer *);
  virtual void read(Reader *);
};

class LWIRFrame : public PacketElement {
  long sequence;
  uint16_t (*frame)[80][60];
  LWIRFrame(void *frame, long sequence);
  virtual void write(Writer *);
  virtual void read(Reader *);
};

class SWIRFrame : public PacketElement {
  long sequence;
  uint8_t (*frame)[3][640][480];
  SWIRFrame(void *frame, long sequence);
  virtual void write(Writer *);
  virtual void read(Reader *);
};

#endif
