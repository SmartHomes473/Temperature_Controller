#include <stdint.h>

#include "msp430g2553.h"
#include "temperature_sensor.h"
#include "fan.h"


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer


	init_temperature_sensor(BIT5);
	fan_t fan = init_fan(BIT6);

	while (1)
	{
		float temp_celsius = read_temperature_celsius();

		if (temp_celsius > 22)
		{
			turn_on_fan(fan);
		}
		else
		{
			turn_off_fan(fan);
		}
	}
}

