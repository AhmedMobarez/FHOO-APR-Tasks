/*
 * Task5_SPI_Tx.c
 *
 * Created: 11/13/2019 12:19:51 PM
 * Author : Ahmed
 */ 

 #define F_CPU (8000000) //Set clock frequency
 #include <avr/io.h>
 #include <util/delay.h>
 #include "ADC_lib.h"
 #include "timer_lib.h"
 #include "SPI_lib.h"
 #include <avr/interrupt.h>
 #include <avr/wdt.h>

 volatile uint8_t reading=0;

 ISR(ADC_vect){
   
   // Read ADC data
   reading = ADCH;
   
 }

 ISR(SPI_STC_vect){

 SPDR = reading;
 PORTD ^= (1<<PD6);
  
  

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

   // initialize SPI//TODO
   spi_slave_init();

   //Initialize timer 1
   timer1_init();

   // set pins PD5, PD6 as output for testing
   //DDRD |= (1<<DDD5) | (1<<DDD6);

   //Start ADC conversion
   ADCSRA |= (1<<ADSC);

   //Enable watchdog timer
   //WatchDog_on();

   //Enable Global Interrupt
   sei();

    DDRD |= (1<<PD6);
   while (1)
   {


   }

 }

