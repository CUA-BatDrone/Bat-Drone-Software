#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdint.h>
#include <sys/ioctl>

const uint8_t ADDRESS = 0x40;

// 205 410
PWMDevice::PWMDevice(char *path) {
	if (fd = open(path, O_RDWR) == -1) {
		fprintf(stderr, "errno %i\n", errno);
	} else {
		struct i2c_msg msgs[2];

		msgs[0].addr = ADDRESS;
		msgs[0].flags = 0;
		msgs[0].len = 2;
		uint8_t buf0[2];
		buf0[0] = 0xfe;
		buf0[1] = 121;
		msgs[0].buf = buf0;

		msgs[1].addr = ADDRESS;
		msgs[1].flags = 0;
		msgs[1].len = 2;
		uint8_t buf1[2];
		buf1[0] = 0x00;
		buf1[1] = 0x00;
		msgs[1].buf = buf1;

		struct i2c_rdwr_ioctl_data data;
		data.msgs = &msgs;
		data.nmsgs = 2;
		ioctl(fd, I2C_RDWR, &data);
	}
}

PWMDevice::~PWMDevice() {
	close(fd);
}

// max 2048
PWMDevice::setPeriod(unsigned char channel, unsigned short period) {
	struct i2c_msg msgs[2];
	msgs[0].addr = ADDRESS;
	msgs[0].flags = 0;
	msgs[0].len = 2;
	uint8_t buf0[2];
	buf0[0] = 4 * channel + 0x06;
	buf0[1] = 4096 - period/2;
	msgs[0].buf = buf0;

	msgs[1].addr = ADDRESS;
	msgs[1].flags = 0;
	msgs[1].len = 2;
	uint8_t buf1[2];
	buf1[0] = 4 * channel + 0x08;
	buf1[1] = (period + 1) / 2;
	msgs[1].buf = buf1;

	struct i2c_rdwr_ioctl_data data;
	data.msgs = &msgs;
	data.nmsgs = 2;
	ioctl(fd, I2C_RDWR, &data);
}

setPosition(unsigned char channel, float position) {
	setPeriod(channel, (position + 1) / 2 * 205 + 205);
}
