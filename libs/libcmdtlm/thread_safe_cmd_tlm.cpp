#include "thread_safe_cmd_tlm.hpp"

ThreadSafeCmdTlm::ThreadSafeCmdTlm(PacketReader *reader, mutex readMutex, PacketWriter *writer, mutex writeMutex) : CmdTlm(reader, writer) {

}
void ThreadSafeCmdTlm::telemetry(Commands *callback) {
  readMutex.lock();
  CmdTlm::telemetry(callback);
  readMutex.unlock();
}
void ThreadSafeCmdTlm::control(const ControlPacketElement &e) {
  writeMutex.lock();
  CmdTlm::control(e);
  writeMutex.unlock();
}