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

 // Status variable of the current communication protocol --> 0:I2c , 1:UART
 uint8_t protocol =0;
// Function to toggle communication protocol
void Push_button();

// Variable to store ADC value
volatile uint8_t reading=0;

ISR(INT1_vect){

    PORTD ^= (1<<PD6);               //For testing
    if (PORTD & (1<<PD6))
    {
    protocol =1;
    PORTD |= (1<<PD5);

    }

    else
    {
    protocol = 0;
    PORTD &= ~(1<<PD5);

    }

}






ISR(ADC_vect){
  
  // Read ADC data
  reading = ADCH;

  if (protocol==1){
  uart_write(reading);

  }

}

ISR(TWI_vect){
  // Check status if the i2c bus and act accordingly
  switch(TWSR & TW_STATUS_MASK){
    
    // Slave address acknowledged, data byte will be transmitted
    case TW_ST_SLA_ACK :
    //PORTD ^= (1<<PD5);               //For testing
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

  //I2c Slave address
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

  //push button function
  Push_button();

  //Enable watchdog timer
  WatchDog_on();

  //Enable Global Interrupt
  sei();


  while (1)
  {
  }

}



void Push_button (){

// Enable external interrupt at PD3 ( INT1 )
GICR |= (1<<INT1);

// Any logical change will generate an interrupt request
MCUCR |= (1<<ISC10);

}





