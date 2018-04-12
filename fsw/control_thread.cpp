#include "control_thread.hpp"
#include <cmd_tlm.hpp>

using namespace std;
using namespace chrono;

ControlThread::ControlThread(bool &run, PWMDevice &pwm) : run(run), pwm(pwm) {}

void ControlThread::startThread() {
  m_thread = std::thread(&ControlThread::mainLoop, this);
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
  m_mutex.unlock();
  m_condition.notify_all();
}

void ControlThread::track() {
  m_mutex.lock();
  state = State::TRACK;
  m_mutex.unlock();
  m_condition.notify_all();
}

void ControlThread::mainLoop() {
  unique_lock<mutex> ul(m_mutex);
  while (run) {
    switch (state) {
      case State::MANUAL: {
        setControls();
        command_pending = false;
        steady_clock::time_point timeout = steady_clock::now() + seconds(2);
        do {
          if (m_condition.wait_until(ul, timeout) == cv_status::timeout) {
            state = State::FAILSAFE;
            break;
          }
        } while (!command_pending);
        break;
      }
      case State::FAILSAFE: {
        setDefaultControls();
        while (!command_pending) m_condition.wait(ul);
        break;
      }
      case State::TRACK: {
        // Unimplemented
        state = State::FAILSAFE;
      }
    }
  }
}

void ControlThread::setDefaultControls() {
  // AETR
  pwm.setPosition(4, 0.0f);
  pwm.setPosition(5, 0.0f);
  pwm.setPosition(6, 0.0f);
  pwm.setPosition(7, 0.0f);
}
void ControlThread::setControls() {
  // AETR
  pwm.setPosition(4, m_control.roll);
  pwm.setPosition(5, m_control.pitch);
  pwm.setPosition(6, m_control.thrust);
  pwm.setPosition(7, m_control.yaw);
}
