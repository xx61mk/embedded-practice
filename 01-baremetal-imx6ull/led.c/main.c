


#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"


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
    led_init();

    while (1)
    {
        led_nor();
        delay(0xAFFFF);
    }

    return 0;
}