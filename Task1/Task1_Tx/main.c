/*
 * Task1_Tx.c
 *
 * Created: 10/9/2019 3:55:46 PM
 * Author : Ahmed Mobarez
 *
 *Description : 
 * This is the Transmitting MCU Code for Task1 in the Applied Programming Course at FHOO
 *
 * Purpose :
 *  The implementation of this code includes reading sensor data through ADC then the transmission of this data through UART communication protocol
 *  to another device
 *	
 *
 * Input/Output : 
 *  Input  : ADC data (ADCH) ( 10-bits)
 *  Output : UART Data (8-bits)
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2 
 *
 *  Developed on Windows 10 using AtmelStudio 7.0.2389
 */ 



#include "UART_lib.h"
#include "ADC_lib.h"
#include "timer_lib.h"
#include <util/delay.h>
#include <avr/interrupt.h>



ISR(ADC_vect)
{
	
	//Toggle pin for testing
	PORTD ^= (1<<PD5);
	
	//Send ADCH through UART
	uart_write(ADCH);


}

ISR(TIMER1_COMPB_vect)
{
	//Toggle pin for testing
	PORTD ^= (1<<PD6); 
	
	
}





int main(void)
{	

	
	// Initialize ADC Module
	ADC_init();				

	// initialize UART transfer					
	uart_init(9600);

	//Initialize timer 1 -- Pass required time interval in seconds
	timer1_init(0.5);						

	// set pins PD5, PD6 as output for testing
	DDRD |= (1<<DDD5) | (1<<DDD6);			

	//Start ADCconversion
	ADC_start();

	//Enable Global Interrupt
	 sei();


  while (1) 
	{

	}

}

