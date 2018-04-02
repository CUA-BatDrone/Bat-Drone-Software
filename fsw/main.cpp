#include "cmd_tlm.hpp"
#include "pwm.hpp"
#include <iostream>
#include "command_handler.hpp"
#include <thread>
#include "telemetry_handler.hpp"
#include "command_handler.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  try {
    UDPSocket s;
    s.bind(1995);
    UDPSplitPacketReader r(s);
    UDPPacketWriter w(s);
    CmdTlm cmdtlm(&r, &w);

    TelemetryHandler t(&cmdtlm, "/dev/video1");
    t.startThread();
    CommandHandler c(&cmdtlm, "/dev/i2c-1");
    c.mainLoop();
    return 0;
  }
  catch (string e) {
    cout << e << endl;
  }
}
