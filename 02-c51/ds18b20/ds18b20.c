#include <reg51.h>
#include <intrins.h>
#include "ds18b20.h"
#include "delay.h"
#include "uart.h"

#define DQ_PIN_LOW 	(P3 &= ~(1 << 7))   // DQ引脚拉低
#define DQ_PIN_HIGH (P3 |= (1 << 7))  	// DQ引脚拉高
#define DQ_CHECK	((P3 & (1 << 7)) != 0)  // 判断DQ引脚为高电平

// ds18b20 复位
int ds18b20_reset(void)
{
	int time = 0;

	// 主机向ds18b20发送复位脉冲
	DQ_PIN_LOW;
	Delay10us(70);
	DQ_PIN_HIGH;
	Delay10us(6);

	// 等待低电平到来 (超时时间)
	while (DQ_CHECK && time < 30)
	{
	   Delay10us(1);
	   time++;
	}
	if (time >= 30)
	{
		uart_sendstr("wait ds18b20_low timeout!\n");
		return -1;
	}

	// 等待高电平到来
	time = 0;
	while (!DQ_CHECK && time < 30)
	{
	   Delay10us(1);
	   time++;		
	}
	if (time >= 30)
	{
		uart_sendstr("wait ds18b20_high timeout!\n");
		return -2;		
	}

  	return 1;
}

// 主机发送一个字节
void write_ds18b20(unsigned char dat) 	
{
	int i = 0;

	for (i = 0; i < 8; i++)
	{
		if (dat & 1)    // 写1
		{
			DQ_PIN_LOW;
			_nop_();	
			_nop_();
			DQ_PIN_HIGH;
			Delay10us(6);	
		}
		else 		    // 写0
		{
			DQ_PIN_LOW;	
			Delay10us(6);
			DQ_PIN_HIGH;		
		}
		dat >>= 1;
	}
}

// 主机读取一个字节
unsigned char read_ds18b20(void)
{
	unsigned char dat = 0;
	int i = 0;

	for (i = 0; i < 8; i++)
	{
		DQ_PIN_LOW;	
		_nop_();	
		_nop_();
		DQ_PIN_HIGH;
		_nop_();	
		_nop_();
		_nop_();
		
		// 代表读到1
		if (DQ_CHECK)
		{
			dat |= (1 << i);
		}	
		Delay10us(6);
	}

	return dat;
}

// 读取ds18b20温度
float get_temp(void)
{
	unsigned char temp_low = 0;
	unsigned char temp_high = 0;
	short temp = 0;

	ds18b20_reset();	   // 复位
	write_ds18b20(0xCC);   // skip ROM
	write_ds18b20(0x44);   // 开启温度转换
   	Delay10us(1000);		   // 等待温度转换完毕 
	ds18b20_reset();	   // 复位
	write_ds18b20(0xCC);   // skip ROM
	write_ds18b20(0xBE);   // 读取温度

	temp_low = read_ds18b20();	  // 读取温度低位
	temp_high = read_ds18b20();	  // 读取温度高位

	// 2个字节温度值
	temp = 	temp_high << 8;          
	temp |= temp_low;	
	
	return temp * 0.0625;			 
}