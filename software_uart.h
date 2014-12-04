#include "msp430g2553_utils.h"


/*
 * Initialize the Software UART for pin P1.5
 */
void softwareUART_init();

/*
 * Transmit a byte using software UART on Pin P1.5
 * Transmits at 19200 baud
 */
void softwareUART_send_byte(uint8_t byte);

/*
 * Transmit an array using software UART on Pin P1.5
 * Transmits at 19200 baud
 */
void softwareUART_send_array(uint8_t* array, uint32_t array_length);
