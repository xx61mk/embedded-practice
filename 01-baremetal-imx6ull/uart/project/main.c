#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "core_ca7.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "interrupt.h"
#include "clock.h"
#include "epit.h"
#include "gpt.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"

int main(void)
{
    clock_init();
    system_interrupt_init();
    led_init();
    beep_init();
//   key_init(); 
//   epit1_init();
    gpt1_init();
    uart1_init();

    while (1)
    {
        int i = 0;
        int j = 0;

        scanf("%d%d", &i, &j);
        printf("%d + %d = %d\n", i, j, i+j);
    }

    return 0;
}