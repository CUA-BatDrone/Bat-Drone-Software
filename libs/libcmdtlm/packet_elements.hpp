#ifndef PACKET_ELEMENTS_HPP
#define PACKET_ELEMENTS_HPP

#include "packet_accessor_2.hpp"
#include <stdint.h>
#include "packet_element.hpp"
#include <string>

class HeaderPacketElement : public virtual PacketElement {
public:
  uint16_t sender_id, sequence;
  uint8_t packet_id;
  HeaderPacketElement();
  HeaderPacketElement(uint16_t sid, uint16_t seq, uint8_t pid);
  virtual void write(Writer *) const;
  virtual void read(Reader *);
};

class ControlPacketElement : public virtual PacketElement {
public:
  float pitch, roll, yaw, thrust;
  ControlPacketElement();
  ControlPacketElement(float p, float r, float y, float t);
  virtual void write(Writer *) const;
  virtual void read(Reader *);
  std::string toString();
};

class LWIRFrame : public virtual PacketElement {
public:
  long sequence;
  uint16_t (*frame)[60][80];
  LWIRFrame();
  LWIRFrame(void *frame, long sequence);
  virtual void write(Writer *) const;
  virtual void read(Reader *);
};

class SWIRFrame : public virtual PacketElement {
public:
  long sequence;
  uint8_t (*frame)[480][640][3];
  SWIRFrame();
  SWIRFrame(void *frame, long sequence);
  virtual void write(Writer *) const;
  virtual void read(Reader *);
};

#endif
