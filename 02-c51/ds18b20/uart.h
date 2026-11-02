#ifndef _UART_H_
#define _UART_H_

extern void uart_init(void);
extern void uart_sendchar(unsigned char dat);
extern void uart_sendstr(const char *p);
extern void uart_sendbuff(const char *p, int len);

extern xdata char recv_buff[32];
extern unsigned int pos;

#endif