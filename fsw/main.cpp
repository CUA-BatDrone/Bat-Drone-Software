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
    UDPSocket rs;
    UDPSocket ws;
    rs.bind(1212);
    UDPSplitPacketReader r(rs);
    //UDPSplitAddrPacketWriter w(Socket::stringToAddr("232.232.232.232", 2121), s);
    UDPSplitAddrPacketWriter w(Socket::stringToAddr("127.0.0.1", 2121), ws);
    CmdTlm cmdtlm(&r, &w);

    TelemetryHandler t(&cmdtlm, "/dev/video1");
    t.startThread();
    CommandHandler c(&cmdtlm, "/dev/i2c-1");
    c.mainLoop();
    t.stopThread();
    t.joinThread();
    return 0;
  }
  catch (string e) {
    cout << e << endl;
  }
}
