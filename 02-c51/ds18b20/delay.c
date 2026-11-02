#include <intrins.h>
int delay(unsigned int n) 
{
	while (n--)
	{
	
	}

	return 0;
}

// 11.0592MHzæß’Ò∞Ê±æ - ”≈ªØ∞Ê
void Delay10us(unsigned int n) //@11.0592MHz
{
unsigned char data i;


_nop_();
_nop_();
_nop_();
i = 2 * n;
while (--i)
{			
	_nop_();
	_nop_();
}
}