#include <reg51.h>
#include "led.h"
int delay(unsigned int n)
{	
	while(n--)
	{
		
	}
	return 0;
}
int main(void)
{
	int i=0;
	led_init();
	while(1)
	{
			for(i=0;i<=7;++i)
			{
				led_on(1<<i);
				delay(3000);
				led_all_off();
			}
			for(i=0;i>=0;--i)
			{
				led_on(1<<i);
				delay(3000);
				led_all_off();
			}
			
	

	}
	return 0;

}