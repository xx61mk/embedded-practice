#ifndef _KEY_H_
#define _KEY_H_

extern void system_interrupt_init(void);
extern void system_interrupt_register(int num, irq_handler_t handler);
extern void system_interrupt_handler(int num) ;

#endif