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
    try {
      while (pt1_init(pt1Device) < 0) {
        cerr << "Unable to init libpt1" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        continue;
      }
      if (pt1_start() < 0) {
        cerr << "Unable to start libpt1" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        continue;
      }
      if (pt1_disable_ffc() < 0) {
        cerr << "Unable to disable ffc" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        continue;
      }
      break;
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
          cerr << e;
          this_thread::sleep_for(chrono::seconds(1));
          continue;
        }
        detectBlob((uint16_t(*)[cols])frame.start);
        try {
          cmdtlm->blob(dx, dy);
        }
        catch (string e) {
          cerr << e;
          this_thread::sleep_for(chrono::seconds(1));
          continue;
        }
#ifdef _WIN32
        Sleep(60);
#endif
      }
      pt1_stop();
      pt1_deinit();
    } catch (string e) {
      pt1_stop();
      pt1_deinit();
      cout << "TelemetryHandler error" << endl;
      cout << e << endl;
    }
  }
}