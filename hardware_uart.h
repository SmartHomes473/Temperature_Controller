#include "msp430g2553_utils.h"

/*
 * Initialze USCI for UART with a 9600 baud rate 
 */
void UART_init_9600();

/*
 * Initialze USCI for UART with a 57600 baud rate
 */
void UART_init_57600();

/*
 * Send a byte over UART
 * Requires UART to be initialized
 */
void UART_send_byte(uint8_t byte);

/*
 * Send an array of bytes over UART
 * Requires UART to be initialzed
 */
void UART_send_array(uint8_t* array, uint32_t array_length);

/*
 * This function returns 1 if there is new UART data available, 0 otherwise
 */
uint8_t UART_data_available();

/*
 * This function returns the most recent UART data
 */
uint8_t UART_get_data();



