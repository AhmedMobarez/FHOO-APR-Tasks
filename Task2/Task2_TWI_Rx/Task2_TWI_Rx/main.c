/*
 * Task2_TWI_Rx.c
 *
 * Created: 10/15/2019 3:05:25 PM
 * Author : Ahmed
 */ 

#define F_CPU (8000000) //Set clock frequency
#define DEL_BAR (6)

#include <avr/io.h>
#include <stdlib.h>
#include "Display/graphics.h"
#include "Display/mylcd.h"
#include "Display/font4x8.h"
#include "Display/font6x8.h"
#include "Display/font5x8.h"
#include "i2c_lib.h"
#include "PWM_lib.h"
#include "timer_lib.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>


volatile uint8_t ui8_RX = 0;			//Variable for receiving high byte from i2c
volatile float f_volt=0;
volatile char c_data_array[7];			// data buffer for conversion from integer value to string
volatile uint8_t ui8_time = 0;			// time variable for plotting
volatile uint8_t ui8_time1 = 0;			// variable to store previous time point
volatile uint8_t ui8_delbar = DEL_BAR;
volatile uint8_t ui8_scale =0;			// Variable to scale the time-dependent curve
volatile uint8_t ui8_scale1 =44;			// Variable to scale the time-dependent curve
volatile uint8_t ui8_address = 0x21;  //i2c slave address
volatile uint8_t plot_cond=0;




void Plot(uint8_t ui8_RX){


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





ISR(TIMER1_COMPB_vect){

i2c_start(ui8_address);

//i2c_sendack();
ui8_RX = i2c_readack();
Plot(ui8_RX);
i2c_stop();
PORTB ^= (1<<PB4);
}

int main(void)
{
    
  //Initialize LCD
  lcd_init();
  lcd_clear();
  lcd_set_cursor(0,0);
  
  
  //Initialize Timer for PWM output
  timer0_init();

  //initialize time 1 for controlling i2c
  timer1_init();
  
  //Initialize I2c
  i2c_master_init();  

  //Set pins for testing
  DDRB |= (1<<DDB4);
  //Enable WatchDog timer
  //WatchDog_on();
  
  //Enable Global Interrupt
  sei();
  
  
  while (1) 
  {
  }
}

