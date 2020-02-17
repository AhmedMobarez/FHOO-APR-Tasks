/*
 * SPI_lib.c
 *
 * Created: 11/13/2019 11:33:07 AM
 *  Author: Ahmed
 */ 
 #include "SPI_lib.h"





 void spi_master_init(){
  //Make MOSI, SCK direction as output pins 
  DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);	

  //Make MISO pin as input pin
  DDRB &= ~(1<<MISO);

  //Disable slave initially by making high on SS pin
  PORTB |= (1<<SS);

  //Enable SPI, Enable in master mode, with F_CPU/16 SCK frequency
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

  //Define Slave enable
  SS_Enable;
 }


 unsigned char spi_receive(){

 // Write data to SSPI data register  to start the communication
 SPDR = 0xFF;

 // Wait till reception complete
 while(!(SPSR & (1<<SPIF)));				

 //Return received value
 return(SPDR);

 }



void SPI_Write(char data)					
{
  // variable to flush SPI data register value
  char flush_buffer;

  // Send data through SPI data register 
  SPDR = data;							

  // Wait till transmission complete
  while(!(SPSR & (1<<SPIF)));

  //Flush received data
  flush_buffer = SPDR;

  // Note to self: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR
  // hence flush buffer used here to access SPDR after SPSR read 
}
