#include "gpio.h"

// gpio初始化
void gpio_init(GPIO_Type *base, int pin, GPIO_Type_t *gpio)  
{
    if (gpio->direction == gpio_output)
    {
        base->GDIR |= (1 << pin);
        if (gpio->default_value)
        {
            base->DR |= (1 << pin);
        }
        else    
        {
            base->DR &= ~(1 << pin);
        }
    }
    else if (gpio->direction == gpio_input)
    {
        base->GDIR &= ~(1 << pin);
    }
}

// gpio写
void gpio_set_value(GPIO_Type *base, int pin, int value)
{
    if (value)
    {
        base->DR |= (1 << pin);
    }
    else 
    {
        base->DR &= ~(1 << pin);  
    }
}

// gpio读 1高电平 0低电平
int gpio_get_value(GPIO_Type *base, int pin)
{
    return (base->DR & (1 << pin)) != 0;
}