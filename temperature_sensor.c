#include "temperature_sensor.h"
#include "adc.h"

void init_temperature_sensor(uint8_t input_pin)
{
	init_adc(input_pin);
}

int32_t read_temperature_celsius()
{
	uint16_t temp_mem = read_adc();

	/* 10 bit adc - 1024 values. 3.3v reference --> 1024 / 3.3 = 310.3 v/bit */
	float temp_voltage = temp_mem / 310.3f;

	/* 1mV / degree C. 750mV at 25 degrees C */
	float temp_celsius = (100 * temp_voltage) - 50;

	return (int32_t)temp_celsius;
}

int32_t read_temperature_farenheit()
{
	float celsius = read_temperature_celsius();

	float farenheit = ((celsius * 9)/5) + 32;
	return (int32_t)farenheit;
}

int32_t read_temperature_kelvin()
{
	float celsius = read_temperature_celsius();

	float kelvin = celsius + 273.15;
	return (int32_t)kelvin;
}
