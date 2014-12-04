#include "software_uart.h"
#include <stdio.h>


void init_lcd()
{
	softwareUART_init();
	_delay_cycles(50000);
}

void set_cursor_home()
{
	softwareUART_send_byte(0x7C);
	softwareUART_send_byte(0x18);
	softwareUART_send_byte(0);

	softwareUART_send_byte(0x7C);
	softwareUART_send_byte(0x19);
	softwareUART_send_byte(0);
}

void set_cursor_x(uint8_t x)
{
	softwareUART_send_byte(0x7C);
	softwareUART_send_byte(0x18);
	softwareUART_send_byte(x);
}
void set_cursor_y(uint8_t y)
{
	softwareUART_send_byte(0x7C);
	softwareUART_send_byte(0x19);
	softwareUART_send_byte(y);
}

void set_text(char* text, uint8_t len, uint8_t x, uint8_t y)
{
	set_cursor_x(x);
	set_cursor_y(y);

	softwareUART_send_array(text, len);
	_delay_cycles(1000000);
}

void set_int(int num, uint8_t x, uint8_t y)
{
	_delay_cycles(50000000);
	char number_string[16];
	sprintf(number_string, "%d", num);

	int len = 0;
	int i;
	for (i = 0; i < 16; i++)
	{
		len++;
		if (number_string[i] == 0)
			break;
	}
	set_text(number_string, len, x, y);
}


void clear_screen()
{
	softwareUART_send_byte(0x7C);
	softwareUART_send_byte(0x00);
}
