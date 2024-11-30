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



/*Using clock 20MHz */

void clock_out(int option);
void pin_setups();
void led_toggle();

int main(void)
{
	float flame_v = 0, co_v = 0, temp_v = 0;
	TCA0_init();
	ADC0_init();

	/* enable global interrupts, just in case */
	sei();
	
	pin_setups();
	while (true) {
		// Example reading of sensors
		flame_v = read_analog_channel(FLAME_CHANNEL);
		co_v = read_analog_channel(CO_CHANNEL);
		temp_v = read_analog_channel(TEMP_CHANNEL);
		led_toggle();
		delay_ms(600);
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
