#include "pwm.hpp"
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

const uint8_t ADDRESS = 0x40;

// 205 410
PWMDevice::PWMDevice(const char *path) {
	if ((fd = open(path, O_RDWR)) < 0) {
		perror("open failed");
	} else {
		struct i2c_msg msgs[2];

		msgs[0].addr = ADDRESS;
		msgs[0].flags = 0;
		msgs[0].len = 2;
		uint8_t buf0[2];
		buf0[0] = 0xfe;
		// 0x86 sets prescaler to 20ms period with 25mhz clock
		buf0[1] = 0x86;
		msgs[0].buf = buf0;

		msgs[1].addr = ADDRESS;
		msgs[1].flags = 0;
		msgs[1].len = 2;
		uint8_t buf1[2];
		buf1[0] = 0x00;
		buf1[1] = 0x80;
		msgs[1].buf = buf1;

		struct i2c_rdwr_ioctl_data data;
		data.msgs = msgs;
		data.nmsgs = 2;
		if (ioctl(fd, I2C_RDWR, &data) < 0) {
			perror("PWMDevice()");
		}
	}
}

PWMDevice::~PWMDevice() {
	close(fd);
}

// max 2048
void PWMDevice::setPeriod(unsigned char channel, unsigned short period) {
	struct i2c_msg msgs[2];
	msgs[0].addr = ADDRESS;
	msgs[0].flags = 0;
	msgs[0].len = 2;
	uint8_t buf0[2];
	buf0[0] = 4 * channel + 0x08;
	buf0[1] = period & 0x00FF;
	msgs[0].buf = buf0;

	msgs[1].addr = ADDRESS;
	msgs[1].flags = 0;
	msgs[1].len = 2;
	uint8_t buf1[2];
	buf1[0] = 4 * channel + 0x09;
	buf1[1] = (period >> 8) & 0x000F;
	msgs[1].buf = buf1;

	struct i2c_rdwr_ioctl_data data;
	data.msgs = msgs;
	data.nmsgs = 2;
	if (ioctl(fd, I2C_RDWR, &data) < 0) {
		perror("setPeriod()");
	}
}

void PWMDevice::setPosition(unsigned char channel, float position) {
	// 0xcd is 1ms
	setPeriod(channel, (position + 1) / 2 * 0xcd + 0xcd);
}
