#include "msp430g2553_utils.h"


/******************************************************************************
* Initialize Processor 
******************************************************************************/
void processor_init()
{
	/* Disable watchdog */
	WDTCTL = WDTPW | WDTHOLD;

	/* Set DCO to 1MHz */
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* GPIO for the device detect line */
	P1DIR |= BIT0;
}

/*************************************************************************
 * Erase Information Flash
 *************************************************************************/
void flash_erase(uint8_t* addr)
{
	__disable_interrupt();               // Disable interrupts to make the erase atomic
	while(BUSY & FCTL3);                 // Check if Flash being used
	FCTL2 = FWKEY + FSSEL_1 + FN3;       // Clk = SMCLK/4
	FCTL1 = FWKEY + ERASE;               // Set Erase bit
	FCTL3 = FWKEY;                       // Clear Lock bit

	*addr = 0;                           // Dummy write to erase Flash segment

	while(BUSY & FCTL3);                 // Check if Flash being used
	FCTL1 = FWKEY;                       // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                // Set LOCK bit
	__enable_interrupt();
}


/*************************************************************************
 * Read stored Information flash
 *************************************************************************/
void flash_write(uint8_t* addr, uint8_t byte)
{
	__disable_interrupt();
	FCTL2 = FWKEY + FSSEL_1 + FN0;       // Clk = SMCLK/4
	FCTL3 = FWKEY;                       // Clear Lock bit
	FCTL1 = FWKEY + WRT;                 // Set WRT bit for write operation

	*addr++ = byte;						// copy value to flash

	FCTL1 = FWKEY;                        // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                 // Set LOCK bit
	while(BUSY & FCTL3);
	__enable_interrupt();
}

/******************************************************************************
* Delay for XXXX Amount of time
******************************************************************************/
//void __inline__ delay_cycles(register unsigned int n)
//{
//	__asm__ __volatile__ (
//			"1: \n"
//			" dec	%[n] \n"
//			" jne	1b \n"
//			: [n] "+r"(n));
//}

