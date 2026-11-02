#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "gpio.h"

void led_init(void)
{
    // 引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);

    // 引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);

    // LED引脚初始化
    GPIO_Type_t t =
    {
        .direction = gpio_output,
        .default_value = 1
    };
    gpio_init(GPIO1, 3, &t);
}

void led_on(void)
{
    gpio_set_value(GPIO1, 3, 0);
}

void led_off(void)
{
    gpio_set_value(GPIO1, 3, 1);
}

void led_nor(void)
{
    gpio_set_value(GPIO1, 3, !gpio_get_value(GPIO1, 3));    
}