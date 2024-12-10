/*
 * alarm.cpp
 *
 * Created: 10/12/2024 10.29.39
 *  Author: pyryl
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "alarm.h"
#include "custom_delay.h"

#define PWM_CCMP_VALUE 0x1428
volatile uint8_t run_alarm = 1;

/**
* @brief Initializes TCB0 for PWM use and configures PA1 for warning LED and PC5 as reset button input
*/
void TCB0_init(){
	VPORTA.DIR |= (0x1 << PIN2_bp); // Make pin PA2 as output for PWM output
	VPORTA.OUT &= ~(0x1 << PIN2_bp); // Make sure PA2 is low
	PORTA.PIN2CTRL |= (0x4 << PORT_ISC_gp); // Disable input and interrupts for pin A2
	TCB0.CTRLB |= (0x1 << TCB_CCMPEN_bp) | (0x7 << TCB_CNTMODE_gp); // Enable capture compare output for TCB and count mode as 8 bit PWM
	TCB0.CTRLA |= (0x2 << TCB_CLKSEL_gp); // Use TCA0 clock for TCB too
	TCB0.CCMP = PWM_CCMP_VALUE; // Write period and duty cycle values to CCMP register. Period 0x4E and duty cycle 0x28
	
	VPORTA.DIR |= (0x1 << PIN1_bp); // Pin PA1 as output for led
	VPORTA.OUT &= ~(0x1 << PIN1_bp); // Make sure PA1 is low
	
	VPORTC.DIR &= ~(0x1 << PIN5_bp); // Pin PC5 as input
	PORTC.PIN5CTRL |= (0x3 << PORT_ISC_gp) | (0x1 << PORT_PULLUPEN_bp); // Interrupt on falling edge and pull-up enable for PC5 
	
}

/**
* @brief Starts the alarm, PWM to speaker and LED on
*/
void start_alarm(){
	run_alarm = 1;
	while(run_alarm){
		TCB0.CTRLA |= (1 << TCB_ENABLE_bp); // Enable TCB0 PWM
		VPORTA.OUT |= (1 << PIN1_bp); // Enable warning led
		delay_ms(500);
		VPORTA.OUT &= ~(1 << PIN1_bp); // Disable warning led
		TCB0.CTRLA &= ~(1 << TCB_ENABLE_bp); // Disable TCB0 PWM
		delay_ms(500);
	}
}

ISR(PORTC_PORT_vect){ // Interrupt handler for PORTC external interrupt (alarm reset button)
	cli(); // Disable future interrupts
	run_alarm = 0;
	PORTC.INTFLAGS |= (1 << PIN5_bp); // Clear PC5 interrupt flag
	sei(); // Enable interrupts back
}