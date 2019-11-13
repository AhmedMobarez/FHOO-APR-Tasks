/*
 * i2c_lib.h
 *
 * Created: 10/18/2019 5:22:40 PM
 *  Author: Ahmed
 */
#define F_CPU (8000000)
#define SCL_FREQ  (100000)
#include <avr/io.h>
#include <util/twi.h>


#ifndef I2C_LIB_H_
#define I2C_LIB_H_

void i2c_master_init();
void i2c_slave_init(uint8_t address);
void i2c_start(unsigned char address);
void i2c_stop();
void i2c_write(unsigned char data);
unsigned char i2c_readack();
unsigned char i2c_readnack();




#endif /* I2C_LIB_H_ */