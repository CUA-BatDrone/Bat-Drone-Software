#include <thread>
#include <cmd_tlm.hpp>

class UI;

class TelemetryHandler {
private:
  bool run;
  UI &ui;
  CmdTlm &cmdtlm;
  thread tlmthread;
public:
  TelemetryHandler(CmdTlm &cmdtlm, UI &ui);
  void mainLoop();
  void start();
  void stop();
  void join();
};