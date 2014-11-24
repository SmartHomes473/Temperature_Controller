#include "msp430g2553.h"
#include <stdint.h>


//int main(void)
//{
//	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
//
//	P1DIR |= BIT6;
//
//	// Initialize the ADC10
//	ADC10CTL0 = SREF_0 | ADC10SHT_2 | ADC10ON;  // Vcc reference, 16 clocks, on
//
//	// input channel 5, trigger on ADC10SC bit, no clock division,
//	// internal ADC clock, single channel single conversion
//	ADC10CTL1 = INCH_5 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0 | CONSEQ_0;
//
//	ADC10AE0 = BIT5;    // enable analog input on A5
//	ADC10CTL0 |= ENC;   // enable conversions
//
//	uint16_t temp_mem = 0;
//	float temp_voltage = 0;
//	volatile float temp_celsius = 0;
//
//	while(1)
//	{
//		ADC10CTL0 |= ADC10SC;                   // trigger the conversion
//		while( ADC10CTL1 & ADC10BUSY ) ;        // loop until done
//
//		temp_mem = ADC10MEM;
//
//		/* 10 bit adc - 1024 values. 3.3v reference --> 1024 / 3.3 = 310.3 v/bit */
//		temp_voltage = temp_mem / 310.3f;
//
//		/* 1mV / degree C. 750mV at 25 degrees C */
//		temp_celsius = (100 * temp_voltage) - 50;
//
//		if (temp_celsius > 22)
//		{
//			P1OUT = BIT6;
//		}
//		else
//		{
//			P1OUT = 0;
//		}
//	}
//}

#define DR P1OUT = P1OUT | BIT4 // define RS high
#define CWR P1OUT = P1OUT (~BIT4) // define RS low
#define READ P1OUT = P1OUT | BIT5

// define Read signal R/W = 1 for reading
#define WRITE P1OUT = P1OUT &amp; (~BIT5)

// define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT = P1OUT | BIT6

// define Enable high signal
#define ENABLE_LOW P1OUT = P1OUT &amp; (~BIT6)

// define Enable Low signal
unsigned int i;
unsigned int j;
void delay(unsigned int k)
{
	for(j=0;j<k;j++)
	{
		for(i=0;i<100;i++);
	}
}

void data_write(void)
{
	ENABLE_HIGH;
	delay(2);
	ENABLE_LOW;
}

void data_read(void)
{
	ENABLE_LOW;
	delay(2);
	ENABLE_HIGH;
}

void check_busy(void)
{
	P1DIR &= ~(BIT3); // make P1.3 as input
	while((P1IN & BIT3) == 1)
	{
		data_read();
	}
	P1DIR |= BIT3; // make P1.3 as output
}

void send_command(unsigned char cmd)
{
	check_busy();
	WRITE;
	CWR;
	P1OUT = (P1OUT &amp; 0xF0)|((cmd&gt;&gt;4) &amp; 0x0F); // send higher nibble
	data_write(); // give enable trigger
	P1OUT = (P1OUT &amp; 0xF0)|(cmd &amp; 0x0F); // send lower nibble
	data_write(); // give enable trigger
}

void send_data(unsigned char data)
{
	check_busy();
	WRITE;
	DR;
	P1OUT = (P1OUT &amp; 0xF0)|((data&gt;&gt;4) &amp; 0x0F); // send higher nibble
	data_write(); // give enable trigger
	P1OUT = (P1OUT &amp; 0xF0)|(data &amp; 0x0F); // send lower nibble
	data_write(); // give enable trigger
}

void send_string(char *s)
{
	while(*s)
	{
		send_data(*s);
		s++;
	}
}

void lcd_init(void)
{
	P1DIR |= 0xFF;
	P1OUT &= 0x00;
	send_command(0x33);
	send_command(0x32);
	send_command(0x28); // 4 bit mode
	send_command(0x0E); // clear the screen
	send_command(0x01); // display on cursor on
	send_command(0x06); // increment cursor
	send_command(0x80); // row 1 column 1
}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // stop watchdog timer
	lcd_init();
	send_string("Manpreet Singh");
	send_command(0xC0);
	send_string("Minhas");
	while(1){}
}
