#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>

const uint8_t address = 0x40;

class PWMDevice {
private:
	int fd;
	void setPeriod(unsigned char channel, unsigned short value);
public:
	PWMDevice(const char *path);
	~PWMDevice();

  /**
   * Controls a channel's position
   * @param position between -1 and 1
   */
	void setPosition(unsigned char channel, float position);
};
