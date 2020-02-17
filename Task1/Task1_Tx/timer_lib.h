/*
 * PWM_lib.h
 *
 * Created: 10/9/2019 9:20:09 PM
 *  Author: Ahmed
 */ 


#ifndef TIMER_LIB_H_
#define TIMER_LIB_H_
#define F_CPU 8000000 //Hz
#include <avr/io.h>


void timer0_init();
void timer0_cycle(uint8_t duty_cycle);
void timer1_init(float delay);



#endif /* PWM_LIB_H_ */