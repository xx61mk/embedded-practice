#include "adxl345.h"
#include "spi.h"
#include "gpio.h"

// adxl345写
void adxl345_write(unsigned char reg_addr, unsigned char data)
{
    // 拉低CS
    gpio_set_value(GPIO1, 20, 0);
    // 发送寄存器地址
    spi3_read_write(reg_addr);
    // 发送数据
    spi3_read_write(data);
    // 拉高CS
    gpio_set_value(GPIO1, 20, 1);   
}

// adxl345读
unsigned char adxl345_read(unsigned char reg_addr)
{
    unsigned char ret = 0;

    // 拉低CS
    gpio_set_value(GPIO1, 20, 0);
    // 发送寄存器地址
    spi3_read_write(reg_addr |0X80);  
    // 发送Dummy
    ret = spi3_read_write(0xFF);    
    // 拉高CS
    gpio_set_value(GPIO1, 20, 1);    

    return ret;
}

// adxl345初始化
void adxl345_init(void)
{
    // 关闭中断
    adxl345_write(0x2E, 0x08);
    // 设置满分辨率 + 设置满量程
    adxl345_write(0x31, 0x0B);
    // 低功耗模式 + 速率
    adxl345_write(0x2C, 0x08);
    // 配置测量模式 + 唤醒模式
    adxl345_write(0x2D, 0x0B);
}

// 获得adxl345三轴加速度
struct ADXL345_Data adxl345_collect(void)
{
    struct ADXL345_Data ret;

    // 读取x轴数据 
    ret.x = adxl345_read(0x32);
    ret.x |= adxl345_read(0x33) << 8;  
    // 读取y轴数据    
    ret.y = adxl345_read(0x34);
    ret.y |= adxl345_read(0x35) << 8;
    // 读取z轴数据    
    ret.z = adxl345_read(0x36);
    ret.z |= adxl345_read(0x37) << 8;    

    // 获得x y z三轴加速度
    ret.xg = ret.x * 0.0039;
    ret.yg = ret.y * 0.0039;
    ret.zg = ret.z * 0.0039;

    return ret;
}