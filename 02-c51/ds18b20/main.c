#include "ds18b20.h"
#include "uart.h"
#include <stdio.h>

int main(void)
{
	float temp = 0;
	char s[32] = {0};

	uart_init();

	while (1)
	{
		 temp = get_temp();
		 sprintf(s, "temp = %.2f\n", temp);
		 uart_sendstr(s);
	}

	return 0;
}