/*
 * Task1_RX.c
 *
 * Created: 10/9/2019 3:55:46 PM
 * Author : Ahmed Mobarez
 *
 *	This is the Receiving MCU Code for Task1 in the Applied Programming Course at FHOO
 *
 *	The implementation of this code includes receiving data through UART communication from another device, Using this data to control brightness of LED via PWM and printing the data on an LCD.
 *	A watchdog timer is implemented to reset the system if no data is received after two seconds of waiting
 *
 * Input : UART data (UDR) -- Output : 1.PWM for LED brightness //  2.Printing on LCD the variable and time curve
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2 
 *
 *Developed on Windows 10 using AtmelStudio 7
 */ 

#define F_CPU (8000000) //Set clock frequency
#define DEL_BAR (6)

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "graphics.h"
#include "mylcd.h"
#include "font4x8.h"
#include "font6x8.h"
#include "font5x8.h"
#include "UART_lib.h"
#include "PWM_lib.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>

volatile uint8_t ui8_RX = 0;			//Variable for receiving high byte from UART
volatile float f_volt=0;
volatile char c_data_array[7];			// data buffer for conversion from integer value to string
volatile uint8_t ui8_time = 0;			// time variable for plotting
volatile uint8_t ui8_time1 = 0;			// variable to store previous time point
volatile uint8_t ui8_delbar = DEL_BAR;
volatile uint8_t ui8_scale =0;			// Variable to scale the time-dependent curve
volatile uint8_t ui8_scale1 =44;			// Variable to scale the time-dependent curve



ISR(USART_RXC_vect)
{	
  wdt_reset();														// Reset watchdog timer											
  ui8_RX = UDR;														// Read high byte from UART
  timer0_cycle(ui8_RX);													// Set PWM output for LED Brightness
  f_volt = (ui8_RX*5.0)/255;
  					
  dtostrf(f_volt,3,2,c_data_array);									//Convert the value into a string for printing 
  lcd_set_cursor(0,55);												// Set cursor location for printing
  lcd_puts(font5x8,c_data_array);									// print the volt value 
  lcd_puts(font5x8,"v");											// Print v''-volts
  
  ui8_scale = 44 - (ui8_RX*40)/255;									// Set curve scale

  g_Interp1(ui8_time1,ui8_scale1,ui8_time,ui8_scale,1);				// Plot graph using interpolation function
  g_VLine(ui8_delbar,0,50,0);										// Create delete bar to update the curve

  ui8_scale1 = ui8_scale;											//Save previous data point
  ui8_time1 = ui8_time;												//Save previous time-frame point
  ui8_time++;
  ui8_delbar++;														//Increment time-axis
  
  
  // if time-axis reaches the end of LCD, reset time variables 
  if(ui8_time>125){
  // Reset time variables
  ui8_time =0;
  ui8_time1=0;
  }
   if(ui8_delbar>125){
	   // Reset time variables
	   ui8_delbar=0;
   }

}



int main(void)
{	

  //Initialize LCD
  lcd_init();
  lcd_clear();
  lcd_set_cursor(0,0);
  
  //Check Watchdog timer status
  if(MCUCSR&(1<<WDRF))
  {
  //If condition is true, display error message for 2 seconds
  lcd_puts(font5x8,"Error : System reset");
  _delay_ms(2000);

  //Clear Watchdog reset pin
  WatchDog_clear();

  //Clear LCD from error message
  lcd_clear();
  }
  
  //Initialize Timer for PWM ouput
  timer0_init();
  
  //Initialize UART
  uart_init(9600);
  
  //Enable WatchDog timer
  WatchDog_on();
  
  //Enable Global Interrupt
  sei();
     
     while (1) 
     {
  
     }
  
}


