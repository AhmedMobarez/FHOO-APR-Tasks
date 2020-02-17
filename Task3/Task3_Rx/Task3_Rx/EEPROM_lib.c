/*
 * EEPROM_lib.c
 *
 * Created: 10/26/2019 9:42:28 PM
 *  Author: Ahmed
 */ 

 #include "EEPROM_lib.h"






 void EEPROM_write(uint8_t eeprom_address,uint8_t eeprom_data){

 //Disable global interrupt
 cli();

 // Wait for completion of previous write 
 while(EECR & (1<<EEWE));

 // Load address and data to EEPROM
 EEAR = eeprom_address;
 EEDR = eeprom_data;

 //Enable EEPROM Master Write
 EECR |= (1<<EEMWE);

 //Start EEPROM writing sequence
 EECR |= (1<<EEWE);

 //Enable global interrupt
 sei();
 }

 uint8_t EEPROM_read(uint8_t eeprom_address){


 //Wait for completion of previous write 
 while(EECR & (1<<EEWE));

 /* Set up address register */
 EEAR = eeprom_address;

 //Start EEPROM Reading sequence
 EECR |= (1<<EERE);

 //Return data
 return EEDR;

 }