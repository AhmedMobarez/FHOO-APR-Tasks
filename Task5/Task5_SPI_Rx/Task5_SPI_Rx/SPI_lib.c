/*
 * SPI_lib.c
 *
 * Created: 11/13/2019 11:33:07 AM
 *  Author: Ahmed
 */ 
 #include "SPI_lib.h"





 void spi_master_init(){

  DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);		/* Make MOSI, SCK, 0th pin direction as output pins */
  DDRB &= ~(1<<MISO);						/* Make MISO pin as input pin */
  PORTB |= (1<<SS);						/* Disable slave initially by making high on SS pin */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);	/* Enable SPI, Enable in master mode, with Fosc/16 SCK frequency */

 }


 unsigned char spi_receive(){

 SPDR = 0xFF;
 while(!(SPSR & (1<<SPIF)));				/* Wait till reception complete */
 return(SPDR);

 }



void SPI_Write(char data)					/* SPI write data function */
{
  char flush_buffer;
  SPDR = data;							/* Write data to SPI data register */
  while(!(SPSR & (1<<SPIF)));				/* Wait till transmission complete */
  flush_buffer = SPDR;					/* Flush received data */
  /* Note: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR hence flush buffer used here to access SPDR after SPSR read */
}
