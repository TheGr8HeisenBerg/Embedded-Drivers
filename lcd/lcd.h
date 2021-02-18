/*
 * lcd.h
 *
 *  Created on: Feb 16, 2021
 *      Author: Youssef
 */

#ifndef LCD_H_
#define LCD_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define LCD_mode 8 //select weather it's 8/4 bit mode

#if (LCD_mode == 4)
#define upperBits
#endif
#undef upperBits
#define CMD_LCD_TWO_LINES_EIGHT_BIT_MODE 0x38
#define CMD_LCD_TWO_LINES_FOUR_BIT_MODE 0x28
#define CMD_RETURN_HOME 0x02
#define CMD_CLEAR_SCREEN 0x01
#define CMD_CURSOR_ON  0x0E
#define CMD_CURSOR_OFF	0x0c
#define RS PB0
#define RW PB1
#define EN PB2
#define LCD_data_port_dir DDRA
#define LCD_data_port_out PORTA
#define LCD_data_port_in  PINA
#define LCD_control_port_dir DDRB
#define LCD_control_port_out PORTB
#define LCD_control_port_in  PINB

void LCD_init(void);
void LCD_sendCommand(uint8);
void LCD_clearScreen(void);
void LCD_cursorOn(void);
void LCD_cursorOff(void);
void LCD_displayChracter(const uint8);
void LCD_displayString(const sint8 *);
void LCD_goToRowColumn(uint8, uint8);
void LCD_displayStringRowColumn(const sint8 *, uint8, uint8);
void LCD_integerToString(sint32 data);
#endif /* LCD_H_ */
