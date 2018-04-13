#include "command_handler.hpp"
#include "pwm.hpp"

using namespace std;
using namespace chrono;

CommandHandler::CommandHandler(bool &run, CmdTlm *cmdtlm, const char *pwmDevice, steady_clock::duration timeout) : cmdtlm(cmdtlm), pwm(pwmDevice), ctl(run, pwm, timeout), run(run) {}

void CommandHandler::mainLoop() {
  class CommandListener : public Commands {
  public:
    ControlThread &ctl;
    CommandListener(ControlThread &ctl) : ctl(ctl) {
    }
    virtual void track() {
      ctl.track();
    }
    virtual void control(float roll, float pitch, float thrust, float yaw) {
      ctl.control(roll, pitch, thrust, yaw);
    }
  } cl(ctl);
  while (run) {
    cmdtlm->telemetry(cl);
  }

}