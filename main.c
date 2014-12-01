#include <stdint.h>

#include "msp430g2553.h"
#include "hardware_uart.h"
#include "lcdLib.h"
#include "temperature_sensor.h"
#include "fan.h"
#include "smrtcontrol_comms.h"


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	init_temperature_sensor(BIT5);
	fan_t fan = init_fan(BIT4);
	UART_init_9600();

	lcdInit();
	lcdSetText("Desired: ", 0, 0);
	lcdSetText("Current: ", 0, 1);

	uint8_t* device_num_addr = (uint8_t*)FLASH_START_ADDR;

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

		if (UART_data_available() == 1) {
			uint8_t* data = parse_packet(device_num_addr);

			if (data != 0)
			{
				desired_temperature = *data;
			}
		}
	}
}
