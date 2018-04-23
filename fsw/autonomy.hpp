#ifndef AUTONOMY_HPP
#define AUTONOMY_HPP
#define COLS 80
#define ROWS 60
#include <iostream>
#include <string>
#include "control_arbiter.hpp"
#include "control.hpp"
#include "frame_grabber.hpp"
#include "sender.hpp"
#include "triple_buffer.hpp"

class Autonomy {
public:
	void detectBlob(uint16_t frame[][COLS]);
	void mainLoop();
	void sendFlightCommands(int x, int y);
	ControlArbiter &arbiter;
	Control con;
	FrameGrabber frameGrabber;
	Sender send;
	void giveFrame(Frame &frame);
	TripleBuffer < Frame > buffer;

	Autonomy(ControlArbiter &a) : arbiter(a) {}
	
};
#endif