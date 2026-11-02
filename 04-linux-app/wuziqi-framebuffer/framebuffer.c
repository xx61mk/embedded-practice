#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <math.h>
#include "framebuffer.h"

void *pmem = NULL;
int fb;
struct fb_var_screeninfo vinfo;

int fbx_g = 0;
int fby_g = 0;

int init_fb(char *devname)
{
    //1. 打开显示设备(/dev/fb0)
    fb = open(devname, O_RDWR);
    if (-1 == fb)
    {
        perror("open fb error");
        return -1;
    }

    //2. 获取显示设备相关参数（分辨率，像素格式）
   
    int ret = ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
    if (ret < 0)
    {
        perror("ioctl error");
        return -1;
    }
	
    printf("xres = %d, yres = %d\n", vinfo.xres, vinfo.yres);
  //  printf("xres_virtual = %d, yres_virtual = %d\n", vinfo.xres_virtual, vinfo.yres_virtual);
    printf("bits_per_pixel = %d\n", vinfo.bits_per_pixel);
	
	fbx_g = vinfo.xres;
	fby_g = vinfo.yres;

    //3. 建立显存空间和用户空间的内存映射
    size_t len = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel/8;
    pmem = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);
    if (pmem == MAP_FAILED)
    {
        perror("mmap error");
        return -1;
    }

    return 0;
}



void draw_point(int x, int y, unsigned int col)
{
    if (x >= vinfo.xres || y >= vinfo.yres)
    {
        return ;
    }
    if (vinfo.bits_per_pixel == RGB_FMT_888)
    {
        unsigned int *p = pmem;
        *(p+vinfo.xres_virtual*y+x) = col;
    }
    else if (vinfo.bits_per_pixel == RGB_FMT_565)
    {
        unsigned short *p = pmem;
        *(p+vinfo.xres_virtual*y+x) = col;
    }
}


void draw_h_line(int x, int y, int len, unsigned int col)
{
	for (int i = x; i < x+len; ++i)
	{
		draw_point(i, y, col);
	}
}

void draw_s_line(int x, int y, int len, unsigned int col)
{
	for (int i = y; i < y+len; ++i)
	{
		draw_point(x, i, col);
	}
}

void draw_clear(unsigned int col)
{
	for (int i = 0; i < fby_g; ++i)
	{
		draw_h_line(0, i, fbx_g, col);
	}
}

void draw_full_rect(int x, int y, int w, int h, unsigned int col)
{
	for (int i = y; i < y+h; ++i)
	{
		draw_h_line(x, i, w, col);
	}
}


void draw_circle(int x0, int y0, int r, unsigned int col)
{
	int x = 0, y = 0;
	int r0 = 0;

	for (r0 = 0; r0 <= r; ++r0)
	{
		for (float si = 0; si <= 360; si += 0.01)
		{
			x = x0 + r0 * cos(2*3.1416/360 * si);
			y = y0 + r0 * sin(2*3.1416/360 * si);
			draw_point(x, y, col);
		}
	}
}

/*********************************************
 *draw_bmp ：绘制bmp格式的图片
 *参数：
 *   x : 图像显示的起始横坐标
 *   y ：图像显示的起始从坐标
 *   bmpname ：bmp图片的名称
 * *********************************************/
void draw_bmp(int x, int y, char *bmpname)
{
#pragma pack(1)
//bmp文件相关信息
typedef struct tagBITMAPFILEHEADER {
    short    bfType;         // 文件类型标志
    int      bfSize;         // 文件大小，单位为字节
    short    bfReserved1;    // 保留字节
    short    bfReserved2;    // 保留字节
    int      bfOffBits;      // 数据偏移量，即实际图像数据开始的位置
}Bmp_file_head_t;
//bmp图像信息
typedef struct tagBITMAPINFOHEADER {
    int   biSize;         // BITMAPINFOHEADER的大小，单位为字节
    int    biWidth;        // 位图的宽度，单位为像素
    int    biHeight;       // 位图的高度，单位为像素
    short    biPlanes;       // 目标设备的位平面数，必须为1
    short    biBitCount;     // 每像素位数（颜色深度）
    int   biCompression;  // 图像压缩类型
    int   biSizeImage;    // 图像大小，单位为字节
    int    biXPelsPerMeter;// 水平分辨率，单位为像素/米
    int    biYPelsPerMeter;// 垂直分辨率，单位为像素/米
    int   biClrUsed;      // 实际使用颜色数
    int   biClrImportant; // 重要颜色数
}Bmp_info_t;
#pragma pack()

     Bmp_file_head_t headinfo;
	Bmp_info_t bmpinfo;
	
    int fd = open(bmpname, O_RDONLY);
    if (-1 == fd)
    {
        perror("open bmp error");
        return ;
    }

	read(fd, &headinfo, sizeof(headinfo));
	read(fd, &bmpinfo,  sizeof(bmpinfo));
    unsigned char *buff = malloc(bmpinfo.biHeight*bmpinfo.biWidth*bmpinfo.biBitCount/8);
    read(fd, buff, bmpinfo.biHeight*bmpinfo.biWidth*bmpinfo.biBitCount/8);
    close(fd);

    unsigned char *p = buff;
    unsigned char r, g, b;

    for (int j = bmpinfo.biHeight-1; j >= 0; j--)
    {
        for (int i = 0; i < bmpinfo.biWidth; i++)
        {
            b = *p;++p;
            g = *p;++p;
            r = *p;++p;
            unsigned int col = (r << 16) | (g << 8) | (b << 0);
			draw_point(i+x, j+y, col);
             
        }

    }

    free(buff);

}





