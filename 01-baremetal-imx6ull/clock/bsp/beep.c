#include "beep.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "gpio.h"

void beep_init(void)
{
    // 配置引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0); 

    // 配置引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);

    // BEEP引脚初始化
    GPIO_Type_t t =
    {
        .direction = gpio_output,
        .default_value = 1
    };
    gpio_init(GPIO5, 1, &t);
}

void beep_on(void)
{
    gpio_set_value(GPIO5, 1, 0);
}

void beep_off(void)
{
    gpio_set_value(GPIO5, 1, 1);
}

void beep_nor(void)
{
    gpio_set_value(GPIO5, 1, !gpio_get_value(GPIO5, 1));   
}