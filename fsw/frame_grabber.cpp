#include "frame_grabber.hpp"
#include "pt1.h"

#include <chrono>

using namespace std;

FrameGrabber::FrameGrabber(Autonomy & autonomy, Sender & sender, const char * device) : autonomy(autonomy), sender(sender), device(device) {}

void FrameGrabber::mainLoop(bool & run) {
  while (run) {
    if (pt1_init(device) < 0 && pt1_init("/dev/video0") < 0 && pt1_init("/dev/video1") < 0) {
      cerr << "Unable to init libpt1 on " << device << endl;
      this_thread::sleep_for(chrono::seconds(1));
      continue;
    }
    cout << "LWIR Initialized" << endl;
    this_thread::sleep_for(chrono::seconds(4));
    if (pt1_start() < 0) {
      cerr << "Unable to start libpt1" << endl;
      pt1_deinit();
      this_thread::sleep_for(chrono::seconds(1));
      continue;
    }
    if (pt1_disable_ffc() < 0) {
      cerr << "Unable to disable ffc" << endl;
      pt1_deinit();
      this_thread::sleep_for(chrono::seconds(1));
      continue;
    }
    while (run) {
      pt1_frame frame;
      if (pt1_get_frame(&frame) < 0) {
        cerr << "Unable to get frame" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        break;
      }
      try {
		// Send frames to sender and autonomy classes
        sender.sendFrame((uint16_t(*)[80])frame.start);
        autonomy.giveFrame((uint16_t(*)[80])frame.start);
      } catch (string e) {
        cerr << "Error sending LWIR frame: " << e << endl;
        this_thread::sleep_for(chrono::seconds(1));
      }
#ifdef _WIN32
      this_thread::sleep_for(chrono::milliseconds(60));
#endif
    }
    pt1_stop();
    pt1_deinit();
  }
}

