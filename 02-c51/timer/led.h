#ifndef _LED_H_
#define _LED_H_

extern void led_init(void);
extern void led_all_on(void);
extern void led_all_off(void);
extern void led_nor(void);
extern void led_on(unsigned int n);

#endif