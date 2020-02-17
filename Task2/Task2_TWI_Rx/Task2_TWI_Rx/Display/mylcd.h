// ##################################################################################################
// #																								#
// #   Name: mylcd.h																				#
// #   Author: Andre Fabricius  (mailto:master.andre@web.de)										#
// #   Date: 13.08.08 18:32																		    #
// #   Description: Header for GLCD routines for KS0108 or compatibel controllers				    #
// #																								#
// ##################################################################################################
// #																								#
// # Copyright (C) 2008  Andre Fabricius															#
// #																								#
// # This program is free software; you can redistribute it and/or modify it under the terms of 	#
// # the GNU General Public License as published by the Free Software Foundation; either version 3 	#
// # of the License, or (at your option) any later version.											#
// #																								#
// # This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 		#				
// # without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 		#
// # See the GNU General Public License for more details.											#
// # You should have received a copy of the GNU General Public License along with this program; 	#
// # if not, see <http://www.gnu.org/licenses/>.													#
// #																								#
// ##################################################################################################

// ##################################################################################################
// #																								#
// #	  Test-Configuration:	ATMEGA8535 @ 16MHz													#
// #							PowerTip PG12864K (PG12864WRF-KNN-H)								#
// #							WinAVR-20070525														#
// #							AVR Studio V4.13 Build 571											#
// #																								#
// ##################################################################################################

#ifndef	MYLCD_H
#define MYLCD_H
#include <avr/pgmspace.h>

	//###############################################################################################
	//# 																							#
	//# User Configurations																			#
	//#																								#
	//###############################################################################################
	
	// Ports
	#define LCD_CMD_PORT		PORTC		// Command Output Register
	#define LCD_CMD_DDR			DDRC		// Data Direction Register for Command Port

	#define LCD_DATA_PIN		PINA		// Data Input Register
	#define LCD_DATA_PORT		PORTA		// Data Output Register
	#define LCD_DATA_DDR		DDRA		// Data Direction Register for Data Port

	// Command Port Bits
	#define CD					0x02		// D/I (Data/Control (RS))
	#define RW					0x03		// R/W (Read/Write)
	#define EN					0x04		// E   (Enable)
	#define CS1					0x05		// CS1 (Chip Select 1)
	#define CS2					0x06		// CS2 (Chip Select 2)	
	//#define LCD_NO_RES					// uncomment this line if you have an pullup on reset
	
	#ifndef LCD_NO_RES
		#define RES				0x07		// RST (Reset)
	#endif



	//###############################################################################################
	//# 																							#
	//# Extra configuration																			#
	//#																								#
	//###############################################################################################

	#define FASTTEXT 			// comment this if you get bad text output
								// draws Text much faster (writes whole pages without using set_pixel)	
								// only recommended to use if font has an height of 8 (or multipel)
								// using y-positions,0,8,16,24... lcd_set_line(uint8_t line)	

	//#define NO_BMPS			// if you do not need bitmaps
	//#define NO_GRAPHICS		// if you do not need lines/rects/circels
	//#define NO_BRESENHAM		// if you only use horizontal or vertical lines (or rects) (~264 Bytes)
	//#define NO_TEXT			// if you do not need text output

	//###############################################################################################
	//# 																							#
	//# Defines																						#
	//#																								#
	//###############################################################################################

	// Chips
	#define CHIP1				0x01
	#define CHIP2				0x02

	// Commands
	#define LCD_ON				0x3F
	#define LCD_OFF				0x3E
	#define LCD_SET_ADD			0x40
	#define LCD_SET_PAGE		0xB8
	#define LCD_DISP_START		0xC0
	#define LCD_BUSY			0x80

	// Pixel Colors
	#define BLACK				0x01
	#define WHITE				0x00

	// Lines

	#define LINE0				0x00
	#define LINE1				0x08
	#define LINE2				0x10
	#define LINE3				0x18
	#define LINE4				0x20
	#define LINE5				0x28
	#define LINE6				0x30
	#define LINE7				0x38


	//###############################################################################################
	//# 																							#
	//# Prototyes																					#
	//#																								#
	//###############################################################################################

	#define max(a, b) ((a)>(b)?(a):(b)) 
	#define min(a, b) ((a)>(b)?(b):(a))


	// low level lcd	
	void lcd_chip_select(uint8_t);
	void lcd_write_cmd(uint8_t,uint8_t);
	void lcd_write_data(uint8_t,uint8_t);
	void lcd_write(uint8_t,uint8_t);
	uint8_t lcd_read_data(uint8_t);
	void wait_while_chip_is_busy();
	
	// lcd
	void lcd_init();
	void lcd_clear();
	void lcd_set_pixel(uint8_t,uint8_t,uint8_t);
	void lcd_invert();
	//#ifndef NO_BMP
	//void lcd_draw_fullscreen_bmp(uint8_t*);
	//#endif



	// low level font
	#ifndef NO_TEXT
	uint8_t font_number_of_chars(const uint8_t*);
	uint8_t font_first_char(const uint8_t*);
	uint8_t font_char_width(const uint8_t*,uint8_t);
	uint8_t font_char_height(const uint8_t*);
	uint16_t font_start_offset(const uint8_t*,uint8_t);
	uint8_t font_char_present(const uint8_t*,uint8_t);	

	// font
	void lcd_set_cursor(uint8_t, uint8_t);	
	void lcd_putc(const uint8_t*,uint8_t);
	void lcd_puts(const uint8_t*, char* );
	void lcd_puts_p(const uint8_t*, char* PROGMEM );
	#endif

#endif
