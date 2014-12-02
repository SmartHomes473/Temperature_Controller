#include <stdint.h>

/*
 * Initialize temperature sensor on the given input pin
 */
void init_temperature_sensor(uint8_t input_pin);

/*
 * Return current temperature in Celsius
 */
int32_t read_temperature_celsius();

/*
 * Return current temperature in Farenheit
 */
int32_t read_temperature_farenheit();

/*
 * Return current temperature in Kelvin
 */
int32_t read_temperature_kelvin();
