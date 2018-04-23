#include "ciaran.hpp"
#include "control_thread.hpp"
#include "command_handler.hpp"


/*
class blobAdjust {
public: 
	ControlThread &ctl;
	void adjustDrone(uint16_t x, uint16_t y, bool isFurther, bool isCloser);
};

void adjustDrone(uint16_t x, uint16_t y, bool isFurther, bool isCloser) {
	if (x * .90 < 30) {
		ctl.control(0.0, 0.0, -1.0, 0.0);
	}
	if (x * .90 > 30) {
		ctl.control(0.0, 0.0, 1.0, 0.0);
	}
	if (y * .90 < 40) {
		ctl.control(0.0, -1.0, 0.0, 0.0);
	}
	if (y * .90 > 40) {
		ctl.control(0.0, 1.0, 0.0, 0.0);
	}

	if (isFurther) {
		ctl.control(0.0, 0.0, 0.0, 1.0);
	}

	if (isCloser) {
		ctl.control(0.0, 0.0, 0.0, -1.0);
	}
}*/