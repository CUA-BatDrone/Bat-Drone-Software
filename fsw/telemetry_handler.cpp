#include "telemetry_handler.hpp"
#include "pt1.h"
#include "cmd_tlm.hpp"
#include <iostream>
#include <string>
#include "ciaran.hpp"

using namespace std;

TelemetryHandler::TelemetryHandler(CmdTlm *cmdtlm, const char *pt1Device) : cmdtlm(cmdtlm), run(true) {
  pt1_init(pt1Device);
}

TelemetryHandler::~TelemetryHandler() {
  pt1_deinit();
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
    pt1_start();
    while (run) {
      pt1_frame frame;
      pt1_get_frame(&frame);
      cmdtlm->lwirFrame((const uint16_t(*)[80])frame.start);
	  detectBlob((uint16_t(*)[cols])frame.start);
	  cmdtlm->blob(dx, dy);
#ifdef _WIN32
      Sleep(60);
#else
#endif
    }
    pt1_stop();
  } catch (string e) {
    pt1_stop();
    cout << "ERROR" << endl;
    cout << e << endl;
  }
}