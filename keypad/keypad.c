/*
 * keypad.c
 *
 *  Created on: Feb 15, 2021
 *      Author: Youssef
 */
#include "keypad.h"
#if (N_cols == 3)
	static uint8 KEYPAD_4x3_adjustSwitchNumber(uint8 switchNumber);
#else
	static uint8 KEYPAD_4x4_adjustSwitchNumber(uint8 switchNumber);
#endif
uint8 KEYPAD_getPressedKey(void){
	uint8 row,col;
	while(1){
		for(col = 0; col < N_cols; col++){
			KEYPAD_PORT_DIR = (KEYPAD_PORT_DIR & 0) | (1<<(col + 4) );
			KEYPAD_PORT_OUT = (KEYPAD_PORT_OUT | 0xff) & ~(1<<(col +4));
			for(row = 0; row < N_rows; row++){
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN, row)){
					#if (N_cols == 3)
						return KEYPAD_4x3_adjustSwitchNumber((N_cols * row) + (col + 1));
					#else
						return KEPAD_4x4_adjustSwitchNumber((N_cols * row) + (col + 1));
					#endif
				}
			}
		}
	}
}
#if (N_cols == 3)
static uint8 KEYPAD_4x3_adjustSwitchNumber(uint8 switchNumber){
	switch(switchNumber){
	case 10: return '*'; break;
	case 11: return 0;   break;
	case 12: return '#'; break;
	default: return switchNumber; break;
	}
}

#else
static uint8 KEYPAD_4x4_adjustSwitchNumber(uint8 switchNumber){
	switch(switchNumber){
		case 1: case 2: case 3: return switchNumber; break;
		case 4:  return 'A'; break;
		case 5:  return 4;   break;
		case 6:  return 5;   break;
		case 7:  return 6;   break;
		case 8:  return 'B'; break;
		case 9:  return 7;   break;
		case 10: return 8;   break;
		case 11: return 9;   break;
		case 12: return 'C'; break;
		case 13: return '*'; break;
		case 14: return 0;   break;
		case 15: return '#'; break;
		default: return 'D'; break;
	}
}
#endif
