
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

// struct input_event {
// struct timeval time; // 事件的事件
// __u16 type; // 事件的类型
// __u16 code; // 对应的 键值
// __s32 value; // 按键按下或释放
// };

int main(int argc, char **argv)
{
  int fd = open("/dev/input/event1", O_RDWR);
  if (-1 == fd)
  {
    perror("open");
    return 1;
  }

  int status = 0;
  struct input_event ev;
  while (1)
  {
    bzero(&ev, sizeof(ev));
    read(fd, &ev, sizeof(ev));

    printf("type:%d code:%d value:%d\n", ev.type, ev.code, ev.value);
  }
  close(fd);
  // system("pause");
  return 0;
}
