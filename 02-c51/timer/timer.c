#include <reg51.h>
#include "timer.h"
#include "led.h"

#define HZ_200 63035
#define HZ_400 64285 
#define	HZ_600 64768
#define	HZ_800 64960
#define	HZ_1000	65075
#define HZ_2000 65366

unsigned short g_x=HZ_200;
// 定时器0中断服务函数
void timer0_handle(void) 	interrupt 1
{
	TL0 = g_x;
	TH0 = g_x >> 8;

	P2 ^=(1<<1);
   
}

// 定时器0初始化
void timer0_init(void)
{
	// 1. 配置定时器0模式：16位定时器
	TMOD &= ~(0x0F << 0);  
	TMOD |= (1 << 0);

	// 2. 设置定时器初值
	TL0 = g_x;
	TH0 = g_x >> 8;

	// 3. 开启定时器0，开始计数
	TCON |= (1 << 4);

	// 4. 开启定时器0中断 + CPU总中断
	IE |= (1 << 1) | (1 <<7);
}