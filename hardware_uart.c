#include "hardware_uart.h"
#include "software_uart.h"
#include "roomba.h"

#define BAUD_9600 104
#define BAUD_57600 1

/******************************************************************************
* Initialize UART at 9600 Baud
******************************************************************************/
void UART_init_9600()
{
	/* Set P1.1 = RX and P1.2 = TX */
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;

	/* Set USCI to use the SMCLK */
	UCA0CTL1 |= UCSSEL_2;

	/* Set Baud rate to 9600 */
	UCA0BR0 = BAUD_9600;
	UCA0BR1 = 0;

	/* Set modulation */
	UCA0MCTL = UCBRS0;

	/* Reset UART state machine */
	UCA0CTL1 &= ~UCSWRST;

	/* Enable interrupt on UART channel 1 receive */
	IE2 |= UCA0RXIE;
}

/******************************************************************************
* Initialize UART at 57600 Baud
******************************************************************************/
void UART_init_57600()
{
	/* Set P1.1 = RX and P1.2 = TX */
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;

	/* Set USCI to use the SMCLK */
	UCA0CTL1 |= UCSSEL_2;

	/* Set Baud rate to 9600 */
	UCA0BR0 = BAUD_57600;
	UCA0BR1 = 0;

	/* Set modulation */
	UCA0MCTL = 0x0F;

	/* Reset UART state machine */
	UCA0CTL1 &= ~UCSWRST;

	/* Enable interrupt on UART channel 1 receive */
	IE2 |= UCA0RXIE;
}

/******************************************************************************
* Send byte over UART
******************************************************************************/
void UART_send_byte(uint8_t byte)
{
	while(!(IFG2 & UCA0TXIFG)) {
		/* Wait for TX buffer to be ready */
	}

	UCA0TXBUF = byte;
}

/******************************************************************************
* Send array of bytes over UART
******************************************************************************/
void UART_send_array(uint8_t* array, uint32_t array_length)
{
	while (array_length--) {
		UART_send_byte(*array);
		array++;
	}
}

static volatile uint8_t new_UART_RX = 0;
static uint8_t new_UART_present = 0;
/******************************************************************************
* ISR for USCI A0 Receive
******************************************************************************/
/*
static void
__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
isr_USCI_RX(void)
*/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	new_UART_RX = UCA0RXBUF;
	new_UART_present = 1;
}


/******************************************************************************
* This function returns 1 if there is new UART data available, 0 otherwise
******************************************************************************/
uint8_t UART_data_available()
{
	return new_UART_present;
}

/******************************************************************************
* This function returns the most recent UART data
******************************************************************************/
uint8_t UART_get_data()
{
	new_UART_present = 0;
	return new_UART_RX;
}
