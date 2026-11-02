#include "MCIMX6Y2.h"
#include "lm75.h"
#include "i2c.h"

float get_lm75_temp(void)
{
    short temp;
    unsigned char s[2];

    I2C_Msg msg = 
    {
        .direction = I2C_Read,
        .dev_address = 0x48,
        .reg_address = 0,
        .reg_len = 1,
        .data = s,
        .len = 2
    };

    master_xfer(I2C1, &msg);
    temp = s[0] << 8;
    temp |= s[1];
    temp >>= 7;

    return temp * 0.5;
}