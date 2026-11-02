#include <reg51.h>
#include "k.h"
unsigned int g_i=0;

void int0_handler(void)	     interrupt 0
{
	g_i++;
}

void int0_init(void)  
{							  
	//1.int0:P3_2引脚拉高
	P3 |= (1<<2);
	//2.外部中断子开关和CPU中断总开关
	IE |=(1<<0)|(1<<7);
	//3.外部中断0引脚中断触发方式：下降沿触发
	TCON|=(1<<0);
}


void key_init()
{
	P1 |=0X0F;
}

int key_press()
{
	int ret = 0;

	if((P1 & (1 << 4)) == 0)
	{
		ret = 1;
	}
	else if((P1 & (1 << 5)) == 0)
	{
		ret = 2;
	}
	else if((P1 & (1 << 6)) == 0)
	{
		ret = 3;
	}
	else if((P1 & (1 << 7)) == 0)
	{
		ret = 4;
	}

	return ret;
}


