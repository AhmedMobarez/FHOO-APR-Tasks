/*
 * PWM_lib.c
 *
 * Created: 10/9/2019 9:20:26 PM
 *  Author: Ahmed
 */ 

 #include "timer_lib.h"






 void timer1_init(float delay)
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
	 //OCR1A = 15624;
	 OCR1A = ((delay*F_CPU)/256)-1;


 }



  void WatchDog_on ()
  {
    //Enable watchdog timer
    WDTCR |= (1<<WDE);
    
    
    //Set timeout period to be 2.1s
    WDTCR |= (1<<WDP0) | (1<<WDP1) | (1<<WDP2);
    
    
  }



  void WatchDog_clear()
  {
    
    MCUCSR &= ~(1<<WDRF);
  
  }