#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// 定义中断函数指针类型，必须放在函数声明前面
typedef void (*irq_handler_t)(void);

void system_interrupt_init(void);
extern void system_interrupt_register(int num, irq_handler_t handler);
void system_interrupt_handler(int num);

#endif
