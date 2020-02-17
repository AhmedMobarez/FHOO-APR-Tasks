/*
* Task2_TWI_Tx.c
*
* Created: 10/15/2019 3:04:44 PM
* Author : Ahmed
*
* Description :
*   This is the Transmitting MCU Code for Task2 in the Applied Programming Course at FHOO
*
* Purpose :
*  The implementation of this code includes reading sensor data through ADC then the transmission of this data through I2C/TWI communication protocol
*  to another device
*
*
* Input/Output :
*  ADC data (ADCH) -- Output : TWI Data
*
* MCU : ATmega32 , BOARD : myAVR Board MK2
*
*  Developed on Windows 10 using AtmelStudio 7.0.2389
*/



#include "ADC_lib.h"
#include "timer_lib.h"
#include "i2c_lib.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

//Variable to store ADC values
volatile uint8_t reading=0;

 ISR(ADC_vect)
 {
   
  // Read ADC data
  reading = ADCH;

 }

 ISR(TWI_vect)
 {
   // Check status of the i2c bus and act accordingly ( All Actions are described by the data sheet)
   switch(TWSR & TW_STATUS_MASK){
     
     // Slave address acknowledged, data byte will be transmitted 
     case TW_ST_SLA_ACK :
     PORTD ^= (1<<PD5);               //pin toggling For testing
     wdt_reset();                     // reset watchdog timer
     i2c_write(reading);              // Send data through I2C
     break;

     // Data byte acknowledged, sending another data byte
     case TW_ST_DATA_ACK :
     PORTD ^= (1<<PD6);               //pin toggling For testing
     wdt_reset();                     // reset watchdog timer
     i2c_write(reading);              // Send data through I2C
     break;

     // Data byte received and NACK returned 
     case TW_ST_DATA_NACK :   
     TWCR |= (1<<TWINT) | (1<<TWEA);
     wdt_reset();
     break;

     // Data byte received and Stop signal returned  
     case TW_ST_LAST_DATA :
     TWCR |= (1<<TWINT) | (1<<TWEA);
     wdt_reset();
     break;

     default:
     wdt_reset();  
     break;

   }




 }


int main(void)
{


  //Check watchdog timer status
  if(MCUCSR&(1<<WDRF))
  {
    //Clear Watchdog reset pin
    WatchDog_clear();
   }

  //I2c Slave address
	uint8_t ui8_address = 0x21;

	// Initialize ADC Module
	ADC_init();

	// initialize I2C slave Protocol 
	i2c_slave_init(ui8_address);

	//Initialize timer 1 -- Pass required time interval in seconds
	timer1_init(0.5);

	// set pins PD5, PD6 as output for testing
	DDRD |= (1<<DDD5) | (1<<DDD6);

	//Start ADC conversion
	ADC_start();

  //Enable watchdog timer
  WatchDog_on();

	//Enable Global Interrupt
	sei();


	while (1)
	{
	}

}

