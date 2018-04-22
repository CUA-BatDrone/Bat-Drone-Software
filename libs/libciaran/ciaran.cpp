#include <iostream>
#include <string>
#include <stack>
#include "ciaran.hpp"


using namespace std;

uint16_t dx = 0;
uint16_t dy = 0;
uint16_t centerX = 0;
uint16_t centerY = 0;
int nPixels = 0;
int nBlobs = 0;
uint16_t arr[rows][cols];
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

int storeLargerX(int x) {
	return x;
}
int storeLargerY(int y) {
	return y;
}

void setCenterX(int x) {
	centerX = x;
}

void setCenterY(int y) {
	centerY = y;
}

int getCenterX() {
	return centerX;
}

int getCenterY() {
	return centerY;
}

void calculatePrioriy() {
	if (isLarger(nPixels)) {
		setCenterX(calculateCenterX(dx, nPixels));
		setCenterY(calculateCenterY(dy, nPixels));
	}
}

uint16_t DFS(uint16_t arr[][cols], int x, int y) {
	arr[x][y] = 0;
	while (!(xStack.empty() && yStack.empty())) {
		if (arr[x][y + 1] == 1 && y + 1 < cols) {
			arr[x][y + 1] = 0;
			stackX(x); stackY(y);
			return DFS(arr, x, y + 1);
		}
		else if (arr[x + 1][y] == 1 && x + 1 < rows) {
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
}



int detectBlob(uint16_t arr[rows][cols]) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr[i][j] == 1) {
				stackX(i); stackY(j);
				DFS(arr, i, j);
				nBlobs++;
				resetVariables();
			}
		}
	}

	return 0;
}

void programRunner() {
	detectBlob(arr);
}

int main2() {
	programRunner();

	system("pause");
	return 0;
}
