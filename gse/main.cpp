#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdint.h>

#include <SDL.h>

#include "cmd_tlm.hpp"
#include "ui.hpp"
#include "telemetry_handler.hpp"
#include <thread>

using namespace std;

int main(int argc, char* argv[]) {
  const char *address = "192.168.0.1";
  int port_send = 1212, port_recv = 2121;
  const char *maddress = "232.232.232.232";
  switch (argc) {
  default:
  case 5:
    port_recv = stoi(argv[4]);
  case 4:
    maddress = argv[3];
  case 3:
    port_send = stoi(argv[2]);
  case 2:
    address = argv[1];
  case 1:
  case 0:;
  }

  cout << "using " << address << endl;

    try {
    UDPSocket ws;
    UDPSocket rs;
    ws.setMTUDiscovery(UDPSocket::IP_PMTUDISC_ENUM::DONT);
    rs.bind(port_recv);
    ws.connect(address, port_send);
    rs.add(maddress, address);
    UDPAddrPacketWriter w(Socket::stringToAddr(address, port_send), ws);
    UDPPacketReader r(rs);
    CmdTlm cmdtlm(&r, &w);

    UI ui(cmdtlm);
    TelemetryHandler th(cmdtlm, ui);
    th.start();
    ui.mainLoop();
    th.stop();
    th.join();
    rs.drop(maddress, address);
    return 0;
  }
  catch (string *e) {
    std::cerr << e << endl;
    while (true);
  }
}


//
// void sendDir(float Pitch, float Yaw, float Roll, float Thrust)
// {
//   //toDo sendDir to FSW for CIARAN
// }
//
// int main(int argc, char* argv[]) {
//
//
//   //for now the user will have to input the values, once we get into the
//   //thick of things we will enhanced control
//   float Pitch, Yaw, Roll, Thrust;
//   cout << "Please enter the pitch: ";
//   cin >> Pitch;
//   cout << "Please enter the Yaw: ";
//   cin >> Yaw;
//   cout << "Please enter the Roll: ";
//   cin >> Roll;
//   cout << "Please enter the thrust: ";
//   cin >> Thrust;
//
//   sendDir(Pitch, Yaw, Roll, Thrust);
//
//   return 0;
// }
