#ifndef TELEMETRY_HANDLER_HPP
#define TELEMETRY_HANDLER_HPP

#include <thread>
#include <mutex>

using namespace std;

class CmdTlm;

class TelemetryHandler {
protected:
  bool &run;
  CmdTlm *cmdtlm;
  thread tlm_thread;
  const char *pt1Device;
  void mainLoop();
public:
  TelemetryHandler(bool &run, CmdTlm *cmdtlm, const char *pt1Device);
  void startThread();
  void stopThread();
  void joinThread();
};

#endif