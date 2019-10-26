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
#include <stdbool.h>

 //I2c Slave address
 uint8_t ui8_address = 0x21;

 //communication variable
 bool protocol=0;

// Variable to store ADC value
volatile uint8_t reading=0;


ISR(INT0_vect){
   
   

   if(PIND & (1<<PD2))
   {

   //Disable UART and enable i2c
   protocol =1;
  TWCR |= (1<<TWEN);
   UCSRB &= ~(1<<TXEN);
   
   // Testing pins
   PORTD |= (1<<PD5);
   PORTD &= ~(1<<PD6);
   }

   else {
   //Disable i2c and enable uart
   
   protocol=0;
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
  
  if(protocol==0){
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

  // initialize I2C slave Protocol //TODO
  i2c_slave_init(ui8_address);
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

DDRD &= ~(1<<PD2);

// Enable external interrupt at PD3 ( INT1 )
GICR |= (1<<INT0);

// Any logical change will generate an interrupt request
MCUCR |= (1<<ISC00);


}

