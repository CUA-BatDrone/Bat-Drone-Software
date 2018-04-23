#include <cmd_tlm.hpp>
#include <iostream>
#include <thread>
#include <string>

#include "drone_controller.hpp"
#include "control_arbiter.hpp"
#include "receiver.hpp"
#include "autonomy.hpp"
#include "frame_grabber.hpp"
#include "sender.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  // const char *address = "232.232.232.232";
  const char *address = "192.168.0.16";
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
    // setup cmdtlm
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

    while (true) {
      try {
        // Set up classes
        DroneController droneController;
        ControlArbiter controlArbiter(droneController);
        Receiver receiver(cmdtlm, controlArbiter);
        Sender sender(cmdtlm);
        Autonomy autonomy(controlArbiter, sender);
        FrameGrabber frameGrabber(autonomy, sender);
        bool run = true;
        thread droneControllerThread(&DroneController::mainLoop, &droneController, ref(run));
        thread receiverThread(&Receiver::mainLoop, &receiver, ref(run));
        thread frameGrabberThread(&FrameGrabber::mainLoop, &frameGrabber, ref(run));
        thread autonomyThread(&Autonomy::mainLoop, &autonomy, ref(run));
        droneControllerThread.join();
        receiverThread.join();
        frameGrabberThread.join();
        autonomyThread.join();

      } catch (string e) {
        cerr << e << endl << "Restarting threads" << endl;
        this_thread::sleep_for(seconds(1));
      }
    }

    return 0;
  }
  catch (string e) {
    cout << "ERROR" << endl;
    cout << e << endl;
    while (true);
  }
}
