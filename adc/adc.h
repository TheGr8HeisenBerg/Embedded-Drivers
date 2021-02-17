/*
 * adc.h
 *
 *  Created on: Feb 17, 2021
 *      Author: JIGamer
 */

#ifndef ADC_H_
#define ADC_H_



#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*****************************************************************************
 * ADC_REFERENCE Values														 *
 * 0 -> AREF, Internal Vref turned off										 *
 * 1 -> AVCC with external capacitor at AREF pin							 *
 * 2 -> Reserved															 *
 * 3 -> Internal 2.56V Voltage Reference with external capacitor at AREF pin *
 *****************************************************************************/
#define ADC_REFERENCE 0

void ADC_init();
uint16 ADC_readChannel(uint8);

#endif /* ADC_H_ */
