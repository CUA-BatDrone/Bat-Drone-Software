#include "cmd_tlm.hpp"
#include "pwm.hpp"
#include <iostream>
#include "command_handler.hpp"
#include <thread>
#include "telemetry_handler.hpp"
#include "command_handler.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  const char *address = "232.232.232.232";
  int port_send = 2121, port_recv = 1212;
  switch (argc) {
  default:
  case 4:
  	port_recv = stoi(argv[3]);
  case 3:
  	port_send = stoi(argv[2]);
  case 2:
	  address = argv[1];
  case 1:
  case 0:;
  }

  try {

	cout << "using " << address << endl;

    cout << "Creating read socket" << endl;
    UDPSocket rs;
    cout << "Binding read socket" << endl;
    rs.bind(port_recv);
    cout << "Creating write socket" << endl;
    UDPSocket ws;
    cout << "Setting write MTU" << endl;
    ws.setMTUDiscovery(UDPSocket::IP_PMTUDISC_ENUM::DONT);
    UDPPacketReader r(rs);
    UDPAddrPacketWriter w(Socket::stringToAddr(address, port_send), ws);
    CmdTlm cmdtlm(&r, &w);

    bool run = true;
    TelemetryHandler t(run, &cmdtlm, "/dev/video0");
    t.startThread();
    CommandHandler c(run, &cmdtlm, "/dev/i2c-1", chrono::seconds(2));
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
