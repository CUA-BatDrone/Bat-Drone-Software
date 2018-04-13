#ifndef COMMAND_HANDLER_CPP
#define COMMAND_HANDLER_CPP
#include "cmd_tlm.hpp"
#include "pwm.hpp"
#include "control_thread.hpp"

class CommandHandler {
protected:
  bool &run;
  PWMDevice pwm;
  CmdTlm *cmdtlm;
  ControlThread ctl;
public:
  CommandHandler(bool &run, CmdTlm *cmdtlm, const char *pwmDevice, std::chrono::steady_clock::duration timeout);
  void mainLoop();
};

#endif