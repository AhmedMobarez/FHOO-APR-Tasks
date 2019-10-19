/*
 * Task1_RX.c
 *
 * Created: 10/9/2019 3:55:46 PM
 * Author : Ahmed Mobarez
 *
 *	This is the Transmitting MCU Code for Task1 in the Applied Programming Course at FHOO
 *
 *	The implementation of this code includes reading sensor data through ADC then transmit this data through UART communication to another device
 *	
 *
 * Input : ADC data (ADCH) -- Output : 1.UART Data 
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2 
 *
 *Developed on Windows 10 using AtmelStudio 7
 */ 
 #define F_CPU (8000000) //Set clock frequency 
#include <avr/io.h>
#include <util/delay.h>
#include "UART_lib.h"
#include "ADC_lib.h"
#include "timer_lib.h"
#include <avr/interrupt.h>

uint8_t reading=0;

ISR(ADC_vect){
	
	//Toggle pin for testing
	PORTD ^= (1<<PD5);
	
	//Send ADCH through UART
	uart_write(ADCH);


}

ISR(TIMER1_COMPB_vect){
	//Toggle pin for testing
	PORTD ^= (1<<PD6); 
	
	
}





int main(void)
{	

	
	// Initialize ADC Module
	ADC_init();				

	// initialize UART transfer					
	uart_init(9600);

	//Initialize timer 1	
	timer1_init();						

	// set pins pd5, pd6 as output for testing
	DDRD |= (1<<DDD5) | (1<<DDD6);			

	//Start conversion
	ADCSRA |= (1<<ADSC);

	//Enable Global Interrupt
	 sei();


    while (1) 
	{


	}

}

