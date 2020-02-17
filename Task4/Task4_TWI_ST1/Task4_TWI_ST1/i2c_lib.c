/*
 * i2c_lib.c
 *
 * Created: 10/18/2019 5:22:54 PM
 *  Author: Ahmed
 */ 

 #include "i2c_lib.h"





void i2c_slave_init(uint8_t address){

 // Set i2c clock
 TWBR = ((F_CPU/SCL_FREQ)-16)/2;

 // No Prescaler
 TWSR &= ~((1<<TWPS0) | (1<<TWPS1));

  //Set slave address + R/W bit
  TWAR = address;

  //Enable interrupt
  TWCR |= (1<<TWIE);

  //slave initialization  sequence
  TWCR |= (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
  TWCR &= ~((1<<TWSTA) | (1<<TWSTO));

}


void i2c_start(unsigned char address){
//i2c START signal
TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
TWCR &= ~(1<<TWSTO);


}


void i2c_stop(){
//i2c STOP signal 
TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
TWCR &= ~(1<<TWSTA);

}


void i2c_write(unsigned char data){

TWDR = data;  
TWCR |= (1<<TWINT) | (TWEA);
}


unsigned char i2c_readack(){
TWCR |= (1<<TWINT) |(1<<TWEA);

return TWDR;
}
unsigned char i2c_readnack(){
TWCR |= (1<<TWINT);

return TWDR;
}