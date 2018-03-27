#include "cmd_tlm.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  try {
    UDPSocket s;
    s.bind(1995);
    UDPSplitPacketReader r(s);
    CmdTlm cmdtlm(&r, NULL);

    class CommandListener : public Commands {
    public:
      void control(ControlPacketElement *e) {
        cout << e->toString();
      }
    } cl;
    while (true) {
      cmdtlm.telemetry(&cl);
    }
    return 0;
  }
  catch (string e) {
    cout << e << endl;
  }
}
