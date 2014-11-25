#include "adc.h"
#include "msp430g2553.h"

void init_adc(uint8_t input_pin)
{
	/* Initialize the ADC10 Vcc reference, 16 clocks, on */
	ADC10CTL0 = SREF_0 | ADC10SHT_2 | ADC10ON;

	/* input channel 5, trigger on ADC10SC bit, no clock division,
	 internal ADC clock, single channel single conversion */
	ADC10CTL1 = INCH_5 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0 | CONSEQ_0;

	/* enable analog input on A5 */
	ADC10AE0 = input_pin;

	/* enable conversions */
	ADC10CTL0 |= ENC;
}

uint16_t read_adc()
{
	/* trigger the conversion */
	ADC10CTL0 |= ADC10SC;

	/* loop until done */
	while(ADC10CTL1 & ADC10BUSY);

	return ADC10MEM;
}
