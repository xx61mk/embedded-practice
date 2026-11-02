#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "core_ca7.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "interrupt.h"
#include "clock.h"


void delay(unsigned int n)
{
    while (n--);
}

int main(void)
{
    clock_init();
    system_interrupt_init();
    led_init();
    beep_init();
    key_init(); 

    while (1)
    {
        delay(0xFFFFF);
    }

    return 0;
}