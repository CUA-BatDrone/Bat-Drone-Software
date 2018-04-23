#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "control_arbiter.hpp"

class Receiver {
protected:
  const ControlArbiter & controlArbiter;
  bool & run;
public:
  Receiver(bool & run, ControlArbiter &);
  void mainLoop();
};

#endif