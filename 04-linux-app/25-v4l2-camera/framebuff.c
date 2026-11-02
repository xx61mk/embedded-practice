#include "framebuff.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdio.h>
#include <sys/mman.h>

/* 打開屏幕設備，返回具有屏幕信息的句柄 */
fb_t *framebuff_init(const char *pdevname)
{
    int ret = 0;
    fb_t *pfb = NULL;

    pfb = malloc(sizeof(fb_t));
    if (NULL == pfb)
    {
        return NULL;
    }

    pfb->fd = open(pdevname, O_RDWR);
    if (-1 == pfb->fd)
    {
        return NULL;
    }

    ret = ioctl(pfb->fd, FBIOGET_VSCREENINFO, &pfb->vscreeninfo);
    if (ret) {
        return NULL;
    }

    printf("width:%d, height:%d\n", pfb->vscreeninfo.xres_virtual, pfb->vscreeninfo.yres_virtual);
    if (32 == pfb->vscreeninfo.bits_per_pixel) 
    {
        printf("format RGB888!\n");
    } 
    else if (16 == pfb->vscreeninfo.bits_per_pixel) 
    {
        printf("format RGB565\n");
    }

    pfb->prgb = mmap(NULL, pfb->vscreeninfo.xres_virtual * pfb->vscreeninfo.yres_virtual * pfb->vscreeninfo.bits_per_pixel / 8,
               PROT_READ | PROT_WRITE, MAP_SHARED, pfb->fd, 0);
    if (NULL == pfb->prgb) 
    {
        return NULL;
    }

    return pfb;
}

/* 銷毀屏幕信息的句柄 */
void framebuff_deinit(fb_t *pfb)
{
    if (pfb != NULL)
    {
        free(pfb);
    }

    return;
}

/* 繪制一個像素點 */
int draw_pixel(fb_t *pfb, int y, int x, unsigned char r, unsigned char g, unsigned b)
{
    (pfb->prgb + y * pfb->vscreeninfo.xres_virtual + x)->R = r;
    (pfb->prgb + y * pfb->vscreeninfo.xres_virtual + x)->G = g;
    (pfb->prgb + y * pfb->vscreeninfo.xres_virtual + x)->B = b;

    return 0;
}

/* 繪制橫線 */
int draw_hor_line(fb_t *pfb, int y, int x, int width, unsigned char r, unsigned char g, unsigned char b)
{
    int i = 0;

    for(i = 0; i < width; i++)
    {
        draw_pixel(pfb, y, x+i, r, g, b);
    }

    return 0;
}

/* 繪制豎線 */
int draw_ver_line(fb_t *pfb, int y, int x, int height, unsigned char r, unsigned char g, unsigned char b)
{
    int i = 0;

    for(i = 0; i < height; i++)
    {
        draw_pixel(pfb, y+i, x, r, g, b);
    }

    return 0;
}

/* 繪制矩形 */
int draw_dict(fb_t *pfb, int y, int x, int width, int height, unsigned char r, unsigned char g, unsigned char b)
{
    draw_hor_line(pfb, y, x, width, r, g, b);
    draw_hor_line(pfb, y+height, x, width, r, g, b);
    draw_ver_line(pfb, y, x, height, r, g, b);
    draw_ver_line(pfb, y, x+width, height, r, g, b);

    return 0;
}

/* 繪制實心矩形 */
int fill_dict(fb_t *pfb, int y, int x, int width, int height, unsigned char r, unsigned char g, unsigned char b)
{
    int i = 0;

    for (i = 0; i < height; i++)
    {
        draw_hor_line(pfb, y+i, x, width, r, g, b);
    }

    return 0;
}

/* 清屏 */
void clear_screen(fb_t *pfb)
{
    fill_dict(pfb, 0, 0, pfb->vscreeninfo.xres_virtual, pfb->vscreeninfo.yres_virtual, 0, 0, 0);

    return;
}

/* 繪制ASCII碼字符 */
void draw_ascii(fb_t *pfb, int y, int x, unsigned char ch,
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb)
{
    int j = 0;
    int i = 0;
    unsigned char flag = 0x80;

    for (j = 0; j < 16; j++)
    {
        for (flag = 0x80, i = 0; i < 8; i++, flag >>= 1)
        {
            if (fontdata_8x16[ch*16+j] & flag) 
            {
                draw_pixel(pfb, y+j, x+i, fr, fg, fb);
            }
            else 
            {
                draw_pixel(pfb, y+j, x+i, br, bg, bb);
            }
        }
    }

    return;
}

/* 繪制字符串 */
void draw_string(fb_t *pfb, int y, int x, char *pstr,
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb)
{
    int i = 0;

    while (*pstr != '\0')
    {
        draw_ascii(pfb, y, x+i*8, *pstr, fr, fg, fb, br, bg, bb);
        pstr++;
        i++;
    }

    return;
}

/* 顯示中文 */
void draw_hanzi(fb_t *pfb, int y, int x, char *pstr, 
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb)
{
    unsigned char zimo[32] = {0};
    int i = 0;
    int j = 0;
    unsigned char flag = 0x80;
    int n = 0;

    for (n = 0; n < sizeof(ghanzi) / sizeof(ghanzi[0]); n++)
    {
        if (0 == strncmp(ghanzi[n].str, pstr, 3))
        {
            break;
        }
    }
    
    if (n == (sizeof(ghanzi) / sizeof(ghanzi[0])))
    {
        return;
    }

    for (j = 0; j < 16; j++)
    {
        for (flag = 0x80, i = 0; i < 8; i++, flag >>= 1)
        {
            if (ghanzi[n].zimo[2*j] & flag) 
            {
                draw_pixel(pfb, y+j, x+i, fr, fg, fb);
            }
            else 
            {
                draw_pixel(pfb, y+j, x+i, br, bg, bb);
            }
        }

        for (flag = 0x80, i = 8; i < 16; i++, flag >>= 1)
        {
            if (ghanzi[n].zimo[2*j+1] & flag) 
            {
                draw_pixel(pfb, y+j, x+i, fr, fg, fb);
            }
            else 
            {
                draw_pixel(pfb, y+j, x+i, br, bg, bb);
            }
        }
    }

    return;
}

/* 繪制漢字字符串 */
void draw_hanzi_string(fb_t *pfb, int y, int x, char *pstr, 
                unsigned char fr, unsigned char fg, unsigned char fb,
                unsigned char br, unsigned char bg, unsigned char bb)
{
    int i = 0;

    while (*pstr != '\0')
    {
        draw_hanzi(pfb, y, x+i*16, pstr, fr, fg, fb, br, bg, bb);
        pstr += 3;
        i++;
    }

    return;
}

int draw_memrgb(fb_t *p, int x, int y, rgb_t *pmem, int width, int height)
{
    int j = 0;
    int i = 0;
    rgb_t *pdata = (rgb_t *)pmem;

    for (j = 0; j < height; j++) {
		memcpy(p->prgb + (j + y) * p->vscreeninfo.xres_virtual + x, 
			pdata + j * width, 
			width * sizeof(rgb_t));	
	}

    return 0;
}