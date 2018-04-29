#ifndef CMD_TLM_HPP
#define CMD_TLM_HPP
#include "commands.hpp"
#include <stdint.h>
#include <vector>

using namespace std;

class PacketReader;
class PacketWriter;
class ControlPacketElement;

class Point {
public:
  float x, y;
  Point(float x, float y) : x(x), y(y) {
  }
};

class TrackerPoint : public Point {
public:
  int id;
  TrackerPoint(int id, float x, float y) : Point(x, y), id(id) {}
};

/**
 * This class handles communication using PacketWriter and PacketReader classes.
 */
class CmdTlm : public Commands {
private:
  PacketReader *packetReader;
  PacketWriter *packetWriter;
public:
  CmdTlm(PacketReader *reader, PacketWriter *writer);
  virtual void telemetry(Commands &callback);
  virtual void control(const ControlPacketElement &e);
  virtual void control(float ailerons, float elevator, float thrust, float rudder);
  virtual void lwirFrame(const uint16_t frame[60][80]);
  virtual void blob(uint16_t x, uint16_t y);
  virtual void pid(float p1, float i1, float d1, float p2, float i2, float d2, float p3, float i3, float d3, float p4, float i4, float d4);
  virtual void blobs(vector<Blob> & blobs);
  virtual void manual();
  virtual void autonomous();
  virtual void track(uint8_t x, uint8_t y);
  virtual void threshold(uint16_t low, uint16_t high);
};

#endif
