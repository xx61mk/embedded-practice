#include "interrupt.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"

irq_handler_t interrupt_vector_table[160];   // 函数地址

void system_interrupt_init(void)
{
    // 初始化GIC控制器
    GIC_Init(); 
}

void system_interrupt_register(int num, irq_handler_t handler)
{
    interrupt_vector_table[num] = handler;
}

void system_interrupt_handler(int num) 
{
    if (interrupt_vector_table[num] != NULL)
    {
        interrupt_vector_table[num]();   
    }
}