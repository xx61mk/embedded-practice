
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv)
{
  int fd = open("/dev/lm75", O_RDWR);
  if (-1 == fd)
  {
    perror("open");
    return 1;
  }

  int on = 0;

  while (1)
  {
    char buf[2] = {0};
    read(fd, buf, 2);
    printf("tmp:%d\n",buf[0]);
    sleep(1);
  }
  close(fd);
  // system("pause");
  return 0;
}
