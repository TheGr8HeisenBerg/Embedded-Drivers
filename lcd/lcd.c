/*
 * lcd.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Youssef
 */
#include "lcd.h"
/**************************************
 * 		  							  *
 *    Static Functions Prototypes	  *
 * 		                              *
 **************************************/

static void LCD_commandEnable(void);
static void LCD_commandDisable(void);
static void LCD_dataEnable(void);
static void LCD_dataDisable(void);

/*******************************************
 * 										   *
 *        Functions Definitions            *
 * 										   *
 *******************************************/

void LCD_init(void){
	LCD_control_port_dir |= (1<<RS) | (1<<RW) | (1<<EN); /* Make the 3 control pins output */
 	#if (LCD_mode == 8)
		LCD_data_port_dir = 0xff; /* make the 8 data pins output */
		LCD_sendCommand(CMD_LCD_TWO_LINES_EIGHT_BIT_MODE); /* 2 lines 8-bit mode*/
	#else
		#ifdef upperBits
			LCD_data_port_dir |= 0xf0; /* make the upper 4 data pins output */
		#else
			LCD_data_port_dir |= 0x0f; /* make the lower 4 data pins output */
		#endif
			LCD_sendCommand(CMD_RETURN_HOME);
			LCD_sendCommand(CMD_LCD_TWO_LINES_FOUR_BIT_MODE); /*2 lines 4-bit mode */
	#endif
	LCD_cursorOff();   /*Turn on screen and turn off the cursor*/
	LCD_displayString("LCD Initialized!"); /*Print LCD Initialized on screen for a second and then clear it*/
	_delay_ms(1000);   /*keep the displayed string for 1 second*/
	LCD_clearScreen(); /*Clear the screen*/
}


void LCD_sendCommand(uint8 a_command){
	/*Command is sent in between LCD_commandEnable() and LCD_commandDisable()*/
	LCD_commandEnable();
	#if (LCD_mode == 8)
		LCD_data_port_out = a_command; /*writing the command to the data pins*/
	#else
		#ifdef upperBits
			LCD_data_port_out = (LCD_data_port_out & 0x0f) | (a_command & 0xf0);
			LCD_commandDisable();
			LCD_commandEnable();
			LCD_data_port_out = (LCD_data_port_out & 0x0f) | ((a_command & 0x0f)<<4);

		#else
			LCD_data_port_out = (LCD_data_port_out & 0xf0) | ((a_command & 0xf0)>>4);
			LCD_commandDisable();
			LCD_commandEnable();
			LCD_data_port_out = (LCD_data_port_out & 0xf0) | (a_command & 0x0f);
		#endif
	#endif
	LCD_commandDisable();
}

void LCD_displayCharacter(const uint8 a_characterToDisplay){
	/*Character is sent in between LCD_dataEnable() and LCD_dataDisable()*/
	LCD_dataEnable();
	#if (LCD_mode == 8)
		LCD_data_port_out = a_characterToDisplay; /*writing the character/data to the data pins*/
	#else
		#ifdef upperBits
			LCD_data_port_out = (LCD_data_port_out & 0x0f) | (a_characterToDisplay & 0xf0);
			LCD_dataDisable();
			LCD_dataEnable();
			LCD_data_port_out = (LCD_data_port_out & 0x0f) | ((a_characterToDisplay & 0x0f)<<4);
		#else
			LCD_data_port_out = (LCD_data_port_out & 0xf0) | ((a_characterToDisplay & 0xf0)>>4);
			LCD_dataDisable();
			LCD_dataEnable();
			LCD_data_port_out = (LCD_data_port_out & 0xf0) | (a_characterToDisplay & 0x0f);
		#endif
	#endif
    LCD_dataDisable();
}

void LCD_displayString(const sint8 * a_stringToDisplay){
	/*looping through the string until we find the NULL terminator*/
	while((*a_stringToDisplay) != '\0'){
		LCD_displayCharacter((*a_stringToDisplay)); /*display the character on the LCD*/
		a_stringToDisplay++; /*increase the pointer to go to the next character*/
	}
}

void LCD_goToRowColumn(uint8 a_row, uint8 a_col){
	uint8 address = 0;
	switch(a_row){
	case 0: /*first row*/
		address = a_col;
		break;
	case 1: /*second row*/
		address = a_col+0x40;
		break;
	case 2: /*third row*/
		address = a_col+0x10;
		break;
	case 3: /*fourth row*/
		address = a_col+0x50;
		break;
	}
	LCD_sendCommand(address|(1<<7));
}

void LCD_displayStringRowColumn(const sint8 * a_stringToDisplay, uint8 a_row, uint8 a_col){
	/*first go to the location*/
	LCD_goToRowColumn(a_row,a_col);
	/*second display the string*/
	LCD_displayString(a_stringToDisplay);
}

void LCD_integerToString(sint32 a_numberToDisplay){
	char buffer[12];
	/*************************************************************************************
	 * Why 12?																			 *
	 * largest number of 32 bit number is 4,294,967,296	                                 *
	 * meaning there are 10 digits in that number and if there is a negative sign (-)    *
	 * that would be a total of 11 and for the null terminator (\0)                      *
	 * that is a total of 12 															 *
	 *************************************************************************************/
	uint8 i = 0, j = 0;
	uint8 isNegative = 0;
	if(a_numberToDisplay == 0){
		buffer[0] = '0';
		buffer[1] = '\0';
		return LCD_displayString(buffer); /*display the zero on the LCD and return from the function*/
	}
	while(a_numberToDisplay != 0){
		/*Negative number check*/
		if(a_numberToDisplay < 0){
			/*it's a negative number so add the sign*/
			isNegative = 1;
			buffer[i++] = '-';
			a_numberToDisplay = 0 - a_numberToDisplay; /*making it a positive number*/
		}
		buffer[i++] = (a_numberToDisplay % 10) + '0';
		a_numberToDisplay /= 10;
	}
	buffer[i--] = '\0';

	/*Now we have the number but it's reversed*/
	for(j = 0; j < i; j++){
		if(isNegative){
			/*Skip the sign*/
			isNegative = 0;
			continue;
		}
		/*Swap the numbers so you reverse the buffer back to normal*/
		buffer[j] = buffer[j] ^ buffer[i];
		buffer[i] = buffer[j] ^ buffer[i];
		buffer[j] = buffer[j] ^ buffer[i];
		i--;
	}
	LCD_displayString(buffer);
}

static void LCD_commandEnable(void){
	CLEAR_BIT(LCD_control_port_out, RS);
	CLEAR_BIT(LCD_control_port_out, RW);
	_delay_ms(1);
	SET_BIT(LCD_control_port_out, EN);
	_delay_ms(1);
}

static void LCD_dataEnable(void){
	SET_BIT(LCD_control_port_out, RS);
	CLEAR_BIT(LCD_control_port_out, RW);
	_delay_ms(1);
	SET_BIT(LCD_control_port_out, EN);
	_delay_ms(1);
}
static void LCD_dataDisable(void){
	_delay_ms(1);
	CLEAR_BIT(LCD_control_port_out, EN);
	_delay_ms(1);
}
static void LCD_commandDisable(void){
	_delay_ms(1);
	CLEAR_BIT(LCD_control_port_out, EN);
	_delay_ms(1);
}
void LCD_clearScreen(void){
	LCD_sendCommand(CMD_CLEAR_SCREEN);
}

void LCD_cursorOn(void){
	LCD_sendCommand(CMD_CURSOR_ON);
}

void LCD_cursorOff(void){
	LCD_sendCommand(CMD_CURSOR_OFF);
}
