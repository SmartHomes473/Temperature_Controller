//#include <stdint.h>
//
#include "msp430g2553.h"
//#include "temperature_sensor.h"
//#include "fan.h"
//#include "i2c.h"
//
//
//int main(void)
//{
//
//	init_i2c(0x1111);
//
//	uint8_t stuff[] = {0x01};
//	i2c_transmit_byte(stuff, 1);
//
//	while (1)
//	{
//	}
//	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
//
//	init_temperature_sensor(BIT5);
//	fan_t fan = init_fan(BIT4);
//
//	while (1)
//	{
//		float temp_celsius = read_temperature_celsius();
//
//		if (temp_celsius > 22)
//		{
//			turn_on_fan(fan);
//		}
//		else
//		{
//			turn_off_fan(fan);
//		}
//	}
//}
int main()
{
	WDTCTL = WDTPW + WDTHOLD;

    // Set up a 16MHz main clock
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;



   /************
    *I2C CONFIG*
    ************/

    // Set master mode, I2C operation, and synchronous mode
    UCB0CTL0 = UCMST + UCMODE1 + UCMODE0 + UCSYNC;

    UCB0CTL1 |= UCSSEL0 + UCSSEL1; // Use SMCLK
    UCB0BR0 = 160; // Set 100kHz baud rate
    UCB0BR1 = 0;

    UCB0I2CSA = (0x7C>>1); // Set the slave address for the nunchuck

    // Connect the SDA/SCL pins to the I2C module
    P1SEL  |= BIT7 + BIT6;
    P1SEL2 |= BIT7 + BIT6;

    UCB0CTL1 &= ~UCSWRST; // Enable the USCI
    UCB0CTL1 |= UCTR; // Set transmit mode
    UCB0CTL1 |= UCTXSTT; // Send START + slave address

    while (UCB0CTL1 & UCTXSTT);

    return 0;
}
