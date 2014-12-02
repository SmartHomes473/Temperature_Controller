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
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	init_temperature_sensor(BIT5);
	fan_t fan = init_fan(BIT4);
	UART_init_9600();
	__bis_SR_register(GIE);

	lcdInit();
	lcdSetText("Desired: ", 0, 0);
	lcdSetText("Current: ", 0, 1);

	uint8_t* device_num_addr = (uint8_t*)FLASH_START_ADDR;

	temp_t desired_temperature = {.temperature = 20, .unit = 0};

	int32_t last_temp = 20;

	while (1)
	{
		int32_t temp;
		if (desired_temperature.unit == 2)
		{
			temp = read_temperature_kelvin();
			lcdSetText("K", 15, 0);
			lcdSetText("K", 15, 1);
		}
		else if (desired_temperature.unit == 1)
		{
			temp = read_temperature_farenheit();
			lcdSetText("F", 15, 0);
			lcdSetText("F", 15, 1);
		}
		else
		{
			temp = read_temperature_celsius();
			lcdSetText("C", 15, 0);
			lcdSetText("C", 15, 1);
		}

		if ((last_temp > 9 && temp <= 9)
			|| (last_temp > 99 && temp <= 99))
		{
			lcdSetText("     ", 9, 1);
		}

		lcdSetInt(desired_temperature.temperature, 9, 0);
		lcdSetInt((int)temp, 9, 1);

		if (UART_data_available() == 1) {
			uint8_t* new_UART_RX = UART_get_data();
			desired_temperature = parse_packet(device_num_addr, new_UART_RX);

			lcdSetText("     ", 9, 0);
		}

		if (temp > desired_temperature.temperature)
		{
			turn_on_fan(fan);
		}
		else
		{
			turn_off_fan(fan);
		}
		last_temp = temp;
	}
}
