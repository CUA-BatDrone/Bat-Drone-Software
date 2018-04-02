#ifndef COMMAND_HANDLER_CPP
#define COMMAND_HANDLER_CPP
#include "cmd_tlm.hpp"
#include "pwm.hpp"

using namespace std;

class CommandHandler {
private:
  PWMDevice pwm;
  CmdTlm *cmdtlm;
public:
  CommandHandler(CmdTlm *, const char *pwmDevice);
  void mainLoop();
};

#endif