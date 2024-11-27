/*
 * custom_delay.cpp
 *
 * Created: 27.11.2024 15.03.06
 *  Author: Mika
 */ 

/*Using clock 20MHz */

#define PERIOD_EXAMPLE_VALUE 78 // 1 ms resolution, about

#include <avr/io.h>
#include <avr/interrupt.h>
#include "custom_delay.h"

volatile uint16_t ms_counter = 0;


void TCA0_init()
{
	// clear main clock divider, 6 is default, 0 means it'll just be 20 MHz
	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0);
	
	/* enable overflow interrupt */
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

	/* set Normal mode */
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;

	/* disable event counting */
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);

	/* set the period */
	TCA0.SINGLE.PER = PERIOD_EXAMPLE_VALUE;

	/* set clock divider to 256 (sys_clk/256) */
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV256_gc;
}

void delay_ms(const int ms_time) {
	/* start time */
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
	
	while (ms_counter < ms_time) {
	}
	
	/* stop time */
	TCA0.SINGLE.CTRLA &= ~(1 << TCA_SINGLE_ENABLE_bp);
	
	ms_counter = 0;
	
}


ISR(TCA0_OVF_vect)
{
	ms_counter++;

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
