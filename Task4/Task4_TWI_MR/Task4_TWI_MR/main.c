/*
 * Task4_TWI_MR.c
 *
 * Created: 11/2/2019 7:49:11 PM
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
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


// Variable for receiving data from i2c
volatile uint8_t ui8_RX = 0;
volatile uint8_t ui8_RX2 = 0;					

// Variable for calculation volts
volatile float f_volt=0;
volatile float f_volt2=0;
// data buffer for conversion from integer/float value to string
volatile char c_data_array[7];
volatile char c_data_array2[7];

// time variable for plotting
volatile uint8_t ui8_time = 0;
volatile uint8_t ui8_time2 = 65;

// variable to store current points for interpolation
volatile uint8_t ui8_time_old = 0;
volatile uint8_t ui8_volt_old =44;
volatile uint8_t ui8_time_old2 = 0;
volatile uint8_t ui8_volt_old2 =44;


// Delete bar to empty the screen so data won't overlap
volatile uint8_t ui8_delbar = DEL_BAR;
volatile uint8_t ui8_delbar2 = DEL_BAR + 64;

// Variables to hold current and data points
volatile uint8_t ui8_scale =0;
	

//i2c slave address
volatile unsigned char ui8_address = 0x21;  
volatile unsigned char ui8_address2 = 0x08; 



// Preprocessor for plot function
void double_plot(uint8_t ui8_RX, uint8_t ui8_RX2);

uint8_t i2c_dataframe(unsigned char address);

ISR(TIMER1_COMPB_vect){

  //Get data values from slave
  ui8_RX = i2c_dataframe(ui8_address);
  ui8_RX2 = i2c_dataframe(ui8_address2);

  // Set PWM output for LED Brightness
  timer0_cycle(ui8_RX);
  timer2_cycle(ui8_RX2);
  //Reset Watchdog timer
  wdt_reset();

  // Plot the data on the LCD
  double_plot(ui8_RX,ui8_RX2);


  //Toggle LED for testing
  PORTB ^= (1<<PB4);
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
  timer2_init();

  //initialize time 1 for controlling i2c data transmission
  timer1_init();
  
  //Initialize I2c
  i2c_master_init();  

  //Set pins for testing
  DDRB |= (1<<DDB4);

  //Enable WatchDog timer
  WatchDog_on();
  
  //Enable Global Interrupt
  sei();
  
  
  while (1) 
  {
  }
}



void double_plot(uint8_t ui8_RX,uint8_t ui8_RX2){

  // Calculate volts from ADC value
  f_volt = (ui8_RX*5.0)/255;
  f_volt2 = (ui8_RX2*5.0)/255;

  // Draw a vertical line to separate both curves
  g_VLine(63,2,64,1);
  // Convert the value into a string for printing
  dtostrf(f_volt,3,2,c_data_array);	
  dtostrf(f_volt2,3,2,c_data_array2);	
  // Set cursor location for printing	first value			
  lcd_set_cursor(0,55);

  // print the first volt value
  lcd_puts(font5x8,c_data_array);									
  lcd_puts(font5x8,"v");

  //Set Cursor location for printing the second valhe
  lcd_set_cursor(66,55);

  // Print the second volt value
  lcd_puts(font5x8,c_data_array2);
  lcd_puts(font5x8,"v");

  // Set curve scale
  f_volt = 44 - (uint8_t)f_volt*8;
  f_volt2 = 44 - (uint8_t)f_volt2*8;

  // Plot first graph using interpolation function
  g_Interp1(ui8_time_old,ui8_volt_old,ui8_time,f_volt,1);
  
  // Plot second graph using interpolation function
  g_Interp1(ui8_time_old2,ui8_volt_old2,ui8_time2,f_volt2,1);

  // Create two delete bar to update the curves
  g_VLine(ui8_delbar,0,50,0);
  g_VLine(ui8_delbar2,0,50,0);

  // Save Current data & time points
  ui8_volt_old = f_volt;
  ui8_volt_old2 = f_volt2;

  ui8_time_old = ui8_time;
  ui8_time_old2 = ui8_time2;

  // Increment time-axis and delete bar axis
  ui8_time++;
  ui8_time2= ui8_time + 63;
  ui8_delbar++;
  ui8_delbar2 = ui8_delbar + 63;


  // if time-axis reaches the end of LCD (128) --> reset time axis
  if(ui8_time>62){
    ui8_time =0;
    ui8_time2= ui8_time + 62;
    ui8_time_old=0;
    ui8_time_old2=ui8_time_old + 62;

  }

  if(ui8_delbar>62){
    ui8_delbar=0;
    ui8_delbar2 = ui8_delbar + 62;
  }


}


uint8_t i2c_dataframe(unsigned char address)
{
  uint8_t data=0;
  // Start i2c transmission
  i2c_start(address);

  // Red data from I2C
  data = i2c_readack();

  //Send stop signal to end transmission
  i2c_stop();

  return data;
}