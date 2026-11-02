#include <reg51.h>
#include "digiter.h"
#include "delay.h"

// 共阴极段码表
unsigned char seg_table[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// 位选
void Select_Bit(unsigned int n)
{
	// P1寄存器低四位清零
	P1 &= ~(0x0F << 0);
	// 选中第n位数码管
	P1 |= (1 << n);
}

// 段选
void Select_Seg(unsigned int n)
{
	 P0 = seg_table[n];
}

// 数码管动态显示
void Digiter_Show(unsigned int n)  
{
    unsigned char i;
    unsigned char dat[4];
    dat[0] = n / 1000;        
    dat[1] = (n / 100) % 10;  
    dat[2] = (n / 10) % 10;   
    dat[3] = n % 10;          
	if(n==0)
	{
		Select_Bit(0) ;
		Select_Seg(0);
	}				
    for(i = 0; i < 4; i++)
    {
        Select_Bit(i);
        Select_Seg(dat[i]);
		delay(100);
        
    }
}
	

