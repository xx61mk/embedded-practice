
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv)
{
  int fd = open("/dev/adxl345", O_RDWR);
  if (-1 == fd)
  {
    perror("open");
    return 1;
  }

  int on = 0;

  while (1)
  {
    char buf[6] = {0};
    read(fd, buf, 6);

    printf("x:%d y:%d z:%d\n", buf[0] | buf[1] << 8, buf[2] | buf[3] << 8, buf[4] | buf[5] << 8);
    sleep(1);
  }
  close(fd);
  // system("pause");
  return 0;
}
