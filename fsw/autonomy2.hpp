#ifndef AUTONOMY2_HPP
#define AUTONOMY2_HPP
#define COLS 80
#define ROWS 60
#include <stdint.h>
#include <iostream>
#include <string>
#include "control_arbiter.hpp"
#include "control.hpp"
#include "sender.hpp"
#include "triple_buffer.hpp"
#include <list>

class Blob {
public:
  int x, y, size;
  Blob() : x(0), y(0), size(0) {}
  void addPixel(int x, int y) {
    Blob::x += x;
    Blob::y += y;
    size++;
  }
  void calculateCentroid() {
    if (size) {
      x /= size;
      y /= size;
    }
  }
};

class Autonomy2 {
protected:
  void threshold(bool out[ROWS][COLS], const uint16_t in[ROWS][COLS], uint16_t low = 3200, uint16_t high = 3600);
  list<Blob> findBlobs(bool in[ROWS][COLS]);
public:
	Autonomy2(ControlArbiter &a, Sender &send) : arbiter(a), send(send) {}
	ControlArbiter &arbiter;
	Sender &send;
	void giveFrame(uint16_t frame[ROWS][COLS]);
	TripleBuffer < uint16_t [ROWS][COLS] > buffer;
	void mainLoop(bool & run);
};

typedef Autonomy2 Autonomy;
#endif