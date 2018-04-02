#ifndef THREAD_SAFE_CMD_TLM_HPP
#define THREAD_SAFE_CMD_TLM_HPP

#include "cmd_tlm.hpp"
#include <mutex>

using namespace std;

/**
* This class handles communication using PacketWriter and PacketReader classes in a thread safe manner.
*/
class ThreadSafeCmdTlm : public Commands {
private:
  mutex &readMutex;
  mutex &writeMutex;
public:
  ThreadSafeCmdTlm(PacketReader *reader, mutex &readMutex, PacketWriter *writer, mutex &writeMutex);
  virtual void telemetry(Commands &callback);
  virtual void control(const ControlPacketElement &e);
};

#endif