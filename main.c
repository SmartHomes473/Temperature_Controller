#include <stdint.h>

#include "msp430g2553.h"
#include "hardware_uart.h"
#include "temperature_sensor.h"
#include "fan.h"
#include "smrtcontrol_comms.h"
#include "graphics_lcd.h"


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

//	int i = 0;
//	for (i = 0; i < 18; i++)
//	{
//		delay_1s();
//	}

	UART_init_9600();
	__bis_SR_register(GIE);

	init_temperature_sensor(BIT5);
	fan_t fan = init_fan(BIT4);

//	init_lcd();
//	clear_screen();

	char des_temp_text[] = "Desired Temperature: ";
	char cur_temp_text[] = "Current Temperature: ";

//	set_text(des_temp_text, 21, 5, 60);
//	set_text(cur_temp_text, 21, 5, 75);
//	set_text("Celsius", 7, 60, 10);

	uint8_t* device_num_addr = (uint8_t*)FLASH_START_ADDR;
	//register_device(device_num_addr);

	temp_t desired_temperature = {.temperature = 20, .unit = 0};

//	set_int(desired_temperature.temperature, 140, 60);


	while (1)
	{
		int32_t temp;

		if (desired_temperature.unit == 2)
		{
			temp = read_temperature_kelvin();
		}
		else if (desired_temperature.unit == 1)
		{
			temp = read_temperature_farenheit();
		}
		else
		{
			temp = read_temperature_celsius();
		}

//		set_int((int)temp, 140, 75);

		if (UART_data_available() == 1) {
			uint8_t* new_UART_RX = UART_get_data();

			temp_t last_des_temp = desired_temperature;
			desired_temperature = parse_packet(device_num_addr, new_UART_RX);

			if (last_des_temp.unit != desired_temperature.unit)
			{
//				if (desired_temperature.unit == 2)
//				{
//					set_text("Kelvin", 6, 50, 10);
//				}
//				else if (desired_temperature.unit == 1)
//				{
//					set_text("Farenheit", 9, 50, 10);
//				}
//				else
//				{
//					set_text("Celsius", 7, 50, 10);
//				}
			}
			if (last_des_temp.temperature != desired_temperature.temperature)
			{
//				set_int(desired_temperature.temperature, 140, 60);
			}
		}

		if (temp > desired_temperature.temperature)
		{
			turn_on_fan(fan);
		}
		else
		{
			turn_off_fan(fan);
		}
	}
}
