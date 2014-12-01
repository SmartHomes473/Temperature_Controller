#include <stdint.h>

#include "msp430g2553.h"
#include "lcdLib.h"
#include "temperature_sensor.h"
#include "fan.h"
#include "i2c.h"


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	init_temperature_sensor(BIT5);
	fan_t fan = init_fan(BIT4);

	lcdInit();
	lcdSetText("Desired: ", 0, 0);
	lcdSetText("Current: ", 0, 1);


	int16_t desired_temperature = 20;
	while (1)
	{
		float temp_celsius = read_temperature_celsius();

		if (temp_celsius > desired_temperature)
		{
			turn_on_fan(fan);
		}
		else
		{
			turn_off_fan(fan);
		}
		lcdSetInt(desired_temperature, 9, 0);
		lcdSetInt((int)temp_celsius, 9, 1);
	}
}
