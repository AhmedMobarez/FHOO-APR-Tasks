/*
 * PWM_lib.c
 *
 * Created: 10/9/2019 9:20:26 PM
 *  Author: Ahmed
 */ 

 #include "PWM_lib.h"


 void timer0_init(){
  
  // Set wave generation mode to fast PWM
  TCCR0 |= (1<<WGM01) | (1<<WGM00);
  
  //Set compare match output mode to inverting mode
  TCCR0 |=  (1<<COM01)| (1<<COM00);
  
  // No timer Prescaling
  TCCR0 |= (1<<CS00);
  
  
  //Enable pin OCR0 as output
  DDRB |= (1<<DDB3);
  
  
  
}

 void timer0_cycle( uint8_t duty_cycle){
  
  OCR0 = duty_cycle;

 }


 void WatchDog_on ()
 {
  //Enable watchdog timer
  WDTCR |= (1<<WDE);
  
  //Enable interrupt
  
  
  //Set timeout period to be 2.1s
  WDTCR |= (1<<WDP0) | (1<<WDP1) | (1<<WDP2);
  
  
 }

 void WatchDog_clear()
 {
  
  MCUCSR &= ~(1<<WDRF);

 }