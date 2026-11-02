#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "lcd.h"

extern void lcd_drawline(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
extern void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
extern void lcd_draw_Circle(unsigned short x0,unsigned short y0,unsigned char r);
extern void lcd_showchar(unsigned     short x,unsigned short y,unsigned char num,unsigned char size, unsigned char mode);
extern unsigned int lcd_pow(unsigned char m,unsigned char n);
extern void lcd_shownum(unsigned short x, unsigned short y, unsigned int num, unsigned char len,unsigned char size);
extern void lcd_showxnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode);
extern void lcd_show_string(unsigned short x,unsigned short y, unsigned short width, unsigned short height, unsigned char size,char *p);
extern void lcd_fill(unsigned    short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int color);
#endif

