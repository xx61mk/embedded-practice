#include <reg51.h>
#include "led.h"

void led_init(void)
{
	P2=0xFF;
}

void led_all_on(void)
{
	P2=0;
}

void led_all_off(void)
{
	P2=0xFF;	
}

void led_nor(void)
{
	P2 ^= 0xFF;
}

void led_on(unsigned int n)
{
	P2=~n;
}