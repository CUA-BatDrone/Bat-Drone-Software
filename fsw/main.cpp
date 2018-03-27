#include "cmd_tlm.hpp"
#include "pwm.hpp"
#include <iostream>

using namespace std;

 // AETR

int main(int argc, char* argv[]) {
  try {
    UDPSocket s;
    s.bind(1995);
    UDPSplitPacketReader r(s);
    CmdTlm cmdtlm(&r, NULL);
    PWMDevice pwm("/dev/i2c-1");

    class CommandListener : public Commands {
    public:
      PWMDevice *pwm;
      CommandListener(PWMDevice *pwm) {
        this->pwm = pwm;
      }
      void control(ControlPacketElement *e) {
        pwm->setPosition(4, e->roll);
        pwm->setPosition(5, e->pitch);
        pwm->setPosition(6, e->thrust);
        pwm->setPosition(7, e->yaw);
      }
    } cl(&pwm);

    while (true) {
      cmdtlm.telemetry(&cl);
    }
    return 0;
  }
  catch (string e) {
    cout << e << endl;
  }
}
