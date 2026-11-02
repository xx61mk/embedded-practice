#include "spi.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "gpio.h"

// spi3_channel0初始化
void spi3_channel0_init(void)
{
    // 1. spi引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);   // CS
    IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);  // SCLK
    IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);    // MISO
    IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);    // MOSI

    // 2. spi引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10B1);

    // 3. 通信前，将CS拉高
    GPIO_Type_t t = 
    {
        .direction = gpio_output,
        .default_value = 1
    };
    gpio_init(GPIO1, 20, &t);

    // 4. SPI寄存器
    ECSPI3->CONREG = 0;
    ECSPI3->CONREG |= (7 << 20) | (0x0E << 12) | (2 << 8) | (1 << 4) | (1 << 3) | (1 << 0);

    ECSPI3->CONFIGREG = 0;
    ECSPI3->CONFIGREG |= (1 << 20) | (1 << 4) | (1 << 0);
}

// spi读写
unsigned char spi3_read_write(unsigned char data)
{
    while ((ECSPI3->STATREG & (1 << 0)) == 0);
    ECSPI3->TXDATA = data;

    while ((ECSPI3->STATREG & (1 << 3)) == 0);
    return ECSPI3->RXDATA;
}