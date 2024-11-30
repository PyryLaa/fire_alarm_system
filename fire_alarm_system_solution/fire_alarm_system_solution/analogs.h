/*
 * analogs.h
 *
 * Created: 30/11/2024 12.25.22
 *  Author: pyryl
 */ 


#ifndef ANALOGS_H_
#define ANALOGS_H_

enum ANALOG_CHANNELS{
	FLAME_CHANNEL = 0,
	CO_CHANNEL,
	TEMP_CHANNEL
};

float read_analog_channel(ANALOG_CHANNELS channel);
void ADC0_init();



#endif /* ANALOGS_H_ */