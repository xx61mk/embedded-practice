#ifndef _I2C_H_
#define _I2C_H_

#include "MCIMX6Y2.h"

extern void i2c1_init(void);
extern void i2c_write(I2C_Type *base, unsigned char dev_address, unsigned short reg_address, int reg_len, const unsigned char *data, int len);
extern void i2c_read(I2C_Type *base, unsigned char dev_address, unsigned short reg_address, int reg_len, unsigned char *data, int len);

// I2C传输方向
enum I2C_Direction
{
    I2C_Write = 0,
    I2C_Read = 1
};

// I2C信息
typedef struct
{
    unsigned char dev_address;
    unsigned short reg_address;
    int reg_len;
    unsigned char *data;
    int len;
    enum I2C_Direction direction;
}I2C_Msg;

extern void master_xfer(I2C_Type *base, I2C_Msg *msg) ;


#endif