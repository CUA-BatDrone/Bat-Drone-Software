#ifndef CONTROL_HPP
#define CONTROL_HPP

/**
 * A directional control for the drone
 */
class Control {
public:
  float aileron;
  float elevator;
  float thrust;
  float rudder;
};

#endif