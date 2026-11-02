#include <reg51.h>
#include "delay.h"
#include "digiter.h"

int main(void)
{
	// 选中第一位数码管
//	Select_Bit(3);

	// 显示0
//	Select_Seg(6);
	int i =0;
	while (1)
	{	 for(i=0;i<=9999;++i)
		{
		 	Digiter_Show(i);
		}	
	}

	return 0;
}