/*
 * Task4_TWI_MR.c
 *
 * Created: 11/2/2019 7:49:11 PM
 * Author : Ahmed
 * Description :
 *  This is the Receiving MCU Code for Task4 in the Applied Programming Course at FHOO
 *
 * Purpose :
 *  The implementation of this code includes receiving data through TWI/I2c communication protocol from two devices/slaves,
 *  Using this data to control the brightness of two LEDs via PWM and printing/plotting the data on an LCD.
 *	A watchdog timer is implemented to reset the system if no data is received after two seconds of waiting
 *
 * Input/Output :
 *  Input  : TWI data (TWDR)
 *  Output : 1.Two PWM signals for LED brightness //  2.Printing on LCD the variables and time curves
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2
 *
 * Developed on Windows 10 using AtmelStudio 7
 */ 

#define F_CPU (8000000) //Set clock frequency
#define DEL_BAR (6)

#include "Display/graphics.h"
#include "Display/mylcd.h"
#include "Display/font5x8.h"
#include "i2c_lib.h"
#include "PWM_lib.h"
#include "timer_lib.h"
#include <stdlib.h>
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

// Variables to hold current and old data points
volatile uint8_t ui8_scale =0;
volatile uint8_t ui8_scale2 =0;

	

//i2c slave address
volatile unsigned char ui8_address = 0x21;  
volatile unsigned char ui8_address2 = 0x08; 



// plot function prototype
void double_plot(uint8_t ui8_RX, uint8_t ui8_RX2);

// i2c dataframe function prototype
uint8_t i2c_dataframe(unsigned char address);

ISR(TIMER1_COMPB_vect){

  //Get data values from slaves
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
    lcd_puts(font5x8,"I2C failure");
    _delay_ms(2000);

    //Clear Watchdog reset pin
    WatchDog_clear();

    //Clear LCD from error message
    lcd_clear();
  }


  //Initialize Timer for PWM output
  timer0_init();
  timer2_init();

  //initialize time 1 for controlling i2c data transmission with 500ms intervals
  timer1_init(0.5);
  
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

  // Convert the values into a string for printing
  dtostrf(f_volt,3,2,c_data_array);	
  dtostrf(f_volt2,3,2,c_data_array2);	

  // Set cursor location for printing	first value			
  lcd_set_cursor(0,55);

  // print the first volt value
  lcd_puts(font5x8,c_data_array);									
  lcd_puts(font5x8,"v");

  //Set Cursor location for printing the second value
  lcd_set_cursor(66,55);

  // Print the second volt value
  lcd_puts(font5x8,c_data_array2);
  lcd_puts(font5x8,"v");

  // Set curve scale ( plot range in Y-axis is 4-40)
  ui8_scale = 44 - (ui8_RX*40)/255;    
  ui8_scale2 = 44 - (ui8_RX2*40)/255;

  // Plot first graph using interpolation function
  g_Interp1(ui8_time_old,ui8_volt_old,ui8_time,ui8_scale,1);
  
  // Plot second graph using interpolation function
  g_Interp1(ui8_time_old2,ui8_volt_old2,ui8_time2,ui8_scale2,1);

  // Create two delete bars to update the curves
  g_VLine(ui8_delbar,0,50,0);
  g_VLine(ui8_delbar2,0,50,0);

  // Save Current data & time points
  ui8_volt_old = ui8_scale;
  ui8_volt_old2 = ui8_scale2;

  ui8_time_old = ui8_time;
  ui8_time_old2 = ui8_time2;

  // Increment time-axis and delete bar axis
  ui8_time++;
  ui8_time2= ui8_time + 63;
  ui8_delbar++;
  ui8_delbar2 = ui8_delbar + 63;


  // if time-axis reaches the end of LCD (128) --> reset time axis
  if(ui8_time>=62){
    ui8_time =0;
    ui8_time2= ui8_time + 64;
    ui8_time_old=0;
    ui8_time_old2=ui8_time_old + 64;

  }

  if(ui8_delbar>=63){
    ui8_delbar=0;
    ui8_delbar2 = ui8_delbar + 65;
  }


}


// Implementation of one full I2C dataframe ( get one byte through I2C )
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