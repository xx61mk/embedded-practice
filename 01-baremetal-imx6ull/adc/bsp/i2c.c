#include "i2c.h"
#include "delay.h"
#include "fsl_iomuxc.h"

// I2CR
#define  IEN    (7)
#define  MSTA   (5)
#define  MTX    (4)
#define  TXAK   (3)
#define  RSTA   (2)

// I2SR
#define  ICF    (7)
#define  IBB    (5)
#define  IAL    (4)
#define  IIF    (1)

// i2c1初始化
void i2c1_init(void)
{
    // 配置引脚复用功能
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);
    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);

    // 配置引脚电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0xF0B0);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0xF0B0);

    I2C1->I2CR &= ~(1 << 7);    // I2C1失能
    I2C1->IFDR = 0x15;          // 640分频
    I2C1->I2CR |= (1 << 7);     // I2C1使能
}

void i2c_write(I2C_Type *base, unsigned char dev_address, unsigned short reg_address, int reg_len, const unsigned char *data, int len)
{
    // 通信前 仲裁丢失标志位清零 + 中断标记位清零
    base->I2SR &= ~((1 << IIF) | (1 << IAL));
    // 通信前，等待数据传输完毕
    while ((base->I2SR & (1 << ICF)) == 0);
    // 发送起始信号
    base->I2CR |= (1 << MSTA);
    // 主机模式：发送模式
    base->I2CR |= (1 << MTX);
    // 后续检测从机ACK信号
    base->I2CR &= ~(1 << TXAK);
    // 中断标记位清零
    base->I2SR &= ~(1 << IIF);
    // 发送设备地址 + 写位
    base->I2DR = dev_address << 1;   // 101 0000 0
    // 等待从机ACK
    while ((base->I2SR & (1 << IIF)) == 0);

    int i = 0; 
    for (i = 0; i < reg_len; i++)
    {
        // 中断标记位清零
        base->I2SR &= ~(1 << IIF);
        // 发送寄存器地址
        base->I2DR = reg_address >> (reg_len - i - 1) * 8;
        // 等待从机ACK
        while ((base->I2SR & (1 << IIF)) == 0);
    }

    while (len--)
    {
        // 中断标记位清零
        base->I2SR &= ~(1 << IIF);
        // 发送数据
        base->I2DR = *data++;
        // 等待从机ACK
        while ((base->I2SR & (1 << IIF)) == 0);
    }

    // 发送停止信号
    base->I2CR &= ~(1 << MSTA);

    // 等待总线空闲
    while ((base->I2SR & (1 << IBB)) != 0)
    {
        delay_us(100);
    }
}

void i2c_read(I2C_Type *base, unsigned char dev_address, unsigned short reg_address, int reg_len, unsigned char *data, int len)
{
   // 通信前 仲裁丢失标志位清零 + 中断标记位清零
    base->I2SR &= ~((1 << IIF) | (1 << IAL));
    // 通信前，等待数据传输完毕
    while ((base->I2SR & (1 << ICF)) == 0);
    // 发送起始信号
    base->I2CR |= (1 << MSTA);
    // 主机模式：发送模式
    base->I2CR |= (1 << MTX);
    // 后续检测从机ACK信号
    base->I2CR &= ~(1 << TXAK);
    // 中断标记位清零
    base->I2SR &= ~(1 << IIF);
    // 发送设备地址 + 写位
    base->I2DR = dev_address << 1;
    // 等待从机ACK
    while ((base->I2SR & (1 << IIF)) == 0);

    int i = 0; 
    for (i = 0; i < reg_len; i++)
    {
        // 中断标记位清零
        base->I2SR &= ~(1 << IIF);
        // 发送寄存器地址
        base->I2DR = reg_address >> (reg_len - i - 1) * 8;
        // 等待从机ACK
        while ((base->I2SR & (1 << IIF)) == 0);
    }

    // 重发起始信号
    base->I2CR |= (1 << RSTA);
    // 中断标记位清零
    base->I2SR &= ~(1 << IIF);
    // 重发设备地址 + 读位
    base->I2DR = dev_address << 1 | 1;
    // 等待从机回复ACK信号
    while ((base->I2SR & (1 << IIF)) == 0);
    // 主机模式：接收模式
    base->I2CR &= ~(1 << MTX);
    // 中断标记位清零
    base->I2SR &= ~(1 << IIF);

    // 代表只需要读取一个字节
    if (len == 1)
    {
        // 代表收到一个字节后，后续回复NACK
        base->I2CR |= (1 << TXAK);
    }

    // 假读
    *data = base->I2DR;

    // 读取len个字节数据
    while (len-- != 0)
    {   
        // 代表收到一个字节数据，并且主机回复了ACK
        while ((base->I2SR & (1 << IIF)) == 0);
        // 中断标记位清零
        base->I2SR &= ~(1 << IIF);

        // 代表数据全部接收完
        if (len == 0)
        {
            // 发送停止信号
            base->I2CR &= ~(1 << MSTA);
            // ACK
            base->I2CR &= ~(1 << TXAK);
            // 等待总线空闲
            while ((base->I2SR & (1 << IBB)) != 0)
            {
                delay_us(100);
             }       
        }
        else if (len == 1)
        {
            // NACK
            base->I2CR |= (1 << TXAK);
        }

        *data++ = base->I2DR;
    }
}

void master_xfer(I2C_Type *base, I2C_Msg *msg)   
{
    if (msg->direction == I2C_Write)
    {
        i2c_write(base, msg->dev_address, msg->reg_address, msg->reg_len, msg->data, msg->len);
    }
    else if (msg->direction == I2C_Read)
    {
        i2c_read(base, msg->dev_address, msg->reg_address, msg->reg_len, msg->data, msg->len);       
    }
}