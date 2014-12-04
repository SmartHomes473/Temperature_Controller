#include "software_uart.h"


#define TXD BIT3
#define BIT_TIME 139 /* 1MHz / 19200 baud = 52 */

static uint8_t bit_count;
static uint32_t transmit_byte;

/******************************************************************************
* Initialze the software UART
******************************************************************************/
void softwareUART_init()
{
	P1DIR |= TXD;

	/* enable interrupts */
	__bis_SR_register(GIE);
}

/******************************************************************************
* ISR for Timer_A - used to create the software UART send
******************************************************************************/
/*static void
__attribute__((__interrupt__(TIMER0_A0_VECTOR)))
isr_timerA(void)
*/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER_ISR(void)
{
	CCR0 += BIT_TIME;

	/* If all bits TXed, disable interrupt */
	if (bit_count == 0)
		CCTL0 &= ~ CCIE ;
	else
	{
		/* Transmit bit */
		if (transmit_byte & 0x01)
			P1OUT |= TXD;
		else
			P1OUT &= ~TXD;
		transmit_byte = transmit_byte >> 1;
		bit_count--;
	}
}

/******************************************************************************
* Send a byte over the software UART
******************************************************************************/
void softwareUART_send_byte(uint8_t byte)
{
	CCTL0 = OUT;

	/* Source timer from SMCLK and set continuous mode */
	TACTL = TASSEL_2 + MC_2;

	/* Sending 8 bits + start bit + stop bit = 10 bits */
	bit_count = 10;

	/* Set compare reg to the current timer value + time for 1 bit */
	CCR0 = TAR + BIT_TIME;

	/* Setup transmission data */
	transmit_byte = byte;
	transmit_byte |= 0x100;
	transmit_byte = transmit_byte << 1;

	/* Set signal, intial value, enable interrupts */
	CCTL0 =  CCIS0 + OUTMOD0 + CCIE;

	while ( CCTL0 & CCIE )
	{
		/* Wait for TX */
	}

	/* Disable timer */
	TACTL = TASSEL_2;
}


/******************************************************************************
* Send an array of bytes over the software UART
******************************************************************************/
void softwareUART_send_array(uint8_t* array, uint32_t array_length)
{
	while (array_length--)
	{
		softwareUART_send_byte(*array);
		array++;
	}
}
