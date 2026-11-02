#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "core_ca7.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "interrupt.h"

// 使能外设时钟
void enable_clocks(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}

void delay(unsigned int n)
{
    while (n--);
}

int main(void)
{
    enable_clocks();
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