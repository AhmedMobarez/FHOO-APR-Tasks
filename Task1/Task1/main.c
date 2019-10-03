/*
 * Task1.c
 *
 * Created: 10/3/2019 9:34:04 AM
 * Author : Ahmed
 * Task1 : Communication between two micro-controllers (ATMega 32) using the UART interface 
 * 
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "graphics.h"
#include "mylcd.h"
#include "font6x8.h"
#include "font5x8.h"
#include "UART_lib.h"
#define F_CPU (8000000) //Set clock frequency 

int main(void)
{
	lcd_init();
	uart_init(9600);
	lcd_clear();
	lcd_set_cursor(0,0);
	//lcd_puts(font5x8, "Data from MC1 is ");
	uint8_t data=0;

	/

	data = uart_read();
	if (data ==7)
	  lcd_puts(font5x8,"Transmission success");
	else
		lcd_puts(font5x8,"Transmission failed");

    while (1) 
    {
	

	
//	data = 7;
	//uart_write(data);
	


    }
}

