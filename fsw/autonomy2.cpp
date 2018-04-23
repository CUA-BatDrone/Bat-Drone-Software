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
  stack<pair<int, int>> next;
  bool visited[ROWS][COLS] = {};
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      visited[y][x] = false;
    }
  }
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      if (tFrame[y][x] && !visited[y][x]) {
        Blob blob;
        next.push(make_pair(x, y));
        while (!next.empty()) {
          blob.addPixel(x, y);
          int cx = next.top().first;
          int cy = next.top().second;
          next.pop();
          if (cx > 0 && tFrame[cy][cx - 1] && !visited[cy][cx - 1]) {
            visited[cy][cx - 1] = true;
            next.push(make_pair(cx - 1, cy));
          }
          if (cy > 0 && tFrame[cy - 1][cx] && !visited[cy - 1][cx]) {
            visited[cy - 1][cx] = true;
            next.push(make_pair(cx, cy - 1));
          }
          if (cx < COLS - 1 && tFrame[cy][cx + 1] && !visited[cy][cx + 1]) {
            visited[cy][cx + 1] = true;
            next.push(make_pair(cx + 1, cy));
          }
          if (cy < ROWS - 1 && tFrame[cy + 1][cx] && !visited[cy + 1][cx]) {
            visited[cy + 1][cx] = true;
            next.push(make_pair(cx, cy + 1));
          }
        }
        blob.calculateCentroid();
        blobs.push_front(blob);
      }
    }
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
      cout << largest->x << " " << largest->y << " " << largest->size << endl;
    }
  }
}