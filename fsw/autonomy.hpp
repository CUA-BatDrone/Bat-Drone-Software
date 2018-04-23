#ifndef AUTONOMY_HPP
#define AUTONOMY_HPP
#define COLS 80
#define ROWS 60
#include <stdint.h>
#include <iostream>
#include <string>
#include "control_arbiter.hpp"
#include "control.hpp"
#include "sender.hpp"
#include "triple_buffer.hpp"

class Autonomy {
public:
	void detectBlob(uint16_t frame[][COLS]);
	void mainLoop(bool & run);
	void sendFlightCommands(int x, int y);
	ControlArbiter &arbiter;
	Control con;
	Sender &send;
	void giveFrame(uint16_t frame[ROWS][COLS]);
	TripleBuffer < uint16_t [ROWS][COLS] > buffer;

	Autonomy(ControlArbiter &a, Sender &send) : arbiter(a), send(send) {}
	
};
#endif