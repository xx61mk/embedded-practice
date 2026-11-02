#ifndef _GT1148_H_
#define _GT1148_H_

struct GT1148_Device
{
    int available;        //数据有效标志
    int num;              //触摸点数量
    unsigned short x[5];  //触摸点X坐标，最多支持5点触摸
    unsigned short y[5];  //触摸点Y坐标，最多支持5点触摸
};
extern struct GT1148_Device gt1148_dev;

extern void gt1148_init(void);
extern void gt1148_read(unsigned short reg_addr, unsigned char *data, int len);
extern void gt1148_write(unsigned short reg_addr, unsigned char *data, int len);




#endif