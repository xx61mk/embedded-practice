#include "uart.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"

void uart1_init(void)
{
    // 配置串口引脚功能
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);

    // 配置串口引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);

    // 失能UART
    UART1->UCR1 &= ~(1 << 0);
    // 复位UART
    UART1->UCR2 &= ~(1 << 0);
    // 忽略RTS引脚
    UART1->UCR2 |= (1 << 14);
    // 无校验
    UART1->UCR2 &= ~(1 << 8);
    // 1位停止位
    UART1->UCR2 &= ~(1 << 6);
    // 8位数据位
    UART1->UCR2 |= (1 << 5);
    // 允许串口发送数据
    UART1->UCR2 |= (1 << 2);
    // 允许串口接收数据
    UART1->UCR2 |= (1 << 1);
    // 交叉模式
    UART1->UCR3 |= (1 << 2);
    // 设置1分频
    UART1->UFCR = (5 << 7);
    // 设置115200波特率
    UART1->UBIR = 999;
    UART1->UBMR = 43402;
    // 使能UART
    UART1->UCR1 |= (1 << 0);
}

void putc(unsigned char ch)
{
    while ((UART1->USR2 & (1 << 3)) == 0);

    UART1->UTXD = ch;
}

void puts(const char *p)
{
    while (*p)
    {
        putc(*p++);
    }
}

unsigned char getc(void)
{
    while ((UART1->USR2 & (1 << 0)) == 0);
    return UART1->URXD & 0xFF;       
}

void raise(void)
{
    
}