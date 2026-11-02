#include "epit.h"
#include "led.h"
#include "interrupt.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"

void epit1_interrupt_handler(void)
{
    if ((EPIT1->SR & (1 << 0)) != 0)
    {
        led_nor();
    }
    EPIT1->SR |= (1 << 0);
}

void epit1_init(void)
{
    EPIT1->CR = 0;
    EPIT1->CR |= (1 << 24) | (1 << 17) | (65 << 4) | (1 << 3) | (1 << 2) | (1 << 1);
    EPIT1->LR = 1000000;
    EPIT1->CMPR = 0;

    // 注册epit1中断函数
    system_interrupt_register(EPIT1_IRQn, epit1_interrupt_handler);
    // 设置epit1中断优先级
    GIC_SetPriority(EPIT1_IRQn, 1);
    // GIC使能epit1中断
    GIC_EnableIRQ(EPIT1_IRQn);

    // 开启EPIT1定时器
    EPIT1->CR |= (1 << 0);
}