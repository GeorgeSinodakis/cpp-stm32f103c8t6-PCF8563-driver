#include "rcc.h"
#include "i2c.h"

void I2C1_clock_enable(void)
{
    RCC_APB1ENR |= 1<<21;
}

void I2C1_Init(void)
{
	GPIOB_clock_enable();
	AFIO_clock_enable();
	I2C1_clock_enable();

	GPIOB_mode(OUTPUT_ALTFN_OPENDRAIN, 6); //PB6 SCL
	GPIOB_mode(OUTPUT_ALTFN_OPENDRAIN, 7); //PB7 SDA

	I2C1_CR2 = 36; //the APB clock frequency in MHz


	// I2C1_CCR |= (1<<15) | (1<<14) ;
	// I2C1_CCR |= 3;
	// I2C1_TRISE = 10;
	// I2C1_CR1 |= (1<<0);

	
	I2C1_CCR = 180;
	I2C1_TRISE = 37;
	I2C1_CR1 |= (1<<0);
}

void I2C1_Start(void)
{
	I2C1_CR1 |= (1<<8);
	while(!(I2C1_SR1 & (1<<0)));
}

void I2C1_Send_Address(uint8_t address, uint8_t RW)
{
	I2C1_DR = (address | RW);
	volatile uint8_t dummy;
	while(!(I2C1_SR1 & (1<<1)));
	dummy = I2C1_SR1;
	dummy = I2C1_SR2;
}

void I2C1_Send_Data(uint8_t data)
{
	while(!(I2C1_SR1 & (1<<7)));
	I2C1_DR = data;
}

uint8_t I2C1_Receive_Data(bool sendACK)
{
	if (sendACK) 
	{
		I2C1_CR1 |= (1<<10);
	}
	else
	{
		I2C1_CR1 &= ~(1<<10);
	}
	while(!(I2C1_SR1 & (1<<6)));
	return I2C1_DR;
}

void I2C1_Stop(void)
{
	if(I2C1_SR2 & 1)
	{
		while(!(I2C1_SR1 & (1<<2)));
	}
	I2C1_CR1 |= (1<<9);
}