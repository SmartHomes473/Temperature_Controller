#include <stdint.h>

#include "i2c.h"
#include "msp430g2553.h"


static uint8_t tx_byte_count = 0;
static uint8_t tx_total_bytes;
static uint8_t* tx_bytes;

void init_i2c(uint16_t address)
{
	/* Assign I2C pins to USCI_B0 */
	P1SEL |= BIT6 + BIT7;
	P1SEL2|= BIT6 + BIT7;

	/* Enable SW reset */
	UCB0CTL1 |= UCSWRST;

	/* I2C Master, synchronous mode */
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;

	/* Use SMCLK, keep SW reset */
	UCB0CTL1 = UCSSEL_2 + UCSWRST;

	/* fSCL = SMCLK/12 = ~100kHz */
	UCB0BR0 = 12;
	UCB0BR1 = 0;

	UCB0I2CSA = address;

	/* Clear SW reset, resume operation */
	UCB0CTL1 &= ~UCSWRST;

	/* Enable RX and TX interrupt */
	IE2 |= UCB0RXIE + UCB0TXIE;
}

void i2c_transmit_byte(uint8_t* bytes, uint16_t num_bytes)
{
	/* Ensure stop condition got sent */
	while (UCB0CTL1 & UCTXSTP);

	/* I2C TX, start condition */
	UCB0CTL1 |= UCTR + UCTXSTT;

	tx_bytes = bytes;
	tx_total_bytes = num_bytes;
	tx_byte_count = 0;

	__bis_SR_register(CPUOFF + GIE);
}

//#pragma vector = USCIAB0TX_VECTOR
//__interrupt void USCIAB0TX_ISR(void)
//{
////  if(Rx == 1){                              // Master Recieve?
////      PRxData = UCB0RXBUF;                       // Get RX data
////      __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
////  }
////
////  else{                                     // Master Transmit
//  if (tx_byte_count < tx_total_bytes)                            // Check TX byte counter
//	{
//	  UCB0TXBUF = tx_bytes[tx_byte_count];                     // Load TX buffer
//	  tx_byte_count++;                            // Decrement TX byte counter
//	}
//	else
//	{
//	  UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
//	  IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
//	  __bic_SR_register_on_exit(CPUOFF);
//	}
// }


