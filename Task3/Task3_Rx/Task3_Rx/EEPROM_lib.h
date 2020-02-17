/*
 * EEPROM_lib.h
 *
 * Created: 10/26/2019 9:42:05 PM
 *  Author: Ahmed
 */ 

 #include <avr/io.h>
 #include <avr/interrupt.h>

#ifndef EEPROM_LIB_H_
#define EEPROM_LIB_H_


void EEPROM_write(uint8_t eeprom_address,uint8_t eeprom_data);
uint8_t EEPROM_read(uint8_t eeprom_address);



#endif /* EEPROM_LIB_H_ */