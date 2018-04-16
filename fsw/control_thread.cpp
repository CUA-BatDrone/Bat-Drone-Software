#include "control_thread.hpp"
#include <cmd_tlm.hpp>
#include <iostream>

using namespace std;
using namespace chrono;

ControlThread::ControlThread(bool &run, PWMDevice &pwm, steady_clock::duration timeout) : run(run), pwm(pwm), timeout(timeout), state(State::FAILSAFE), command_pending(false) {
  startThread();
}

ControlThread::~ControlThread() {
  stopThread();
  joinThread();
}

void ControlThread::startThread() {
  m_thread = thread(&ControlThread::mainLoop, this);
}

void ControlThread::stopThread() {
  run = false;
}

void ControlThread::joinThread() {
  m_thread.join();
}

void ControlThread::control(float a, float e, float t, float r) {
  m_mutex.lock();
  m_control.roll = a;
  m_control.pitch = e;
  m_control.thrust = t;
  m_control.yaw = r;
  state = State::MANUAL;
  command_pending = true;
  m_mutex.unlock();
  m_condition.notify_all();
}

void ControlThread::track() {
  m_mutex.lock();
  state = State::TRACK;
  command_pending = true;
  m_mutex.unlock();
  m_condition.notify_all();
}

void ControlThread::mainLoop() {
  unique_lock<mutex> ul(m_mutex);
  // Main state machine
  while (run) {
    switch (state) {
      // Send manual control;
      case State::MANUAL: {
        // Set controls
        setControls();
        command_pending = false;
        // Wait for next command or enter failsafe state
        steady_clock::time_point t = steady_clock::now() + timeout;
        do {
          if (m_condition.wait_until(ul, t) == cv_status::timeout) {
            state = State::FAILSAFE;
            break;
          }
        } while (!command_pending);
        break;
      }
      case State::FAILSAFE: {
        setFailsafeControls();
        command_pending = false;
        while (!command_pending) m_condition.wait(ul);
        break;
      }
      case State::TRACK: {
        // Unimplemented
        command_pending = false;
        state = State::FAILSAFE;
        break;
      }
    }
  }
}

void ControlThread::setFailsafeControls() {
  // AETR
  pwm.setPosition(3, 0.0f);
  pwm.setPosition(5, 0.0f);
  pwm.setPosition(6, -1.0f);
  pwm.setPosition(7, 0.0f);
}
void ControlThread::setControls() {
  // AETR
  pwm.setPosition(3, m_control.roll);
  pwm.setPosition(5, m_control.pitch);
  pwm.setPosition(6, m_control.thrust);
  pwm.setPosition(7, m_control.yaw);
}
