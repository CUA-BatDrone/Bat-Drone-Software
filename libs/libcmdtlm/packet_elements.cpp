#include "packet_elements.hpp"
#include "packet_accessor.hpp"


HeaderPacketElement::HeaderPacketElement() {}

HeaderPacketElement::HeaderPacketElement(uint16_t sid, uint16_t seq, uint8_t pid) {
  sender_id = sid;
  sequence = seq;
  packet_id = pid;
}

void HeaderPacketElement::write(Writer *w) const {
  w->write(&sender_id);
  w->write(&sequence);
  w->write(&packet_id);
}

void HeaderPacketElement::read(Reader *r) {
  r->read(&sender_id);
  r->read(&sequence);
  r->read(&packet_id);
}

/******************************************************************************/

ControlPacketElement::ControlPacketElement() {}

ControlPacketElement::ControlPacketElement(float p, float r, float y, float t) {
  pitch = p;
  roll = r;
  yaw = y;
  thrust = t;
}

void ControlPacketElement::write(Writer *w) const {
  w->write(&pitch);
  w->write(&roll);
  w->write(&yaw);
  w->write(&thrust);
}

void ControlPacketElement::read(Reader *r) {
  r->read(&pitch);
  r->read(&roll);
  r->read(&yaw);
  r->read(&thrust);
}

/******************************************************************************/

LWIRFrame::LWIRFrame() {}

LWIRFrame::LWIRFrame(void *frame, long sequence) {
  this->sequence = sequence;
  this->frame = (uint16_t (*)[60][80]) frame;
}

void LWIRFrame::write(Writer *w) const {
  w->write(&sequence);
  w->write(frame);
}

void LWIRFrame::read(Reader *r) {
  r->read(&sequence);
  r->read(frame);
}

/******************************************************************************/

SWIRFrame::SWIRFrame() {};

SWIRFrame::SWIRFrame(void *frame, long sequence) {
  this->sequence = sequence;
  this->frame = (uint8_t (*)[480][640][3]) frame;
}

void SWIRFrame::write(Writer *w) const {
  w->write(&sequence);
  w->write(frame);
}

void SWIRFrame::read(Reader *r) {
  r->read(&sequence);
  r->read(frame);
}
