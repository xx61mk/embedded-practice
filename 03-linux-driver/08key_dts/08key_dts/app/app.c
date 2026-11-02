
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv)
{
  int fd = open("/dev/key0", O_RDWR);
  if (-1 == fd)
  {
    perror("open");
    return 1;
  }

  int status = 0;

  while (1)
  {
   read(fd,&status,4);
   if(0 == status)
   {
    printf("key_relese\n");
   }
   else  
   {
    printf("key_press\n");
   }
  }
  close(fd);
  // system("pause");
  return 0;
}
