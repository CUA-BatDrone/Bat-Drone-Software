#ifndef CONTROL_THREAD_HPP
#define CONTROL_THREAD_HPP
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <pwm.hpp>

using namespace std;

class ControlThread {
public:
  ControlThread(bool &run, PWMDevice &pwm);
  void startThread();
  void stopThread();
  void joinThread();
  void control(float a, float e, float t, float r);
  void track();
  void mainLoop();
protected:
  bool &run;
  PWMDevice &pwm;
  bool command_pending;
  float control_a, control_e, control_t, control_r;
  void setControls();
  void setDefaultControls();
  mutex m_mutex;
  condition_variable m_condition;
  thread m_thread;
  enum State {
    TRACK,
    MANUAL,
    FAILSAFE
  } state;
};

#endif