#ifndef _KEY_H_
#define _KEY_H_
typedef void (*irq_handler_t)(void);
extern void key_init(void);
extern int key_press(void);

#endif