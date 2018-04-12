#ifndef COMMAND_HANDLER_CPP
#define COMMAND_HANDLER_CPP
#include "cmd_tlm.hpp"
#include "pwm.hpp"
#include "control_thread.hpp"

using namespace std;

class CommandHandler {
protected:
  bool &run;
  PWMDevice pwm;
  CmdTlm *cmdtlm;
  ControlThread ctl;
public:
  CommandHandler(bool &run, CmdTlm *cmdtlm, const char *pwmDevice);
  void mainLoop();
};

#endif