#include "command_handler.hpp"
#include "pwm.hpp"

CommandHandler::CommandHandler(bool &run, CmdTlm *cmdtlm, const char *pwmDevice) : cmdtlm(cmdtlm), pwm(pwmDevice), ctl(run, pwm), run(run) {}

void CommandHandler::mainLoop() {
  class CommandListener : public Commands {
  public:
    PWMDevice &pwm;
    ControlThread &ctl;
    CommandListener(PWMDevice &pwm, ControlThread &ctl) : pwm(pwm), ctl(ctl) {
    }

    void control(const ControlPacketElement &e) {
      // Receiver label
      // AETR
      pwm.setPosition(4, e.roll);
      pwm.setPosition(5, e.pitch);
      pwm.setPosition(6, e.thrust);
      pwm.setPosition(7, e.yaw);
    }
  } cl(pwm, ctl);
  while (run) {
    cmdtlm->telemetry(cl);
  }

}