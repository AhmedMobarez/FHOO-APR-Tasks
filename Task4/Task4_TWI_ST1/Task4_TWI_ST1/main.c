/*
 * Task4_TWI_ST1.c
 *
 * Created: 11/2/2019 7:50:17 PM
 * Author : Ahmed
 * Description :
 * This is the first Transmitting slave MCU Code for Task4 in the Applied Programming Course at FHOO
 *
 * Purpose :
 *  The implementation of this code includes reading sensor data through ADC then the transmission of this data through I2C/TWI communication protocol
 *  to another device (Master)
 *
 *
 * Input/Output :
 *  Input : ADC data (ADCH)
 *  Output : TWI Data
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2
 *
 *  Developed on Windows 10 using AtmelStudio 7
 */ 



#include "ADC_lib.h"
#include "timer_lib.h"
#include "i2c_lib.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

//Variable to store ADC readings
volatile uint8_t reading=0;

ISR(ADC_vect){
  
  // Read ADC data
  reading = ADCH;

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
    PORTD ^= (1<<PD6);               //For testing
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

  //Initialize timer 1 with 500ms
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

