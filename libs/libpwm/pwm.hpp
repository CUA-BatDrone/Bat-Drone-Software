#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdint.h>

const uint8_t address = 0x40;

class PWMDevice {
	int fd;
	setPeriod(unsigned char channel, unsigned short value);
public:
	PWMDevice(char *path);
	~PWMDevice(char *path);

  /**
   * Controls a channel's position
   * @param position between -1 and 1
   */
	setPosition(unsigned char channel, float position);
}
