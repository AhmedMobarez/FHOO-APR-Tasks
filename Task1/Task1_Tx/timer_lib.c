/*
 * PWM_lib.c
 *
 * Created: 10/9/2019 9:20:26 PM
 *  Author: Ahmed
 */ 

 #include "timer_lib.h"


 void timer0_init(){

  // Set wave generation mode to fast PWM
  TCCR0 |= (1<<WGM01) | (1<<WGM00);
  
  //Set compare match output mode to non-inverting mode
  TCCR0 |=  (1<<COM01)| (1<<COM00);
  
  // No timer Pre-scaling
  TCCR0 |= (1<<CS00);
  
  
  //Enable pin OCR0 as output
  DDRB |= (1<<DDB3);
 }

 void timer0_cycle( uint8_t duty_cycle){
  
  OCR0 = duty_cycle;

 }

  

 void timer1_init()
 {
	 // set up timer with Pre-scaler = 256 
	 TCCR1B |= (1 << WGM12);

	 //Setup CTC Mode
	 TCCR1B |= (1 << CS12);

	 //Enable interrupt
	 TIMSK |= (1<<OCIE1B);
	 
	 // initialize counter
	 TCNT1 = 0;
	 
	 // initialize compare value --> Required delay of 500ms at frequecy of F_CPU/256
	 OCR1A = 15624;


 }