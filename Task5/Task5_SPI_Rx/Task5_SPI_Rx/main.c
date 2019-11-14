/*
 * Task5_SPI_Rx.c
 *
 * Created: 11/13/2019 11:32:10 AM
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
#include "SPI_lib.h"
#include "PWM_lib.h"
#include "timer_lib.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


// Variable for receiving data from SPI
volatile uint8_t ui8_RX = 0;

// Variable for calculation volts
volatile float f_volt=0;

// data buffer for conversion from integer/float value to string
volatile char c_data_array[7];

// time variable for plotting
volatile uint8_t ui8_time = 0;

// variable to store current time point for interpolation
volatile uint8_t ui8_time1 = 0;

// Delete bar to empty the screen so data won't overlap
volatile uint8_t ui8_delbar = DEL_BAR;

// Variables to hold current and data points
volatile uint8_t ui8_scale =0;
volatile uint8_t ui8_scale1 =44;





// Preprocessor for plot function
void Plot(uint8_t ui8_RX);


ISR(TIMER1_COMPB_vect){
  
  ui8_RX = spi_receive();
 
  PORTD ^= (1<<PD6);
  // Set PWM output for LED Brightness
  timer0_cycle(ui8_RX);

  //Reset Watchdog timer
 wdt_reset();

  // Plot the data on the LCD
  Plot(ui8_RX);


  //Toggle LED for testing
  
}

int main(void)
{
  
  //Initialize LCD
  lcd_init();
  lcd_clear();
  lcd_set_cursor(0,0);
  
  //Check watchdog timer status
  if(MCUCSR&(1<<WDRF))
  {
    //If condition is true, display error message for 2 seconds
    lcd_puts(font5x8,"I2C failure-System reset");
    _delay_ms(2000);

    //Clear Watchdog reset pin
    WatchDog_clear();

    //Clear LCD from error message
    lcd_clear();
  }


  //Initialize Timer for PWM output
  timer0_init();

  //initialize time 1 for controlling i2c data transmission
  timer1_init();
  
  //Initialize 
  spi_master_init();
  SS_Enable;
  //Set pins for testing
  //DDRB |= (1<<DDB4);

  //Enable WatchDog timer
  //WatchDog_on();
  
  //Enable Global Interrupt
  sei();

 
  DDRD |= (1<<PD6);
  
  while (1)
  {

  }
}




void Plot(uint8_t ui8_RX){

  // Calculate volts from ADC value
  f_volt = (ui8_RX*5.0)/255;

  // Convert the value into a string for printing
  dtostrf(f_volt,3,2,c_data_array);
  
  // Set cursor location for printing
  lcd_set_cursor(0,55);

  // print the volt value
  lcd_puts(font5x8,c_data_array);
  lcd_puts(font5x8,"v");

  // Set curve scale
  ui8_scale = 44 - (ui8_RX*40)/255;

  // Plot graph using interpolation function
  g_Interp1(ui8_time1,ui8_scale1,ui8_time,ui8_scale,1);
  
  // Create delete bar to update the curve
  g_VLine(ui8_delbar,0,50,0);

  // Save Current data & time points
  ui8_scale1 = ui8_scale;
  ui8_time1 = ui8_time;

  // Increment time-axis and delete bar axis
  ui8_time++;
  ui8_delbar++;


  // if time-axis reaches the end of LCD (128) --> reset time axis
  if(ui8_time>125){
    ui8_time =0;
    ui8_time1=0;
  }

  if(ui8_delbar>125){
    ui8_delbar=0;
  }


}
