/*
 * SPI_lib.c
 *
 * Created: 11/13/2019 11:33:07 AM
 *  Author: Ahmed
 */ 
 #include "SPI_lib.h"






 void spi_transmit(unsigned char data){
  //Write data to SPI data register
  SPDR = data;

  
  while(!(SPSR & (1<<SPIF)));	


 }



 unsigned char SPI_Receive()								
 {  
  //Wait till transmission complete
  while(!(SPSR & (1<<SPIF)));

  //Return Data
  return(SPDR);	
 }



 void spi_slave_init(){
  // Make MOSI, SCK, SS pin direction as input pins 
  DDRB &= ~((1<<MOSI)|(1<<SCK)|(1<<SS));

  //make MISO pin as output pin 
  DDRB |= (1<<MISO);

  //Enable SPI & SPI interrupt
  SPCR = (1<<SPE) | (1<<SPIE);
 }
