#include "smrtcontrol_comms.h"
#include "hardware_uart.h"
#include "msp430g2553_utils.h"

#define DEVICE_URL "http://rwooster.com/roomba.tar.gz"

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
		uint8_t register_array[39] = {PACKET_START_BYTE, 0, 0, 0, 33};
		memcpy(register_array+5, DEVICE_URL, 33);
		register_array[38] = PACKET_END_BYTE;

		UART_send_array(register_array, 39);

		flash_erase(device_num_addr);
		flash_write(device_num_addr, 0);
	}
}

/******************************************************************************
* Parse incoming packet from SMRTControl
******************************************************************************/
uint8_t* parse_packet(uint8_t* device_num_addr)
{
	static parse_state_t state = NONE;
	static uint8_t data_idx = 0;
	static uint8_t device_num;
	static uint8_t status;
	static uint8_t data[10];
	static uint16_t packet_size = 1;
	uint8_t new_UART_RX;

	new_UART_RX = UART_get_data();
	switch(state)
	{
	case NONE:
		if (new_UART_RX == PACKET_START_BYTE) {
			state = DEVICE_NUM;
		}
		break;
	case DEVICE_NUM:
		device_num = new_UART_RX;
		state = STATUS;
		break;
	case STATUS:
		status = new_UART_RX;
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
		break;
	case SIZE:
		packet_size = (new_UART_RX << 8);
		state = SIZE_2;
		break;
	case SIZE_2:
		packet_size |= (new_UART_RX);

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
		break;
	case DATA:
		data[data_idx] = new_UART_RX;
		data_idx++;
		break;
	case END:
		state = NONE;
		break;
	}

	if (data_idx >= packet_size && state == DATA)
	{
		/* Packet done */
		data_idx = 0;
		state = END;

		return data;
	}

	return 0;
}


