#ifndef CONTROL
#define CONTROL

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