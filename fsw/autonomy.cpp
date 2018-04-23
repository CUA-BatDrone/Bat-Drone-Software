#include <stdlib.h>
#include <cstring>
#include <stack>
#include <string>
#include <iostream>
#include "autonomy.hpp"
#include "control.hpp"
#include "control_arbiter.hpp"

using namespace std;

uint16_t dx = 0;
uint16_t dy = 0;
uint16_t centerX = 0;
uint16_t centerY = 0;
uint16_t blobSize;
bool isCloser;
int nPixels = 0;
int nBlobs = 0;
uint16_t arr[ROWS][COLS];
stack<int> xStack;
stack<int> yStack;
int previous = 0;

int calculateCenterX(int x, int n) {
	if (n != 0) {
		int centerX = x / n + (x % n != 0);
		return centerX;
	}
}

int calculateCenterY(int y, int n) {
	if (n != 0) {
		int centerY = y / n + (y % n != 0);
		return centerY;
	}
}

void resetVariables() {
	nPixels = 0;
	dx = 0;
	dy = 0;
}

void stackX(int x) {
	xStack.push(x);
}

int getStackX() {
	int x = xStack.top();
	xStack.pop();
	return x;
}

void stackY(int y) {
	yStack.push(y);
}

int getStackY() {
	int y = yStack.top();
	yStack.pop();
	return y;
}

bool isLarger(int n) {
	if (n > previous) {
		previous = n;
		return true;
	}
	else
		return false;
}

uint16_t setCenterX(int x) {
	centerX = x;
	return centerX;
}

uint16_t setCenterY(int y) {
	centerY = y;
	return centerY;
}

uint16_t setBlobSize(int n) {
	blobSize = n;
	return blobSize;
}

// Should take previous blob size and current pixel size and compare
void isNewBlobCloser(int previousBlob, int currentBlob) {
	if (previousBlob < currentBlob) {
		isCloser = true;
	}
	isCloser = false;
}

// Inputs are centerX and centerY
void Autonomy::sendFlightCommands(int x, int y) {
	//Roll, Yaw, Pitch, Thrust
	if (x * .90 > 30) {
		con.aileron  = -1.0;
		con.elevator =  0.0;
		con.thrust   =  0.0;
		con.rudder   =  0.0;
	}
	if (x * .90 < 30) {
		con.aileron  = 1.0;
		con.elevator = 0.0;
		con.thrust   = 0.0;
		con.rudder   = 0.0;
	}
	if (y * .90 > 40) {
		con.aileron  =  0.0;
		con.elevator = -1.0;
		con.thrust   =  0.0;
		con.rudder   =  0.0;
	}
	if (y * .90 < 40) {
		con.aileron  = 0.0;
		con.elevator = 1.0;
		con.thrust   = 0.0;
		con.rudder   = 0.0;
	}

	if (isCloser) {
		con.aileron  =  0.0;
		con.elevator =  0.0;
		con.thrust   = -1.0;
		con.rudder   =  0.0;
	}

	if (!isCloser) {
		con.aileron  = 0.0;
		con.elevator = 0.0;
		con.thrust   = 1.0;
		con.rudder   = 0.0;
	}
	send.sendAutonomyControl(con);
}

void calculatePrioriy() {
	if (isLarger(nPixels)) {
		setCenterX(calculateCenterX(dx, nPixels));
		setCenterY(calculateCenterY(dy, nPixels));
	}
}

uint16_t DFS(uint16_t arr[][COLS], int x, int y) {
	arr[x][y] = 0;
	while (!(xStack.empty() && yStack.empty())) {
		if (arr[x][y + 1] == 1 && y + 1 < COLS) {
			arr[x][y + 1] = 0;
			stackX(x); stackY(y);
			return DFS(arr, x, y + 1);
		}
		else if (arr[x + 1][y] == 1 && x + 1 < COLS) {
			arr[x + 1][y] = 0;
			stackX(x); stackY(y);
			return DFS(arr, x + 1, y);
		}

		else if (arr[x][y - 1] == 1 && y - 1 >= 0) {
			arr[x][y - 1] = 0;
			stackX(x); stackY(y);
			return DFS(arr, x, y - 1);
		}

		else if (arr[x - 1][y] == 1 && x - 1 >= 0) {
			arr[x - 1][y] = 0;
			stackX(x); stackY(y);
			return DFS(arr, x - 1, y);
		}
		else {
			int newX = getStackX();
			int newY = getStackY();
			dx += newX;
			dy += newY;
			nPixels++;
			return DFS(arr, newX, newY);
		}
	}
	calculatePrioriy();
	isNewBlobCloser(blobSize, nPixels);
}

void Autonomy::detectBlob(uint16_t arr[ROWS][COLS]) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (arr[i][j] == 1) {
				stackX(i); stackY(j);
				DFS(arr, i, j);
				setBlobSize(nPixels);
				nBlobs++;
				sendFlightCommands(centerX, centerY);
				resetVariables();
			}
		}
	}
}

void Autonomy::giveFrame(uint16_t frame[ROWS][COLS]) {
	memcpy(&buffer.getFront(), frame, sizeof(uint16_t [ROWS][COLS]));
	buffer.swapFront();
}

void Autonomy::mainLoop(bool & run) {
  while (run) {
	  buffer.swapBack();
	  detectBlob(buffer.getBack());
  }
}

//Synced at 12:20