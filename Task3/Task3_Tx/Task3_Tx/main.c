/*
 * Task3_Tx.c
 *
 * Created: 10/24/2019 9:57:24 AM
 * Author : Ahmed
 * Description :
 *  This is the Transmitting MCU Code for Task3 in the Applied Programming Course at FHOO
 *
 * Purpose :
 *  The implementation of this code includes reading sensor data through ADC then the transmission of this data through I2C/TWI or UART communication protocol
 *  to another device.
 *  The user can change the communication protocol from I2C/TWI to UART at will using a push button on the receiver side which triggers
 *  an external interrupt on this side ( transmitter side) to implement that change
 *
 *
 * Input/Output :
 *  ADC data (ADCH) -- Output : TWI Data/ UART Data
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2
 *
 *  Developed on Windows 10 using AtmelStudio 7
 */ 

#define I2C 1           // I2C communication protocol is represented as a High value
#define UART 0          // UART communication protocol is represented as a Low value


#include "UART_lib.h"
#include "ADC_lib.h"
#include "timer_lib.h"
#include "i2c_lib.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <stdbool.h>

 //I2c Slave address
 uint8_t ui8_address = 0x21;

 //communication variable
 bool protocol=0;

// Variable to store ADC value
volatile uint8_t reading=0;

//Function prototype
void push_button();


ISR(INT0_vect){
   
   if(PIND & (1<<PD2))
   {

   //Disable UART and enable i2c
   protocol =I2C;
   TWCR |= (1<<TWEN);
   UCSRB &= ~(1<<TXEN);
   
   // Testing pins
   PORTD |= (1<<PD5);
   PORTD &= ~(1<<PD6);
   }

   else {
   //Disable i2c and enable UART
   
   protocol=UART;
   TWCR &= ~(1<<TWEN);
   UCSRB |= (1<<TXEN);

   //Testing pins
   PORTD |= (1<<PD6);
   PORTD &= ~(1<<PD5);
   }

}


ISR(ADC_vect){
  
  // Read ADC data
  reading = ADCH;
  
  if(protocol==UART){
  wdt_reset();
  uart_write(reading);}

}

ISR(TWI_vect){
  
  // Check status if the i2c bus and act accordingly
  switch(TWSR & TW_STATUS_MASK){
    
    // Slave address acknowledged, data byte will be transmitted
    case TW_ST_SLA_ACK :
    wdt_reset();
    i2c_write(reading);
    break;

    // Data byte acknowledged, sending another data byte
    case TW_ST_DATA_ACK :
    wdt_reset();
    i2c_write(reading);
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


  //UART init
  uart_init(9600);

  //I2c Slave address
  uint8_t ui8_address = 0x21;

  // Initialize ADC Module
  ADC_init();

  // initialize I2C slave Protocol
  i2c_slave_init(ui8_address);

  //Push button function init
  push_button();

  //Initialize timer 1
  timer1_init();

  // set pins PD5, PD6 as output for testing
  DDRD |= (1<<DDD5) | (1<<DDD6);

  //Start ADC conversion
  ADCSRA |= (1<<ADSC);  

  //Enable watchdog timer
  WatchDog_on();

  //Enable Global Interrupt
  sei();


  while (1)
  {
  }

}



void push_button(){

  // SET PD2 as input
  DDRD &= ~(1<<PD2);
  
  // Enable external interrupt at PD2 ( INT0 )
  GICR |= (1<<INT0);
  
  // Any logical change will generate an interrupt request
  MCUCR |= (1<<ISC00);


}

