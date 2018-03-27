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
   * @param channel The channel on the PWM Hat starting at zero.
   * @param position -1 stick left or down. 1 stick right or up.
   */
	void setPosition(unsigned char channel, float position);
};
