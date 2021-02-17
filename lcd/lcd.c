/*
 * lcd.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Youssef
 */
#include "lcd.h"

static void LCD_commandEnable(void);
static void LCD_commandDisable(void);
static void LCD_dataEnable(void);
static void LCD_dataDisable(void);

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
	LCD_cursorOff();
	LCD_clearScreen();
}
void LCD_sendCommand(uint8 a_command){
	LCD_commandEnable();
	#if (LCD_mode == 8)
		LCD_data_port_out = a_command;
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
	LCD_dataEnable();
	#if (LCD_mode == 8)
		LCD_data_port_out = a_characterToDisplay;
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

void LCD_displayString(const sint8 * stringToDisplay){
	while((*stringToDisplay) != '\0'){
		LCD_displayCharacter((*stringToDisplay));
		stringToDisplay++;
	}
}

void LCD_goToRowColumn(uint8 a_row, uint8 a_col){
	uint8 address = 0;
	switch(a_row){
	case 0:
		address = a_col;
		break;
	case 1:
		address = a_col+0x40;
		break;
	case 2:
		address = a_col+0x10;
		break;
	case 3:
		address = a_col+0x50;
		break;
	}
	LCD_sendCommand(address|(1<<7));
}

void LCD_displayStringRowColumn(const sint8 * stringToDisplay, uint8 a_row, uint8 a_col){
	LCD_goToRowColumn(a_row,a_col);
	LCD_displayString(stringToDisplay);
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