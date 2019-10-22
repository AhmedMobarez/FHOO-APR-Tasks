/*
 * Task2_TWI_Tx.c
 *
 * Created: 10/15/2019 3:04:44 PM
 * Author : Ahmed
 */ 

 #define F_CPU (8000000) //Set clock frequency
 #include <avr/io.h>
 #include <util/delay.h>
 #include "ADC_lib.h"
 #include "timer_lib.h"
 #include <avr/interrupt.h>
 #include "i2c_lib.h"

 volatile uint8_t reading=0;

 ISR(ADC_vect){
   
   //Toggle pin for testing
   
   
   //Send ADCH through UART
  // uart_write(ADCH);
  reading = ADCH;

 }

 ISR(TWI_vect){

   switch(TWSR & TW_STATUS_MASK){
     case TW_ST_SLA_ACK :
     PORTD ^= (1<<PD5);
     i2c_write(reading);
     break;

     case TW_ST_DATA_ACK :
     PORTD ^= (1<<PD6);
     i2c_write(reading);
     
     break;
     case TW_ST_DATA_NACK :   //TODO
     TWCR |= (1<<TWINT) | (1<<TWEA);
     break;
     case TW_ST_LAST_DATA :   //TODO
     TWCR |= (1<<TWINT) | (1<<TWEA);
     break;

     default:                 //TODO
     break;

   }




 }

 ISR(TIMER1_COMPB_vect){
   //Toggle pin for testing

   
 }



int main(void)
{

	uint8_t ui8_address = 0x21;
	// Initialize ADC Module
	ADC_init();

	// initialize I2C slave Protocol //TODO
	i2c_slave_init(ui8_address);

	//Initialize timer 1
	timer1_init();

	// set pins PD5, PD6 as output for testing
	DDRD |= (1<<DDD5) | (1<<DDD6);

	//Start ADC conversion
	ADCSRA |= (1<<ADSC);

	//Enable Global Interrupt
	sei();


	while (1)
	{
	}

}

