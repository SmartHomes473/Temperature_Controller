#include "fan.h"
#include "msp430g2553.h"

fan_t init_fan(uint8_t output_pin)
{
	P1DIR |= output_pin;

	fan_t fan = {.fan_pin = output_pin};

	return fan;
}

void turn_on_fan(fan_t fan)
{
	P1OUT |= fan.fan_pin;
}

void turn_off_fan(fan_t fan)
{
	P1OUT &= ~(fan.fan_pin);
}



