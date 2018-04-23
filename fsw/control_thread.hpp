#ifndef CONTROL_THREAD_HPP
#define CONTROL_THREAD_HPP
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <pwm.hpp>
#include <commands.hpp>
#include <chrono>

using namespace std;
using namespace chrono;

class ControlThread : public Commands {
public:
  ControlThread(bool &run, PWMDevice &pwm, steady_clock::duration timeout);
  ~ControlThread();
  virtual void control(float a, float e, float t, float r);
  virtual void track();
protected:
  steady_clock::duration timeout;
  bool &run;
  PWMDevice &pwm;
  bool command_pending;
  class {
  public:
    float roll, pitch, thrust, yaw;
  } m_control;
  void setPendingControls();
  void setFailsafeControls();
  mutex m_mutex;
  condition_variable m_condition;
  thread m_thread;
  enum State {
    TRACK,
    MANUAL,
    FAILSAFE
  } state;
  void startThread();
  void stopThread();
  void joinThread();
  void mainLoop();
};

#endif