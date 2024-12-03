/*
 * analogs.cpp
 *
 * Created: 30/11/2024 12.25.07
 *  Author: pyryl
 */
#include <avr/io.h>
#include "analogs.h"
#include "custom_delay.h"

float read_analog_channel(ANALOG_CHANNELS channel){
	uint16_t conv_res = 0;
	float voltage = 0;
	
	// Start ADC for given channel
	ADC0.MUXPOS = channel;
	ADC0.COMMAND |= (0x1 << ADC_STCONV_bp); // Start conversion
	while(!(ADC0.INTFLAGS & 0x1)); // Wait until conversion is done
	
	//Read the result and convert it to voltage
	
	conv_res = ADC0.RES;
	voltage = (float)conv_res * 5.0 / 1023.0;
	return voltage;
}


/**
* @brief Initializes the adc for use
*/
void ADC0_init(){
	ADC0.CTRLA &= ~(0x1 << ADC_RESSEL_bp); // ADC resolution to 10 bits
	ADC0.CTRLB = 0; // No result accumulation
	
	// Reference voltage to Vdd (5V) and clock prescaler to 16 since ADC max clock is 1,5MHz and the cpu will run 20MHz. This will put ADC clock to about 1,25MHz
	ADC0.CTRLC |= (0x1 << ADC_REFSEL_gp) | (0x3 << ADC_PRESC_gp); 
	
	// Disable digital input buffer and interrupts for pins PD0, PD1 and PD2. These are used for ADC
	PORTD.PIN0CTRL |= (0x4 << PORT_ISC_gp);
	PORTD.PIN1CTRL |= (0x4 << PORT_ISC_gp);
	PORTD.PIN2CTRL |= (0x4 << PORT_ISC_gp);
	
	ADC0.CTRLA |= (0x1 << ADC_ENABLE_bp); //Enable the ADC
}