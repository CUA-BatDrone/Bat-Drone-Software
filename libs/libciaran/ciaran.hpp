#ifndef CIARAN_HPP
#define CIARAN_HPP
#define rows 60
#define cols 80
#include <stdint.h>

extern uint16_t dx;
extern uint16_t dy;

int detectBlob(uint16_t arr[rows][cols]);

#endif

