#ifndef CMD_TLM_HPP
#define CMD_TLM_HPP

#include "packet_elements.hpp"
#include "commands.hpp"



/**
 * This class handles communication using PacketWriter and PacketReader classes.
 */
class CmdTlm : public Commands {
private:
  PacketReader *packetReader;
  PacketWriter *packetWriter;
public:
  CmdTlm(PacketReader *reader, PacketWriter *writer);
  void telemetry(Commands *callback);
  virtual void control(ControlPacketElement *e);
  virtual void lwirFrame(SWIRFrame *frame);
  virtual void swirFrame(SWIRFrame *frame);
};

#endif
