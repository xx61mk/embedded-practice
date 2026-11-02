#include <reg51.h>
#include "k.h"
#include "digiter.h"
#include "delay.h"

int main(void)
{
	int ret = 0;
	key_init();
	int0_init();
	while(1)
	{
		
		Digiter_Show(ret);
	}
	return 0;

}