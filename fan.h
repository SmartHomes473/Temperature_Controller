#include <stdint.h>

typedef struct
{
	uint8_t fan_pin;
} fan_t;

fan_t init_fan(uint8_t output_pin);

void turn_on_fan(fan_t fan);

void turn_off_fan(fan_t fan);

