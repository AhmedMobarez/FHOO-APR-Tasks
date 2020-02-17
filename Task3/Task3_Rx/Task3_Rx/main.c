/*
 * Task3_Rx.c
 *
 * Created: 10/24/2019 9:53:40 AM
 * Author : Ahmed
 * Description :
 *  This is the Receiving MCU Code for Task3 in the Applied Programming Course at FHOO
 *
 * Purpose :
 *  The implementation of this code includes receiving data through TWI/I2c or UART communication protocol from another device,
 *  Using this data to control brightness of LED via PWM and printing the data on an LCD.
 *	A watchdog timer is implemented to reset the system if no data is received after two seconds of waiting, also it alternate between the communication protocols automatically in case of failure
 *  The user can change the communication protocol from I2C/TWI to UART at will using a push button 
 *
 * Input/Output :
 *  UART data (UDR)
 *  Output : 1.PWM for LED brightness //  2.Printing on LCD the variable and time curve
 *
 * MCU : ATmega32 , BOARD : myAVR Board MK2
 *
 * Developed on Windows 10 using AtmelStudio 7
 */

#define F_CPU (8000000) //Set clock frequency //Hz
#define DEL_BAR (6)
#define I2C 1           // I2C communication protocol is represented as a High value
#define UART 0          // UART communication protocol is represented as a Low value

#include "Display/graphics.h"
#include "Display/mylcd.h"
#include "Display/font5x8.h"
#include "i2c_lib.h"
#include "PWM_lib.h"
#include "timer_lib.h"
#include "UART_lib.h"
#include "EEPROM_lib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


// Variable for receiving data from i2c
volatile uint8_t ui8_RX = 0;

// Variable for calculation of volts
volatile float f_volt=0;

// data buffer for conversion from integer/float value to string
volatile unsigned char c_data_array[7];

// time variable for plotting
volatile uint8_t ui8_time = 0;

// variable to store current time point for interpolation
volatile uint8_t ui8_time1 = 0;

// Delete bar to empty the screen so data won't overlap
volatile uint8_t ui8_delbar = DEL_BAR;

// Variables to hold current and data points
volatile uint8_t ui8_scale =0;
volatile uint8_t ui8_scale1 =44;        

//i2c slave address
volatile uint8_t ui8_address = 0x21;

//EEPROM address to save communication protocol state
volatile uint8_t ui8_eeprom_address = 0x07;


// protocol value represents the used communication protocol --> I2C:1  , UART:0
bool protocol=0;      

// Functions prototypes
void Plot(uint8_t ui8_RX);
void push_button();
void switch_communication();



ISR(INT0_vect){

  //if PD2 reads high --> set the protocol to I2C  
  if(PIND & (1<<PD2)){
    protocol = I2C;
    switch_communication();
  }

  //if PD2 reads low --> set the protocol to UART  
  else{
    protocol= UART;
    switch_communication();
  }
}


ISR(USART_RXC_vect){

  // Reset watchdog timer
  wdt_reset();														
  
  //Check for parity errors and disable operation if error is found
  check_parity();

  //Store UART Data
  ui8_RX = UDR;

  // Set PWM output duty cycle
  timer0_cycle(ui8_RX);

  //Plot the data
  Plot(ui8_RX);

}

ISR(TIMER1_COMPB_vect){


  if (protocol==I2C){
    // Start i2c transmission
    i2c_start(ui8_address);

    // Read data from I2C
    ui8_RX = i2c_readack();

    // Set PWM output for LED Brightness
    timer0_cycle(ui8_RX);

    //Reset Watchdog timer
    wdt_reset();

    // Plot the data on the LCD
    Plot(ui8_RX);

    //Send stop signal to end transmission
    i2c_stop();
    }
  
  //Check the state of the communication protocol variable and act accordingly 
  switch_communication();
  }



int main(void)
{
  //Check the state of the push button [PD2] to define the initial communication protocol
  if(PIND & (1<<PD2)){
   protocol = I2C;
   }
  
  else{
  protocol = UART;
  }

  //Initialize LCD
  lcd_init();
  lcd_clear();
  lcd_set_cursor(0,0);


   //Initialize UART
   uart_init(9600);

   //Initialize push button operation
   push_button();

  //Initialize Timer for PWM output
  timer0_init();

  //initialize time 1 -- Pass required argument in seconds
  timer1_init(0.5);

  //Initialize UART
  uart_init(9600);

  //Initialize I2c
  i2c_master_init();



  //Set pins for testing
  DDRB |= (1<<DDB4) | (1<<DDB6);



  //Check watchdog timer status
  if(MCUCSR&(1<<WDRF))
  {
    //If condition is true, display error message for 2 seconds
    lcd_puts(font5x8,"System Failure");
    _delay_ms(2000);
    
    //Switch communication
    //Read last communication state from EEPROM
    protocol= EEPROM_read(ui8_eeprom_address);
    
    //Switch the communication protocol
    protocol = !protocol;
    switch_communication();
    
    //Clear Watchdog reset pin
    WatchDog_clear();
    
    //Clear LCD from error message
    lcd_clear();
  }

  //Enable WatchDog timer
  WatchDog_on();

  //Enable Global Interrupt
  sei();


  while (1)
  {
  }

}




void Plot(uint8_t ui8_RX){

  // Calculate volts from ADC value
  f_volt = (ui8_RX*5.0)/255;

  // Convert the value into a string for printing
  dtostrf(f_volt,3,2,c_data_array);

  // Set cursor location for printing the values on the screen
  lcd_set_cursor(0,55);

  // print the volt value
  lcd_puts(font5x8,c_data_array);
  lcd_puts(font5x8,"v");

  //Showing current communication protocol
  if (protocol == 1)
  lcd_puts(font5x8,"  I2C ");
  else
  lcd_puts(font5x8,"  UART");


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


// Setup PD2 -INT0- as a simulated Push Button
void push_button(){

  // Set PD2 as input
  DDRD &= ~(1<<PD2);

  // Enable external interrupt at PD2 ( INT0 )
  GICR |= (1<<INT0);

  // Any logical change will generate an interrupt request
  MCUCR |= (1<<ISC00);

}


// Function to alternate between I2C & UART communication
void switch_communication(){

  // Switch to I2C
  if(protocol==I2C)
  {
    // Enable I2C operations
    TWCR |= (1<<TWEN);

    // Disable UART operations
    UCSRB &= ~((1<<RXEN)| (1<<RXCIE));

    //  Signal the transmitter to change communication protocol
    PORTB |= (1<<PB4);

    // Save current communication state in EEPROM -> I2C
    EEPROM_write(ui8_eeprom_address,protocol);
  }

  // Switch to UART
  else if (protocol==UART)
  {
    // Disable I2C operations
    TWCR &= ~(1<<TWEN);

    // Enable UART operations
    UCSRB |= (1<<RXEN)| (1<<RXCIE);

    //Signal the transmitter to change communication protocol
    PORTB &=~ (1<<PB4);

    // Save current communication state in EEPROM -> UART
    EEPROM_write(ui8_eeprom_address,protocol);
  }

}
