
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv)
{
  int fd = open("/dev/beep", O_RDWR);
  if (-1 == fd)
  {
    perror("open");
    return 1;
  }

  int on = 0;

  while (1)
  {
    write(fd, &on, sizeof(on));
    sleep(1);
    if(0 == on)
    {
        on=1;
    }
    else if(1 == on)
    {
        on = 0;
    }
  }
  close(fd);
  // system("pause");
  return 0;
}
