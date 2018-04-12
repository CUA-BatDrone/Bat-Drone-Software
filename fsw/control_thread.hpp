#ifndef CONTROL_THREAD_HPP
#define CONTROL_THREAD_HPP
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <pwm.hpp>
#include <commands.hpp>

using namespace std;

class ControlThread : Commands {
public:
  ControlThread(bool &run, PWMDevice &pwm);
  void startThread();
  void stopThread();
  void joinThread();
  virtual void control(float a, float e, float t, float r);
  virtual void track();
  void mainLoop();
protected:
  bool &run;
  PWMDevice &pwm;
  bool command_pending;
  class {
  public:
    float roll, pitch, thrust, yaw;
  } m_control;
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