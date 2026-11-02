#ifndef _UART_H_
#define _UART_H_

extern void uart1_init(void);
extern void putc(unsigned char ch);
extern void puts(const char *p);
extern unsigned char getc(void);

#endif