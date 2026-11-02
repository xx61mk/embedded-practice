#include "video.h"
#include "framebuff.h"
#include <signal.h>

camera_t *pcamera = NULL;
fb_t *pfb = NULL;
unsigned char *pframe = NULL;
rgb_t *rgb_output = NULL;

void handler(int signo)
{
    capute_one_frame(pcamera, pframe);
    yuyv422_to_rgb888(pframe, rgb_output, VIDEO_WIDTH, VIDEO_HEIGHT);
    draw_memrgb(pfb, 0, 0, rgb_output, VIDEO_WIDTH, VIDEO_HEIGHT);

    return;
}

int main(void)
{
    int ret = 0;
    struct itimerval tv;

    pfb = framebuff_init("/dev/fb0");
    if (NULL == pfb)
    {
        fprintf(stdout, "framebuff_init failed\n");
        return -1;
    }

    clear_screen(pfb);
    
    pcamera = camera_init("/dev/video1");
    if (NULL == pcamera)
    {
        fprintf(stdout, "camera_init failed\n");
        return -1;
    }

    ret = camera_opendev(pcamera);
    if (ret) 
    {
        fprintf(stdout, "camera_opendev failed\n");
        return -1;
    }

    camera_showfmt(pcamera);

    ret = camera_setfmt(pcamera, VIDEO_WIDTH, VIDEO_HEIGHT, V4L2_PIX_FMT_YUYV);
    if (-1 == ret)
    {
        perror("uvc_camera_setfmt failed");
        return -1;
    }

    camera_startcap(pcamera);

    pframe = malloc(pcamera->mapbuffers[0].length);
    if (NULL == pframe)
    {
        perror("fail to malloc");
        return;
    }

    rgb_output = malloc(pfb->vscreeninfo.xres_virtual * pfb->vscreeninfo.yres_virtual * sizeof(rgb_t));
    if (NULL == rgb_output)
    {
        perror("fail to malloc");
        return;
    }

    signal(SIGALRM, handler);

    tv.it_value.tv_sec = 0;
    tv.it_value.tv_usec = 50000;  // 100ms = 100000us
    tv.it_interval.tv_sec = 0;
    tv.it_interval.tv_usec = 50000;
    setitimer(ITIMER_REAL, &tv, NULL);

    while (1)
    {
        pause();
    }
     
    free(rgb_output);
    free(pframe);

    framebuff_deinit(pfb);

    return 0;
}