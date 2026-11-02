#include <reg51.h>
#include "k.h"
unsigned int g_i=0;

void int0_handler(void)		interrupt 0
{
	g_i++;
}

void int0_init(void)
{

	P3 |=(1<<2);
	IE |=(1<<0) |(1<<7);
	TCON |= (1<<0);
}
void key_init(void)
{
	P1 |=(0x0F<<4);
}


int key_press(void)
{
	int ret=0;
	if((P1 & (1<<4))==0)
	{
		ret=1;
	}
	else if((P1 & (1<<5))==0)
	{
		ret=2;
	}
	else if((P1 & (1<<6))==0)
	{
		ret=3;
	}
	else if((P1 & (1<<7))==0)
	{
		ret=4;
	}
	return ret;
}
