#ifndef _GPIO_H_
#define _GPIO_H_

#include "MCIMX6Y2.h"

// GPIO引脚方向枚举
enum GPIO_Direction
{
    gpio_input,
    gpio_output
};

// GPIO信息结构体
typedef struct 
{
    enum GPIO_Direction direction;       // 引脚方向
    int default_value;                   // 引脚默认电平
}GPIO_Type_t;

extern void gpio_init(GPIO_Type *base, int pin, GPIO_Type_t *gpio);
extern void gpio_set_value(GPIO_Type *base, int pin, int value);
extern int gpio_get_value(GPIO_Type *base, int pin);

#endif