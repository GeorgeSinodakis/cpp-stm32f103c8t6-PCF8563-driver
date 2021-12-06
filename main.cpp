#include <stdint.h>
#include <string>
#include <vector>
#include "nvic.h"
#include "tim2345.h"
#include "delay.h"
#include "rcc.h"
#include "gpio.h"
#include "adc.h"
#include "st7735.h"
#include "i2c.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

uint8_t decToBCD(uint8_t input)
{
	return ((input/10) << 4) + (input % 10);
}

uint8_t BCDtoDec(uint8_t input, uint8_t tens, uint8_t units)
{
	return ((input & tens) >> 4) * 10 + (input & units);
}

int main(void)
{
	clock_72Mhz();

	Led_init();
	Led_off();

	st7735_init(3);
	st7735_fill(BLACK);

	std::string str;
	u8 seconds;
	u8 minutes;
	u8 hours;

	I2C1_Init();

	// I2C1_Start();
	
	// I2C1_Send_Address(0xA2, I2C_write);

	// I2C1_Send_Data(0x00); //reg address

	// I2C1_Send_Data(0x00); //Control_status_1
	// I2C1_Send_Data(0x00); //Control_status_2

	// I2C1_Send_Data(0x01); //seconds
	// I2C1_Send_Data(0x55); //minutes
	// I2C1_Send_Data(0x07); //hours
	// I2C1_Send_Data(0x06); //days
	// I2C1_Send_Data(0x01); //weekdays
	// I2C1_Send_Data(0x12); //months
	// I2C1_Send_Data(0x21); //years

	// I2C1_Send_Data(0x80); //alarms
	// I2C1_Send_Data(0x80); //alarms
	// I2C1_Send_Data(0x80); //alarms
	// I2C1_Send_Data(0x80); //alarms

	// I2C1_Send_Data(0x83); //CLKOUT_control

	// I2C1_Send_Data(0x00); //Timer_control
	// I2C1_Send_Data(0x00); //Timer

	// I2C1_Stop();

	// Led_on();

	while (1)
	{
		I2C1_Start();
		I2C1_Send_Address(0xA2, I2C_write);
		I2C1_Send_Data(0x02);
		I2C1_Start();
		I2C1_Send_Address(0xA2, I2C_read);
		seconds = I2C1_Receive_Data(true);
		minutes = I2C1_Receive_Data(true);
		hours = I2C1_Receive_Data(false);
		I2C1_Stop();

		str = std::to_string(BCDtoDec(hours, 0b00110000, 0b00001111)) + ':' + 
		std::to_string(BCDtoDec(minutes, 0b01110000, 0b00001111)) + ':' + 
		std::to_string(BCDtoDec(seconds, 0b01110000, 0b00001111));
		st7735_print_str(5, 5, 2, WHITE, BLACK, &str);
		
		ms(1000);
		st7735_fill(BLACK);
	}
}