/**
 * @file th.c
 * @author yas (you@domain.com)
 * @brief 内核线程的测试
 * @version 0.1
 * @date 2026-10-14
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/completion.h>

extern void msleep(unsigned int msecs);
static struct completion my_com_h;
static struct completion my_com_w;
int th1(void *data)
{
  int i = 10;
  while (i--)
  {
    wait_for_completion(&my_com_h);
    pr_info("hello, ");
    complete(&my_com_w);
  }
  return 0;
}

int th2(void *data)
{
  int i = 10;
  while (i--)
  {
    wait_for_completion(&my_com_w);
    pr_info("world\n");
    complete(&my_com_h);
    msleep(1000);
  }

  return 0;
}

static int __init test_init(void)
{
  struct task_struct *task1 = NULL;
  struct task_struct *task2 = NULL;
  init_completion(&my_com_h);
  init_completion(&my_com_w);
  complete(&my_com_h);
  task1 = kthread_run(th1, NULL, "th1");
  task2 = kthread_run(th2, NULL, "th2");

  kthread_stop(task1);
  kthread_stop(task2);

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
