#include "command_handler.hpp"
#include "pwm.hpp"

CommandHandler::CommandHandler(CmdTlm *ct, const char *pwmDevice) : cmdtlm(ct), pwm(pwmDevice) {}

void CommandHandler::mainLoop() {
  class CommandListener : public Commands {
  public:
    PWMDevice & pwm;
    CommandListener(PWMDevice &pwm) : pwm(pwm) {
    }

    void control(const ControlPacketElement &e) {
      // Receiver label
      // AETR
      pwm.setPosition(4, e.roll);
      pwm.setPosition(5, e.pitch);
      pwm.setPosition(6, e.thrust);
      pwm.setPosition(7, e.yaw);
    }
  } cl(pwm);
  while (true) {
    cmdtlm->telemetry(cl);
  }

}