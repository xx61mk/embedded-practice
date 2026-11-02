#include <reg51.h>
#include "timer.h"
//#include "led.h"
#include "k.h"

#define HZ_200 63035
#define HZ_400 64285 
#define	HZ_600 64768
#define	HZ_800 64960
#define	HZ_1000	65075
#define HZ_2000 65366

extern unsigned short g_x;
int main(void)
{
	int ret=0;
	led_init();
	timer0_init();
	key_init();
	while (1)
	{
		ret=key_press();
		if(ret==1)
		{
			g_x=HZ_400;
		}
		else if(ret==2)
		{
			g_x=HZ_600;
		}
			else if(ret==3)
		{
			g_x=HZ_800;
		}
			else if(ret==4)
		{
			g_x=HZ_1000;
		}
	
	}
	return 0;
}