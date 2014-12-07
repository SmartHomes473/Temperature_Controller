#include <stdint.h>

#include "msp430g2553.h"
#include "hardware_uart.h"
#include "lcdLib.h"
#include "temperature_sensor.h"
#include "fan.h"
#include "smrtcontrol_comms.h"

void itoa(long unsigned int value, char* result, int base)
{
      // check that the base if valid
      if (base < 2 || base > 36) { *result = '\0';}

      char* ptr = result, *ptr1 = result, tmp_char;
      int tmp_value;

      do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
      } while ( value );

      // Apply negative sign
      if (tmp_value < 0) *ptr++ = '-';
      *ptr-- = '\0';
      while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
      }

}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	init_temperature_sensor(BIT5);
	fan_t fan = init_fan(BIT4);
	UART_init_9600();
	__bis_SR_register(GIE);


	uint8_t* device_num_addr = (uint8_t*)FLASH_START_ADDR;

	temp_t desired_temperature = {.temperature = 20, .unit = 0};

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

//		uint8_t temp_array[10] = {0x0F, 0x05, 0x03, 0x00};
//		char temp_str[3];
//		itoa(temp, temp_str, 10);
//
//		if (temp < 9)
//		{
//			temp_array[4] = 0x01;
//			memcpy(temp_array+5, temp_str, 1);
//			temp_array[6] = 0x04;
//			UART_send_array(temp_array, 7);
//		}
//		else if (temp < 99)
//		{
//			temp_array[4] = 0x02;
//			memcpy(temp_array+5, temp_str, 2);
//			temp_array[7] = 0x04;
//			UART_send_array(temp_array, 8);
//		}
//		else
//		{
//			temp_array[4] = 0x02;
//			memcpy(temp_array+5, temp_str, 3);
//			temp_array[8] = 0x04;
//			UART_send_array(temp_array, 9);
//		}


		if (UART_data_available() == 1) {
			uint8_t* new_UART_RX = UART_get_data();
			temp_t new_desired_temperature = parse_packet(device_num_addr, new_UART_RX);

			if (new_desired_temperature.temperature != INT32_MAX)
			{
				desired_temperature = new_desired_temperature;
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
