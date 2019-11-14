/*
 * SPI_lib.h
 *
 * Created: 11/13/2019 11:32:53 AM
 *  Author: Ahmed
 */ 


#ifndef SPI_LIB_H_
#define SPI_LIB_H_

#include <avr/io.h>
#define MOSI 5								/* Define SPI bus pins */
#define MISO 6
#define SCK 7
#define SS 4
#define SS_Enable PORTB &= ~(1<<SS)			/* Define Slave enable */
#define SS_Disable PORTB |= (1<<SS)			/* Define Slave disable */


void spi_slave_init();
unsigned char spi_transmit(unsigned char data);
char SPI_Receive();

#endif /* SPI_LIB_H_ */