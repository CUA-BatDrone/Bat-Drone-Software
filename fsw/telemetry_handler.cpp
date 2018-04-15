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
  try {
    while (run) {
      if (pt1_init(pt1Device) < 0) {
        cerr << "Unable to init libpt1" << endl;
}
      if (pt1_start() < 0) {
        cerr << "Unable to start libpt1" << endl;
        continue;
      }
      while (run) {
        pt1_frame frame;
        if (pt1_get_frame(&frame) < 0) {
          break;
        }
        cmdtlm->lwirFrame((const uint16_t(*)[80])frame.start);
        detectBlob((uint16_t(*)[cols])frame.start);
        cmdtlm->blob(dx, dy);
#ifdef _WIN32
        Sleep(60);
#endif
      }
      pt1_stop();
      pt1_deinit();
    }
  } catch (string e) {
    pt1_stop();
    pt1_deinit();
    cout << "TelemetryHandler error" << endl;
    cout << e << endl;
  }
}