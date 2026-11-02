#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
int main(int argc, char *argv[])
{
    int fd = open("/dev/i2c-0",O_RDWR);
    if(fd<0)
    {
        perror("open iic\n");
        return 1;
    }
    // 设置从机 地址
    ioctl(fd,I2C_SLAVE,0x4c);

    char cmd[2]={0x01,0x00};
    char data[2]={0};
    write(fd,cmd,2);

    // 发送 0x00 reg, 准备读
    cmd[0]=0x00;
    write(fd,cmd,1);

    read(fd,data,2);
    
    printf("temp:%d\n",data[0]);

    return 0;
}
