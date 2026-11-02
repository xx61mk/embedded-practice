#ifndef _LCD_H_
#define _LCD_H_

struct tftlcd_t 
{
    unsigned short height;
    unsigned short width;
    unsigned short pix_size;
    unsigned short vspw;
    unsigned short vbp;
    unsigned short vfp;
    unsigned short hspw;
    unsigned short hbp;
    unsigned short hfp;
    unsigned int frame_buffer;
    unsigned int fore_color;
    unsigned int back_color;
};

#define LCD_FRAMEBUF_ADDR   (0x89000000)

extern struct tftlcd_t lcd_dev;


extern void lcd_init(void);
extern void lcd_clear_screen(unsigned int col);
extern void lcd_drawpoint(int x, int y, unsigned int col);

#endif