#include "pwm.hpp"
#include <iostream>

using namespace std;

#define PWM_TAG "Fake PWMDevice:"

PWMDevice::PWMDevice(const char *path) {
	cout << PWM_TAG << " requesting to open " << path << endl;
}

PWMDevice::~PWMDevice() {
	cout << PWM_TAG << " closing PWM " << endl;
}

void PWMDevice::setPeriod(unsigned char channel, unsigned short period) {
	cout << PWM_TAG << " PWMDevice::setPeriod {\n  channel: " << (int) channel << ",\n  period:  " << period << "\n}" << endl;
}

void PWMDevice::setPosition(unsigned char channel, float position) {
	cout << PWM_TAG << " PWMDevice::setPosition {\n  channel:  " << (int) channel << ",\n  position: " << position << "\n}" << endl;
	// setPeriod(channel, (position + 1) / 2 * 0xcd + 0xcd);
}
