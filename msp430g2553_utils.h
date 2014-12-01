#include <stdint.h>
#include "msp430g2553.h"

#define delay_20ms() _delay_cycles(20 * 1000)
#define delay_500ms() _delay_cycles((uint32_t)500000)
#define delay_1s() _delay_cycles(1000000)

#define FLASH_START_ADDR 0x0E000

/*
 * Initalize procesor clocks
 * Disable watchdog timer
 */
void processor_init();

/*
 * Erase a flash segment
 */
void flash_erase(uint8_t *addr);

/*
 * Write to a flash segment
 * Required to erase a flash segment before it is written to
 */
void flash_write(uint8_t *addr, uint8_t byte);

/*
 * Delay processor for n * XXXX time
 */
//void delay_cycles(register unsigned int n);
