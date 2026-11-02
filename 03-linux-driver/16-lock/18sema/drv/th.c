/**
 * @file th.c
 * @author yas (you@domain.com)
 * @brief 内核线程的测试
 信号量， 可以作互斥,同步，计数信号量 
 会阻塞，会调度。
 * @version 0.1
 * @date 2026-10-14
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>

static int count = 0;
static struct semaphore mysema;
int th1(void *data)
{
  int i = 5000;
  while (i--)
  {
    int tmp = 0;
    down(&mysema);
    tmp = count;
    pr_info("th name:%s,count is %d\n", current->comm, tmp + 1);
    count = tmp + 1;
    up(&mysema);
  }
  return 0;
}

int th2(void *data)
{
  int i = 5000;
  while (i--)
  {
    int tmp = 0;
    down(&mysema);
    tmp = count;
    pr_info("th name:%s,count is %d\n", current->comm, tmp + 1);
    count = tmp + 1;
    up(&mysema);
  }
  return 0;
}

static int __init test_init(void)
{
  struct task_struct *task1 = NULL;
  struct task_struct *task2 = NULL;
  sema_init(&mysema, 1);
  task1 = kthread_run(th1, NULL, "th1");
  task2 = kthread_run(th2, NULL, "th2");

  // kthread_stop(task1);
  // kthread_stop(task2);

  return 0;
}

static void __exit test_exit(void)
{
  return;
}

module_exit(test_exit);
module_init(test_init);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("YAS");
MODULE_DESCRIPTION("this is th test");
