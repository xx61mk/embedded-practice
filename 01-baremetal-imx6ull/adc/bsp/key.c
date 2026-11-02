#include "key.h"
#include "gpio.h"
#include "led.h"
#include "beep.h"
#include "fsl_iomuxc.h"
#include "core_ca7.h"
#include "interrupt.h"

void key_interrupt_handler(void)
{
    if ((GPIO1->ISR & (1 << 18)) != 0) 
    {
        led_nor();
        beep_nor();
    }
    GPIO1->ISR |= (1 << 18);
}    

void key_init(void)
{
    // 引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    // 引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF0B0);
    // 按键引脚初始化
    GPIO_Type_t t = 
    {
        .direction = gpio_input
    };
    gpio_init(GPIO1, 18, &t);

    // 配置按键引脚中断触发方式：下降沿触发
    GPIO1->ICR2 |= (3 << 4);      

    // 使能引脚中断
    GPIO1->IMR |= (1 << 18);

    // 注册按键中断
    system_interrupt_register(GPIO1_Combined_16_31_IRQn, key_interrupt_handler); 

    // 设置按键中断优先级
    GIC_SetPriority(GPIO1_Combined_16_31_IRQn, 0);

    // GIC使能按键中断
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
}
int key_press(void)
{
    if (gpio_get_value(GPIO1, 18) == 0)
    {
        return 1;
    }
    else  
    {
        return 0;
    }
}


