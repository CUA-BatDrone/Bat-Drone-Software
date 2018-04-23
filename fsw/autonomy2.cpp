#include <stdlib.h>
#include <cstring>
#include <stack>
#include <string>
#include <iostream>
#include "autonomy2.hpp"
#include "control.hpp"
#include "control_arbiter.hpp"

using namespace std;

void Autonomy2::threshold(bool out[ROWS][COLS], const uint16_t in[ROWS][COLS], uint16_t low, uint16_t high) {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      out[y][x] = in[y][x] < high && in[y][x] > low;
    }
  }
}

list<Blob> Autonomy2::findBlobs(bool tFrame[ROWS][COLS]) {
  list<Blob> blobs;
  list<list<Blob>::iterator> blobPointers;
  list<list<Blob>::iterator>::iterator blobDoublePointers[ROWS][COLS];
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      if (tFrame[y][x]) {
        if (y > 0 && tFrame[y - 1][x]) {
          if (x > 0 && tFrame[y][x - 1] && *blobDoublePointers[y][x - 1] != *blobDoublePointers[y - 1][x]) {
            // merge blob pointers
            (*blobDoublePointers[y - 1][x])->mergeBlob(**blobDoublePointers[y][x - 1]);
            blobs.erase(*blobDoublePointers[y][x - 1]);
            *blobDoublePointers[y][x - 1] = *blobDoublePointers[y - 1][x];
          }
          blobDoublePointers[y][x] = blobDoublePointers[y - 1][x];
          (*blobDoublePointers[y][x])->addPixel(x, y);
        } else if (x > 0 && tFrame[y][x - 1]) {
          blobDoublePointers[y][x] = blobDoublePointers[y][x - 1];
          (*blobDoublePointers[y][x])->addPixel(x, y);
        } else {
          blobDoublePointers[y][x] = blobPointers.insert(blobPointers.cend(), blobs.insert(blobs.cend(), Blob()));
        }
      }
    }
  }
  for (Blob &b : blobs) {
    b.calculateCentroid();
  }
  return blobs;
}



void Autonomy2::giveFrame(uint16_t frame[ROWS][COLS]) {
	memcpy(&buffer.getFront(), frame, sizeof(uint16_t [ROWS][COLS]));
	buffer.swapFront();
}

void Autonomy2::mainLoop(bool & run) {
  while (run) {
	  buffer.swapBack();
    bool tFrame[ROWS][COLS];
	  threshold(tFrame, buffer.getBack());
    list<Blob> blobs = findBlobs(tFrame);
    // cout << blobs.size() << endl;
    if (blobs.size()) {
      list<Blob>::const_iterator largest = blobs.cbegin();
      for (list<Blob>::const_iterator i = ++blobs.cbegin(); i != blobs.cend(); i++) {
        if (i->size > largest->size) largest = i;
      }
      send.sendAutonomyBlob(largest->x, largest->y);
    }
  }
}