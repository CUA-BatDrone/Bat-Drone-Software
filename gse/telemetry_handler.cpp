#include "telemetry_handler.hpp"
#include "ui.hpp"
#include <cmd_tlm.hpp>
#include <iostream>

using namespace std;

TelemetryHandler::TelemetryHandler(CmdTlm &cmdtlm, UI &ui) : cmdtlm(cmdtlm), ui(ui) {
  run = true;
}

void TelemetryHandler::mainLoop() {
  //try {
    while (run) {
      cmdtlm.telemetry(ui);
    }
  //} catch (string e) {
    //cout << e << endl;
  //}
}

void TelemetryHandler::start() {
  tlmthread = thread(&TelemetryHandler::mainLoop, this);
}

void TelemetryHandler::stop() {
  run = false;
}

void TelemetryHandler::join() {
  tlmthread.join();
}