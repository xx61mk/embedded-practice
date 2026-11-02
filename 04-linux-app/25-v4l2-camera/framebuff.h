#ifndef __FRAMEBUFF_H__
#define __FRAMEBUFF_H__

#include <linux/fb.h>

/* 單個像素點類型 */
typedef struct __rgb__ 
{
    unsigned char B;                    //藍色
    unsigned char G;                    //綠色
    unsigned char R;                    //紅色
    unsigned char Reserved;             
}rgb_t;

/* 屏幕操作句柄 */
typedef struct __fb__ 
{
    struct fb_var_screeninfo vscreeninfo;//屏幕參數
    rgb_t *prgb;                        //顯存空間首地址
    int fd;                             //屏幕文件描述符
}fb_t;

/* 漢字結構 */
typedef struct __hanzi__ 
{
    char str[4];                        //漢字的字符串
    unsigned char zimo[32];             //漢字字模
}hanzi_t;

extern unsigned char fontdata_8x16[4096];
extern hanzi_t ghanzi[50];

extern fb_t *framebuff_init(const char *pdevname);
extern void framebuff_deinit(fb_t *pfb);
extern int draw_pixel(fb_t *pfb, int y, int x, unsigned char r, unsigned char g, unsigned b);
extern int draw_hor_line(fb_t *pfb, int y, int x, int width, unsigned char r, unsigned char g, unsigned char b);
extern int draw_ver_line(fb_t *pfb, int y, int x, int height, unsigned char r, unsigned char g, unsigned char b);
extern int draw_dict(fb_t *pfb, int y, int x, int width, int height, unsigned char r, unsigned char g, unsigned char b);
extern int fill_dict(fb_t *pfb, int y, int x, int width, int height, unsigned char r, unsigned char g, unsigned char b);
extern void clear_screen(fb_t *pfb);
extern void draw_ascii(fb_t *pfb, int y, int x, unsigned char ch,
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb);
extern void draw_string(fb_t *pfb, int y, int x, char *pstr,
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb);
extern void draw_hanzi(fb_t *pfb, int y, int x, char *pstr, 
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb);
extern void draw_hanzi_string(fb_t *pfb, int y, int x, char *pstr, 
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb);
extern int draw_memrgb(fb_t *p, int x, int y, rgb_t *pmem, int width, int height);

#endif