/*
 * UART_lib.h
 *
 * Created: 10/3/2019 9:57:48 AM
 *  Author: Ahmed
 */ 
 #include <avr/io.h>
 #define F_CPU 8000000



#ifndef UART_LIB_H_
#define UART_LIB_H_

void uart_init(uint16_t baud_val);
void baud_rate(uint16_t baud_val);
void uart_write( unsigned char send);



#endif /* UART_LIB_H_ */