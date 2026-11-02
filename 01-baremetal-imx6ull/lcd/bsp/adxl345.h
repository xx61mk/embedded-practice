#ifndef _ADXL345_H_
#define _ADXL345_H_

struct ADXL345_Data
{
    short x;
    short y;
    short z;
    float xg;
    float yg;
    float zg;
};

extern void adxl345_write(unsigned char reg_addr, unsigned char data);
extern unsigned char adxl345_read(unsigned char reg_addr);
extern void adxl345_init(void);
extern struct ADXL345_Data adxl345_collect(void);

#endif