/*
 * PWM_lib.h
 *
 * Created: 10/9/2019 9:20:09 PM
 *  Author: Ahmed
 */ 


#ifndef TIMER_LIB_H_
#define TIMER_LIB_H_
#define  F_CPU 8000000 //Hz
#include <avr/io.h>


 void timer1_init(float delay);

 void WatchDog_on();
 void WatchDog_clear();



#endif /* PWM_LIB_H_ */