/*
 * PWM_lib.h
 *
 * Created: 10/9/2019 9:20:09 PM
 *  Author: Ahmed
 */ 


#ifndef TIMER_LIB_H_
#define TIMER_LIB_H_

#include <avr/io.h>



void timer1_init();
void WatchDog_on();
void WatchDog_clear();


#endif /* PWM_LIB_H_ */