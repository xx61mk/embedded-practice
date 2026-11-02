#include "gt1148.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"
#include "gpio.h"
#include "i2c.h"
#include "delay.h"
#include "stdio.h"
#include "interrupt.h"

struct GT1148_Device gt1148_dev;


// gt9147读
void gt1148_read(unsigned short reg_addr, unsigned char *data, int len)
{
    I2C_Msg msg = 
    {
        .direction = I2C_Read,
        .dev_address = 0x5D,
        .reg_address = reg_addr,
        .reg_len = 2,
        .data = data,
        .len = len
    };

    master_xfer(I2C2, &msg);
}

// gt9147写
void gt1148_write(unsigned short reg_addr, unsigned char *data, int len)
{
    I2C_Msg msg = 
    {
        .direction = I2C_Write,
        .dev_address = 0x5D,
        .reg_address = reg_addr,
        .reg_len = 2,
        .data = data,
        .len = len
    };

    master_xfer(I2C2, &msg);
}

const unsigned short gt1148_reg[5]={0x8150,0x8158,0x8160,0x8168,0x8170};

void read_points(void)
{
    unsigned char buffer[16] = {0};
    gt1148_read(0x814E,buffer,1);
    gt1148_dev.num = buffer[0] & 0x0F;


    int i=0;
    for(i=0;i<gt1148_dev.num;i++)
    {
        gt1148_read(gt1148_reg[i],buffer,4);
        gt1148_dev.x[i] = buffer[1] <<8 | buffer[0];
        gt1148_dev.y[i] = buffer[3] <<8 | buffer[2];

    }

    if(gt1148_dev.num==0)
    {
        gt1148_dev.available=0;
    }
    else
    {
        gt1148_dev.available=1;

    }



    buffer[0]=0;
    gt1148_write(0x814E,buffer,1);

}

void gt1148_interrupt_handler()
{
    if((GPIO1->ISR &(1<<9))!=0)
    {   
        read_points();
        GPIO1->ISR |= (1<<9);
    }
}
// gt9147触控芯片初始化
void gt1148_init(void)
{
    // CT_RST 复用功能
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0);
    // CT_INT 复用功能
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0);
    // CT_RST 电气属性
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0x10B0);
    // CT_INT 电气属性
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0x10B0);   

    // 复位gt9147芯片
    GPIO_Type_t t = 
    {
        .direction = gpio_output,
        .default_value = 0
    };
    gpio_init(GPIO5, 9, &t);
    gpio_init(GPIO1, 9, &t);    

    gpio_set_value(GPIO5, 9, 0);   
    gpio_set_value(GPIO1, 9, 0);
    delay_ms(10);
    gpio_set_value(GPIO5, 9, 1);
    delay_ms(100);

    // INT引脚切换为悬浮输入
    t.direction = gpio_input;
    gpio_init(GPIO1, 9, &t);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0xF0B0);  

    // 读取gt9147 ID
    unsigned char buffer[16];
    gt1148_read(0x8140, buffer, 4);
    printf("gt1148_id: %s\n", (const char *)buffer);

    gt1148_read(0x804D,buffer,1);
    printf("way: %d\n", buffer[0] & 0x03);

    //配置引脚中断触发方式
    GPIO1->ICR1 |= (3<<18);
    GPIO1->IMR |= (1<<9);
    //注册gt1148中断函数

    system_interrupt_register(GPIO1_Combined_0_15_IRQn,gt1148_interrupt_handler);

    GIC_SetPriority(GPIO1_Combined_0_15_IRQn,0);
    //GIC使能中断
    GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);



}