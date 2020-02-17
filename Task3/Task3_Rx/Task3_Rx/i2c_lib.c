/*
 * i2c_lib.c
 *
 * Created: 10/18/2019 5:22:54 PM
 *  Author: Ahmed
 */ 

 #include "i2c_lib.h"



 void i2c_master_init(){
 // Set i2c clock
 TWBR = ((F_CPU/SCL_FREQ)-16)/2;

 // No Prescaler 
 TWSR &= ~((1<<TWPS0) | (1<<TWPS1));


 

 }




void i2c_start(unsigned char address){

 //Send START condition
 TWCR |= (1U<<TWEN) | (1U<<TWSTA) | (1U <<TWINT);
 TWCR &= ~(1<<TWSTO);

 // Wait for START transmission
 while( ! (TWCR & (1U<<TWINT) ) );

 //Sending address
 TWDR = address | TW_READ ;
 TWCR |= (1<<TWINT) | (1<<TWEN);
 TWCR &= ~((1<<TWSTA) | (1<<TWSTO));
 // Wait for address transmission
 while( ! (TWCR & (1U<<TWINT) ) );

}


unsigned char i2c_readack(){

  //Initialize receive
  TWCR = (1<<TWINT) | (1<<TWEN);

  //wait for TWINT flag to be set
  while(!(TWCR & (1<<TWINT)));

  return TWDR;

}


void i2c_stop(){
  //i2c STOP signal 
  //Send STOP condition
  TWCR |= (1U<<TWEN) | (1U<<TWSTO) | (1U<<TWINT);
  TWCR &= ~(1<<TWSTA);
  
  // Wait for STOP transmission
  while( TWCR & (1U<<TWSTO) );
}


unsigned char i2c_readnack(){
  TWCR |= (1<<TWINT) | (1<<TWEN);
  
  return TWDR;
}



void i2c_sendack(){
  TWCR |= (1<<TWINT) |(1<<TWEA);

}
void i2c_sendnack(){
  TWCR |= (1<<TWINT);


}