/*
 * SPI_lib.c
 *
 * Created: 11/13/2019 11:33:07 AM
 *  Author: Ahmed
 */ 
 #include "SPI_lib.h"






 unsigned char spi_transmit(unsigned char data){

  	SPDR = data;								/* Write data to SPI data register */
  	while(!(SPSR & (1<<SPIF)));					/* Wait till transmission complete */
  	return(SPDR);
 }



 char SPI_Receive()								/* SPI Receive data function */
 {
   while(!(SPSR & (1<<SPIF)));					/* Wait till reception complete */
   return(SPDR);								/* return received data */
 }



 void spi_slave_init(){
 
DDRB &= ~((1<<MOSI)|(1<<SCK)|(1<<SS));		/* Make MOSI, SCK, SS pin direction as input pins */
DDRB |= (1<<MISO);							/* Make MISO pin as output pin */
SPCR = (1<<SPE) | (1<<SPIE);
 }
