/*
 * ADC_lib.c
 *
 * Created: 10/8/2019 6:48:19 PM
 *  Author: Ahmed
 */ 

 #include "ADC_lib.h"

 void ADC_init(){

 //Set reference voltage to be VCC
 ADMUX |=(1<<REFS0);

 //Choose PA1 as our analog input pin (ADC1)
 ADMUX |=(1<<MUX0);

 //Left adjust ADC data for easier 8 bit reading
 ADMUX |=(1<<ADLAR) ;


  //Enable ADC
  ADCSRA |= (1<<ADEN);
  
  //Set Pre-scaler value to 128 ( frequency = F_CPU/128 = 62.5KHz )
  ADCSRA |=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
  
  //Enable ADC Interrupt
  ADCSRA |= (1<<ADIE);
  
  //Enable ADC auto-triggering
  ADCSRA |= (1<<ADATE);
  
  //Auto Triggering mode -> Timer/Counter 1 Compart Match B 
  SFIOR |= (1<<ADTS2) | (1<<ADTS0);
 

 }


 void ADC_read(){

 //Start conversion
ADCSRA |= (1<<ADSC);


 }