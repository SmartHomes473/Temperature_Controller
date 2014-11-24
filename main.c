#include "msp430g2553.h"
#include <stdint.h>


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P1DIR |= BIT6;

	// Initialize the ADC10
	ADC10CTL0 = SREF_0 | ADC10SHT_2 | ADC10ON;  // Vcc reference, 16 clocks, on

	// input channel 5, trigger on ADC10SC bit, no clock division,
	// internal ADC clock, single channel single conversion
	ADC10CTL1 = INCH_5 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0 | CONSEQ_0;

	ADC10AE0 = BIT5;    // enable analog input on A5
	ADC10CTL0 |= ENC;   // enable conversions

	uint16_t temp_mem = 0;
	float temp_voltage = 0;
	volatile float temp_celsius = 0;

	while(1)
	{
		ADC10CTL0 |= ADC10SC;                   // trigger the conversion
		while( ADC10CTL1 & ADC10BUSY ) ;        // loop until done

		temp_mem = ADC10MEM;

		/* 10 bit adc - 1024 values. 3.3v reference --> 1024 / 3.3 = 310.3 v/bit */
		temp_voltage = temp_mem / 310.3f;

		/* 1mV / degree C. 750mV at 25 degrees C */
		temp_celsius = (100 * temp_voltage) - 50;

		if (temp_celsius > 22)
		{
			P1OUT = BIT6;
		}
		else
		{
			P1OUT = 0;
		}
	}
}
