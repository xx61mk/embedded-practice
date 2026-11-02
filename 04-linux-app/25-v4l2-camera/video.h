#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <pthread.h>
#include "framebuff.h"

#define VIDEO_WIDTH     640
#define VIDEO_HEIGHT    480

//映射到图像缓存队列中空间数组首地址
typedef struct mapbuffer {
        void *                  start;
        size_t                  length;
}mapbuffer_t;

typedef struct uvc_camera
{
    char devname[20];                   //设备文件名

    int camerafd;                       //文件描述符

    struct v4l2_fmtdesc camerafmt;      //描述视频设备支持的像素格式(详见V4L2格式说明.txt)
    struct v4l2_capability cameracap;   //描述视频设备基本信息和能力(详见V4L2格式说明.txt)
    struct v4l2_frmsizeenum frmsize;    //枚举该格式的分辨率
    struct v4l2_format setfmt;          //设置视频设备的图像编码
    struct v4l2_requestbuffers viedoreq;//映射到图像缓存队列空间

    mapbuffer_t      *mapbuffers;       //映射到图像缓存队列中空间队列首地址
    unsigned int     mapbuffers_cnt;    //映射到图像缓存队列中空间个数

}camera_t;

extern int camera_opendev(camera_t *pcamera);
extern int camera_closedev(camera_t *pcamera);
extern int camera_showfmt(camera_t *pcamera);
extern camera_t *camera_init(const char *pdevname);
extern int camera_setfmt(camera_t *pcamera, int width, int height, unsigned int format);
extern int capute_one_frame(camera_t *pcamera, unsigned char *pframe);
extern int get_frame(camera_t *pcamera, unsigned char *pframe);
extern void yuyv422_to_rgb888(const unsigned char*yuyv_input, rgb_t *rgb_output, int width, int height);
extern int camera_startcap(camera_t *pcamera);
extern int camera_stopcap(camera_t *pcamera);

#endif