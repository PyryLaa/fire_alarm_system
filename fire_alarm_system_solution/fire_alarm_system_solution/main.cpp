/*
 * fire_alarm_system_solution.cpp
 *
 * Created: 27.11.2024 14.45.37
 * Author : Mika
 */ 

#define PERIOD_EXAMPLE_VALUE 78
#define CLKOUT_OFF 0
#define CLKOUT_ON 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include "custom_delay.h"
#include "analogs.h"
#include "alarm.h"



/*Using clock 20MHz */

void clock_out(int option);
void pin_setups();
void led_toggle();
void led_on();
void led_off();

int main(void)
{
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
	//start_alarm();
	pin_setups();
	while (true) {
		// Example reading of sensors
		flame_voltage = read_analog_channel(FLAME_CHANNEL);
		co_voltage = read_analog_channel(CO_CHANNEL);
		temp_voltage = read_analog_channel(TEMP_CHANNEL);

		co_value = co_voltage / (5.0 / 1023.0); // Volt to approximate PPM
		temp_value = temp_voltage * 100.0; // Volt to Celsius
		
		led_toggle();
		//delay_ms(600);
		
		if (temp_value > 50 || co_value > 27 || flame_voltage > 1) {
			led_on();
			start_alarm();
		} else {
			led_off();
		}
	}
}

void pin_setups() {
	/* LED pin as OUTPUT */
	VPORTF.DIR |= (1 << 4);
	VPORTF.OUT &= ~(1 << 4);
}

void led_toggle() {
	VPORTF.OUT ^= (1 << 4);
}

void led_on() {
	VPORTF.OUT ^= (1 << 4);
}

void led_off() {
	VPORTF.OUT ^= (1 << 4);
}




// Clock signal to pin PA7
void clock_out(int option) { // PA7
	cli();  // Disable Interrupts
	switch (option) {
		case CLKOUT_ON:
		_PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, CLKCTRL_CLKOUT_bm);
		break;
		case CLKOUT_OFF:
		default:
		_PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, (CLKCTRL_MCLKCTRLA & ~(CLKCTRL_CLKOUT_bm)));
	}
	sei();  // Enable Interrupts
}
