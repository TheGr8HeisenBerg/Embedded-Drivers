/*
 * adc.c
 *
 *  Created on: Feb 17, 2021
 *      Author: JIGamer
 */


#include "adc.h"

void ADC_init(){
	ADMUX = (ADC_REFERENCE<<6);
	ADCSRA = (1<<ADEN);
	#ifdef F_CPU
		#if (F_CPU == 16000000UL)
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
		#elif (F_CPU == 8000000UL)
			ADCSRA |= (1<<ADPS1) | (1<<ADPS2);
		#elif (F_CPU == 1000000UL)
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1);
		#endif
	#else
	    ADCSRA |= (1<<ADPS0) | (1<<ADPS1);
	#endif
}

uint16 ADC_readChannel(uint8 channelNumber){
	ADMUX = (ADMUX & 0xE0) | (channelNumber & 0x07);
	ADCSRA |= (1<<ADSC);
	while(BIT_IS_CLEAR(ADCSRA, ADIF));
	SET_BIT(ADCSRA, ADIF);
	return ADC;
}

