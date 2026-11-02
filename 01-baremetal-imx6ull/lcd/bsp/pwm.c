#include "pwm.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "interrupt.h"
#include "core_ca7.h"

float global_ratio = 1.0;

// 设置占空比
void set_ratio(void)
{
    int i = 0 ;

    for (i = 0; i < 4; i++)
    {
        PWM1->PWMSAR = PWM1->PWMPR * global_ratio;
    }
}


void pwm1_interrupt_handler(void)
{
    if ((PWM1->PWMSR & (1 << 3)) != 0)
    {
        set_ratio();
        PWM1->PWMSR |= (1 << 3);
    }
}

// pwm1初始化
void pwm1_init(void)
{
    // 1. pwm引脚复用
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 0);
    // 2. pwm引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT, 0xB9);

    PWM1->PWMCR = 0;
    PWM1->PWMCR |= (1 << 26) | (1 << 16) | (65 << 4) | (3 << 1);
    PWM1->PWMIR |= (1 << 0);
    PWM1->PWMPR = 998;
    set_ratio();

    // 注册pwm中断
    system_interrupt_register(PWM1_IRQn, pwm1_interrupt_handler);
    // 设置pwm中断优先级
    GIC_SetPriority(PWM1_IRQn, 0);
    // 使能pwm中断
    GIC_EnableIRQ(PWM1_IRQn);

    // 使能PWM
    PWM1->PWMCR |= (1 << 0);
}