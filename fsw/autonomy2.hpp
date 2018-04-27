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



class Autonomy2 {
public:
  class Blob {
  public:
    int x, y, size;
    Blob();
    void addPixel(int x, int y);
    void calculateCentroid();
  };
protected:
  void threshold(bool out[ROWS][COLS], const uint16_t in[ROWS][COLS], uint16_t low = 2800, uint16_t high = 3600);
  list<Blob> findBlobs(bool in[ROWS][COLS]);
  void blobListToCmdBlobVect(std::vector<Commands::Blob> &commandBlobs, std::list<Autonomy2::Blob> &blobs);
  Blob findLargestBlob(std::list<Autonomy2::Blob> &blobs);
  Control calculateFlightControls(Blob blob);
  TripleBuffer<Control> receivedControlBuffer;
  // The target blob the user selects to track
  TripleBuffer <Blob> receivedTargetBuffer;
  float targetSize;
public:
	Autonomy2(ControlArbiter &a, Sender &send) : arbiter(a), send(send) {}
	ControlArbiter &arbiter;
	Sender &send;
	void giveFrame(uint16_t frame[ROWS][COLS]);
  void giveControl(Control control);
  void giveTarget(int x, int y);
	TripleBuffer < uint16_t [ROWS][COLS] > buffer;
  void mainLoop(bool & run);
};

typedef Autonomy2 Autonomy;
#endif