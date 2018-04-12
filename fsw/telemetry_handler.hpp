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
  void mainLoop();
public:
  TelemetryHandler(bool &run, CmdTlm *cmdtlm, const char *pt1Device);
  ~TelemetryHandler();
  void startThread();
  void stopThread();
  void joinThread();
};

#endif