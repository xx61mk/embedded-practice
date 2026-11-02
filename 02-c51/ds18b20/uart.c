#include <reg51.h>
#include "uart.h"

xdata char recv_buff[32];
unsigned int pos = 0;

// 串口接收中断服务函数
void uart_recv_handler(void)	interrupt 4	  
{
		// 判断是否收到一个字节数据
		if ((SCON & (1 << 0)) == 1)
		{
			// 判断是否越界
			if (pos < 32)
			{
				recv_buff[pos++] = SBUF;
				recv_buff[pos] = 0;
			}

			SCON &= ~(1 << 0);
		}
}


// 串口初始化
void uart_init(void)
{
	 // 设置UART工作模式：8位UART 波特率可变
	 SCON &= ~(3 << 6);   
	 SCON |= (1 << 6);
	 // 允许串口接收数据
	 SCON |= (1 <<4);
	 // 通过SCON寄存器中的SM0和SM1指定串口工作模式
	 PCON &= ~(1 << 6);
	 // 波特率翻倍
	 PCON |= (1 <<7);

	 // 配置定时器1工作模式:8位自动重装载
	 TMOD &= ~(0x0F << 4);
	 TMOD |= (1 << 5);
	 // 定时器初值：12MHZ:230  11.0592MHZ:232
	 TL1 = 232;
	 TH1 = 232;
	 // 打开定时器1
	 TCON |= (1 << 6);

	 // 开启CPU总中断 + 串口中断
	 IE |= (1 << 4) | (1 << 7);
}

// 发送单个字符
void uart_sendchar(unsigned char dat)
{
	SBUF = dat;

	// 判断上一个字节数据是否发送完毕
	while ((SCON & (1 << 1)) == 0);

	// 软件清零
	SCON &= ~(1 << 1);
}

// 发送字符串
void uart_sendstr(const char *p) 
{
	while (*p)
	{
		uart_sendchar(*p++);	
	}
}

// 发送指定长度数据
void uart_sendbuff(const char *p, int len)
{
	while (len--)
	{
		uart_sendchar(*p++);	
	}

}