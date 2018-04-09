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
    rs.bind(1212);
    UDPSocket ws;
    ws.setMTUDiscovery(UDPSocket::IP_PMTUDISC_ENUM::DONT);
    UDPPacketReader r(rs);
    UDPAddrPacketWriter w(Socket::stringToAddr("232.232.232.232", 2121), ws);
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
    cout << "ERROR" << endl;
    cout << e << endl;
    while (true);
  }
}
