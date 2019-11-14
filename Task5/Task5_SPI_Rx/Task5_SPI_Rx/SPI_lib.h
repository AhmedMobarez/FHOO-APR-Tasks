/*
 * SPI_lib.h
 *
 * Created: 11/13/2019 11:32:53 AM
 *  Author: Ahmed
 */ 


#ifndef SPI_LIB_H_
#define SPI_LIB_H_


#define MOSI 5								/* Define SPI bus pins */
#define MISO 6
#define SCK 7
#define SS 4
#define SS_Enable PORTB &= ~(1<<SS)			/* Define Slave enable */
#define SS_Disable PORTB |= (1<<SS)			/* Define Slave disable */


#include <avr/io.h>


void spi_master_init();
void SPI_Write(char data);
unsigned char spi_receive();

#endif /* SPI_LIB_H_ */