#include "telemetry_handler.hpp"
#include "pt1.h"
#include "cmd_tlm.hpp"
#include <iostream>
#include <string>
#include "ciaran.hpp"

using namespace std;

TelemetryHandler::TelemetryHandler(bool &run, CmdTlm *cmdtlm, const char *pt1Device) : cmdtlm(cmdtlm), run(run), pt1Device(pt1Device) {
}

void TelemetryHandler::startThread() {
  tlm_thread = thread(&TelemetryHandler::mainLoop, this);
}

void TelemetryHandler::stopThread() {
  run = false;
}

void TelemetryHandler::joinThread() {
  tlm_thread.join();
}

void TelemetryHandler::mainLoop() {
  while (run) {
    if (pt1_init(pt1Device) < 0) {
      cerr << "Unable to init libpt1" << endl;
      pt1_deinit();
      this_thread::sleep_for(chrono::seconds(1));
      continue;
    }
    cout << "LWIR Initialized" << endl;
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
          cmdtlm->lwirFrame((const uint16_t(*)[80])frame.start);
        } catch (string e) {
          cerr << "Error sending LWIR frame: " << e << endl;
          this_thread::sleep_for(chrono::seconds(1));
        }
        detectBlob((uint16_t(*)[cols])frame.start);
        try {
          //cmdtlm->blob(dx, dy);
        } catch (string e) {
          cerr << "Error sending blob: " << e << endl;
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