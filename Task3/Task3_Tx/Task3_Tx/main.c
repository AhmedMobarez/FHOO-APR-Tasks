/*
 * Task3_Tx.c
 *
 * Created: 10/24/2019 9:57:24 AM
 * Author : Ahmed
 */ 


#define F_CPU (8000000) //Set clock frequency
#include <avr/io.h>
#include <util/delay.h>
#include "ADC_lib.h"
#include "timer_lib.h"
#include <avr/interrupt.h>
#include "i2c_lib.h"
#include "UART_lib.h"
#include <avr/wdt.h>

 //I2c Slave address
 uint8_t ui8_address = 0x21;

void toggle_protocol(uint8_t protocol);

// Variable to store ADC value
volatile uint8_t reading=0;

// Variable that described used communication protocol --> i2c : 0  , UART : 1
volatile uint8_t protocol = 0; 

ISR(ADC_vect){
  
  // Read ADC data
  reading = ADCH;

  //Send data through uart
  
  uart_write(reading);
  
  wdt_reset();

  }






ISR(TWI_vect){
  // Check status if the i2c bus and act accordingly
  switch(TWSR & TW_STATUS_MASK){
    
    // Slave address acknowledged, data byte will be transmitted
    case TW_ST_SLA_ACK :
    PORTD ^= (1<<PD5);               //For testing
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

 

  // Initialize ADC Module
  ADC_init();

  // Initialize I2C slave Protocol //TODO
  i2c_slave_init(0);

  //Initialize timer 1
  timer1_init();

  // Initialize UART
  uart_init(9600);



  // set pins PD5, PD6 as output for testing
  DDRD |= (1<<DDD5) | (1<<DDD6);

  //Start ADC conversion
  ADCSRA |= (1<<ADSC);

  //Enable watchdog timer
  WatchDog_on();

  //Setup communication protocol pin
  DDRB &= ~(1<<DDB6);

  //Enable Global Interrupt
  sei();


  while (1)
  {
  toggle_protocol(protocol);

  }

}


void toggle_protocol(uint8_t protocol){


//Toggle Communication protocol
switch(!(PINB & (1<<PB6))){
  
  case 1 :
  TWCR |= (1<<TWIE);
  ui8_address = 0x21;
  protocol =0;
  //PORTD ^= (1<<PD6);
  _delay_ms(100);
  break;

  case 0:
  
  ui8_address=0;
  protocol =1;
  //PORTD ^= (1<<PD6);
  _delay_ms(100);
  break;
  }



}