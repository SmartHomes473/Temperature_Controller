#include <stdlib.h>

#include "smrtcontrol_comms.h"
#include "hardware_uart.h"
#include "msp430g2553_utils.h"

#define DEVICE_URL "http://rwooster.com/temperature.tar.gz"

typedef enum
{
	NONE,
	STATUS,
	DEVICE_NUM,
	SIZE,
	SIZE_2,
	DATA,
	END
} parse_state_t;

/******************************************************************************
* Register Roomba device on SMRTControl
******************************************************************************/
void register_device(uint8_t* device_num_addr)
{
	/* Uninitialized flash memory is all 1*/
	if (*device_num_addr == 255)
	{
		/* Register device */
		uint8_t register_array[44] = {PACKET_START_BYTE, 0, 0, 0, 38};
		memcpy(register_array+5, DEVICE_URL, 38);
		register_array[43] = PACKET_END_BYTE;

		UART_send_array(register_array, 44);

		flash_erase(device_num_addr);
		flash_write(device_num_addr, 0);
	}
}

/******************************************************************************
* Parse incoming packet from SMRTControl
******************************************************************************/
temp_t parse_packet(uint8_t* device_num_addr, uint8_t* new_UART_RX)
{
	parse_state_t state = NONE;
	uint8_t data_idx = 0;
	uint8_t device_num;
	uint8_t status;
	char data[10];
	uint16_t packet_size = 1;

	temp_t desired_temperature;

	while(state != END)
	{
		switch(state)
		{
		case NONE:
			if (*new_UART_RX == PACKET_START_BYTE) {
				state = DEVICE_NUM;
			}
			new_UART_RX++;
			break;
		case DEVICE_NUM:
			device_num = *new_UART_RX;
			state = STATUS;
			new_UART_RX++;
			break;
		case STATUS:
			status = *new_UART_RX;
			if (status == 0)
			{
				/* Store device number */
				flash_erase(device_num_addr);
				flash_write(device_num_addr, device_num);
				state = NONE;
			}
			else
			{
				state = SIZE;
			}
			new_UART_RX++;
			break;
		case SIZE:
			packet_size = (*new_UART_RX << 8);
			state = SIZE_2;
			new_UART_RX++;
			break;
		case SIZE_2:
			packet_size |= (*new_UART_RX);

			if (packet_size == 0 && device_num == 0)
			{
				/* Device being deleted */
				flash_erase(device_num_addr);
				state = NONE;
			}
			else
			{
				state = DATA;
			}
			new_UART_RX++;
			break;
		case DATA:
			data[data_idx] = *new_UART_RX;
			data_idx++;
			new_UART_RX++;
			break;
		}

		if (data_idx >= packet_size && state == DATA)
		{
			/* Packet done */
			desired_temperature.unit = data[data_idx-1] - ASCII_OFFSET;
			data[data_idx-1] = 0;
			data_idx = 0;
			state = END;
		}
	}
	state = NONE;
	desired_temperature.temperature = atoi(data);

	return desired_temperature;
}


