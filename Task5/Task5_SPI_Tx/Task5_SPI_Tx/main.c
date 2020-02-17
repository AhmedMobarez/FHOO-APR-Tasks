/*
 * Task5_SPI_Tx.c
 *
 * Created: 11/13/2019 12:19:51 PM
 * Author : Ahmed
 * Description :
 * This is the Transmitting MCU Code for Task5 in the Applied Programming Course at FHOO
 *
 * Purpose :
 *  The implementation of this code includes reading sensor data through ADC then the transmission of this data through SPI communication protocol
 *  to another device
 *
 *
 * Input/Output :
 *  Input  : ADC data (ADCH)
 *  Output : SPI Data
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2
 *
 * Developed on Windows 10 using AtmelStudio 7
 */ 

 #define F_CPU (8000000) //Set clock frequency
 #include "ADC_lib.h"
 #include "timer_lib.h"
 #include "SPI_lib.h"
 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>
 #include <avr/wdt.h>

 // Variable to store ADC Value
 volatile uint8_t reading=0;

 ISR(ADC_vect){
   
   // Read ADC data
   reading = ADCH;
   
 }


 ISR(SPI_STC_vect){

 // Send ADC Value through SPI Data register
 SPDR = reading;

 //Toggle LED for testing
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

   // initialize SPI
   spi_slave_init();

   //Initialize timer 1
   timer1_init();



   //Start ADC conversion
   ADCSRA |= (1<<ADSC);

   //Enable watchdog timer
   WatchDog_on();

   //Enable Global Interrupt
   sei();

   //Set PD6 as output for testing
   DDRD |= (1<<PD6);

   while (1)
   {


   }

 }

