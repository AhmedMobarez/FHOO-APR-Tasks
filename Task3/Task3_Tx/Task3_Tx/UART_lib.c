/*
 * UART_lib.c
 *
 * Created: 10/3/2019 9:58:11 AM
 *  Author: Ahmed
 */ 

 #include "UART_lib.h"



 void baud_rate(uint16_t baud_val){

 // Value to be calculated that corresponds to required baud rate
 uint16_t UBRR=0;	

 // Calculate UBRR value for the micro-controller
 UBRR = (F_CPU/baud_val)-1;	

 // Split the value into the higher and lower UBRR bits
 UBRRH = (UBRR >> 8);
 UBRRL = UBRR; 

 }


 void uart_init(uint16_t baud_val){

 // Enable UART reception and transmission
 UCSRB |= (1<<RXEN) | (1<<TXEN);

 // Set frame format : 8 data bits, 1 stop bits

 UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);

 //Set baud_Rate
 baud_rate(baud_val);


 }


 unsigned char uart_read(){

 // Wait for data to be received 
 while ( !(UCSRA & (1<<RXC)) );

 //Return received value
 return UDR;

 }




 unsigned char uart_write(unsigned char send){


//Wait for empty transmit buffer
while ( !(UCSRA & (1<<UDRE) ) );

//Put data into buffer
UDR = send;


 }

