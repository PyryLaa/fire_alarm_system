/*
 * fire_alarm_system_solution.cpp
 *
 * Created: 27.11.2024 14.45.37
 * Author : Mika
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "custom_delay.h"
#include "analogs.h"
#include "alarm.h"

int main(void){
	float flame_voltage = 0;
	float co_voltage = 0;
	float temp_voltage = 0;

	float co_value = 0;
	float temp_value = 0;

	TCA0_init();
	ADC0_init();
	TCB0_init();

	/* enable global interrupts, just in case */
	sei();
	while (true) {
		// Reading sensor and trigger alarm if conditions are met
		flame_voltage = read_analog_channel(FLAME_CHANNEL);
		co_voltage = read_analog_channel(CO_CHANNEL);
		temp_voltage = read_analog_channel(TEMP_CHANNEL);

		co_value = co_voltage / (5.0 / 1023.0); // Volt to approximate PPM
		temp_value = temp_voltage * 100.0; // Volt to Celsius
		
		if (temp_value > 50 || co_value > 27 || flame_voltage > 1) {
			start_alarm();
		}
	}
}