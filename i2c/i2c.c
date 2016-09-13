#define MPU6050Addr 0x68
#define MPU6050ReadAddr 0xD1
#define MPU6050WriteAddr 0xD0
#define F_CPU 12000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<util/twi.h>
#define waitTillDone while(TWCR&(1<<TWINT)==0);
#define SCL_CLOCK 400000

/*
*	This library is used for i2c communication. 
*	This currently used peter flury's code(opensourced.) 
*	This will be soon replaced.
*/


void TWIInit(void){
	//Initialise the TWI hardware at 400kHz
	TWSR = 0x0;	//clean status register
	TWBR = 0X07;	// Prescaler set for SCL 400kHz
	TWCR = (1<<TWEN)|(1<<TWINT);
	return;
}

// void TWIStart(void){
// 	//TWCR controls the TWI hardware
// 	//TWSTA sends start signal.
// 	//TWINT should be cleared by setting it.This is set at the end of the action by hardware
// 	//TWEN enables the TWI
// 	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
// 	waitTillDone;
// }
//
// void TWIStop(void){
// 	//TWSTO sends stop signal
// 	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
// }
//
// void TWIWrite(uint8_t data){
// 	//This sends data level 1  abstraction.
// 	//TWDR holds data to be written
// 	TWDR = data;
// 	TWCR = (1<<TWINT)|(1<<TWEN);
// 	waitTillDone;
// }
//
// uint8_t TWIReadACK(void){
// 	//Read data and acknowledge
// 	uint8_t x;
// 	x=TWDR;
// 	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
// 	waitTillDone;
// 	_delay_us(50);
// 	return TWDR;
// }
//
// uint8_t TWIReadNACK(void){
// 	uint8_t x;
// 	x=TWDR;
// 	//Read data and acknowledge
// 	TWCR = ((1<<TWINT)|(1<<TWEN));
// 	waitTillDone;
// 	_delay_us(50);
// 	return TWDR;
// }
//
// uint8_t TWIGetStatus(void){
// 	uint8_t status;
// 	status = TWSR&0xf8;
// 	return status;
//
// }
//
// void TWIWriteReg(uint8_t reg, uint8_t data){
//         TWIStart();
//
//         _delay_us(10);
//         while(TWIGetStatus()!=0x08);
//         TWIWrite(MPU6050WriteAddr);
//
//         _delay_us(10);
//         while(TWIGetStatus()!=0x18);
//         TWIWrite(reg);
//         _delay_us(10);
//
//         while(TWIGetStatus()!=0x28);
//         TWIWrite(data);
//
//         while(TWIGetStatus()!=0x28);
//         TWIStop();
//         return;
//
// }
//
//
// uint8_t TWIReadReg(uint8_t reg){
// 	uint8_t regdata;
// 	TWIStart();
//
//         _delay_us(10);
//         while(TWIGetStatus()!=0x08);
//         TWIWrite(MPU6050WriteAddr);
//
//         _delay_us(10);
//         while(TWIGetStatus()!=0x18);
//         TWIWrite(reg);
//         _delay_us(10);
//
//         while(TWIGetStatus()!=0x28);
//         TWIStart();
//         _delay_us(10);
//
//         while(TWIGetStatus()!=0x10);
//         TWIWrite(MPU6050ReadAddr);
//         _delay_us(10);
//
//         while(TWIGetStatus()!=0x40);
//
//         regdata = TWIReadNACK();
//          _delay_us(10);
//         while(TWIGetStatus()!=0x58);
//
//         TWIStop();
//
//         return regdata;
//
//
// }
//



// from peter flury

unsigned char i2c_start(unsigned char address)
{
    uint8_t   twst;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;
}/* i2c_start */


unsigned char i2c_write( unsigned char data )
{
    uint8_t   twst;

	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}/* i2c_write */

unsigned char i2c_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;

}/* i2c_readAck */


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition

 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;

}/* i2c_readNak */


void i2c_stop(void)
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}/* i2c_stop */

void i2c_init(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */

  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */

}/* i2c_init */
