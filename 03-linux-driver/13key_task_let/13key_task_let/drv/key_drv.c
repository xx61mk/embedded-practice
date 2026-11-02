/**
 * @file key_drv.c
 * @author yas (rage_yas@hotmail.com)
 * @brief

 gpio1_18
低电平，按键按下
 1.这个一个key 的测试的驱动
 2.加入自动创建设备节点，不用在mknod
 3. 使用misc 方式，编写驱动
 4. 加入dts的支持
 5. pin+gpio
 6. 加入阻塞功能(waitque) +irq
 7.tasklet + workque(工作队列)

 * @version 0.1
 * @date 2026-10-09
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "asm-generic/errno-base.h"
#include "asm-generic/gpio.h"
#include "asm/gpio.h"
#include "asm/uaccess.h"
#include "linux/export.h"
#include "linux/fs.h"
#include "linux/interrupt.h"
#include "linux/kdev_t.h"
#include "linux/kernel.h"
#include "linux/of_irq.h"
#include "linux/printk.h"
#include "linux/string.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

#define DEV_NAME "key0"  // /dev/key0
// static dev_t dev_no;     // 主设备号(12bit)+次设备号(20bit)  32bit  ，
// static struct cdev cdev;

extern void msleep(unsigned int msecs);

#define BITS_OFFSET 18
static int myopen(struct inode *node, struct file *f);
static int myrelease(struct inode *node, struct file *f);
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off);
static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off);
static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
static void task_handle(unsigned long);
void work_hanled(struct work_struct *work);
static irqreturn_t myhandler(int, void *);

static struct device_node *pnode = NULL;

static struct device_attribute attr = __ATTR(key_state, 0664, show, store);
static int key_gpio = 0;
static wait_queue_head_t wait_que;
static int wait_que_cond = 0;
static int irq_no = 0;
DECLARE_TASKLET(mytasklet, task_handle, 0);
static struct work_struct mywork;
typedef enum
{
  MYkey_OFF,
  MYkey_ON
} MYkey_STATE;
MYkey_STATE mykey_state;

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = myopen,
    .release = myrelease,
    .write = mywrite,
    .read = myread,
};

static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,  // 动态分配次设备号
    .name = DEV_NAME,             // 在/dev目录下设备名，-> open
    .fops = &fops,
};

// sys 目录下 key状态文件被读出调用
// cat /sys/class/key/key0/key_state
// key_OFF  key_ON
ssize_t show(struct device *dev, struct device_attribute *attr, char *buf)
{
  //  1 11100  00 0 0 00 0110
  if (gpio_get_value(key_gpio))  //  0100 0000 0000 0000 0000
  {
    // key up
    // key_status = 0;
    strcpy(buf, "key_up");
  }
  else
  {
    // key press
    // key_status = 1;
    strcpy(buf, "key_down");
  }
  return strlen(buf);
}
// sys 目录下 key状态文件被写入调用
// echo "1\n" > /sys/class/key/key0/key_state
// echo "0\n" > /sys/class/key/key0/key_state
ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
  pr_info("store call...\n");
  return count;
}

static int myopen(struct inode *node, struct file *f)
{
  pr_info("myopen call...\n");
  return 0;
}
static int myrelease(struct inode *node, struct file *f)
{
  pr_info("close call...\n");
  return 0;
}

void work_handle(struct work_struct *work)
{
  // 第半部， 可以打断，可以休眠
  wait_que_cond = 1;
  wake_up(&wait_que);
  msleep(1);
  return ;
}

//底半部 处理
void task_handle(unsigned long arg)
{
  //不会打断，不可休眠
  schedule_work(&mywork);// 线程化处理。
  // msleep(10);
  return;
}

//顶半部  处理 ，不能作耗时操作，不被高优先抢断。
irqreturn_t myhandler(int irq_no, void *arg)
{
  //需要调度 tasklet
  tasklet_schedule(&mytasklet);
  return IRQ_HANDLED;
}
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off)
{
  int ret = 0;
  u32 key_status = 0;
  wait_que_cond = 0;
  wait_event_interruptible(wait_que, wait_que_cond);
  if (gpio_get_value(key_gpio))
  {
    // key up
    key_status = 0;
  }
  else
  {
    // key press
    key_status = 1;
  }

  ret = copy_to_user(data, &key_status, sizeof(key_status));
  if (ret)
  {
    pr_err(" myread copy_to_user error\n");
    return -EFAULT;
  }

  return sizeof(key_status);
}

// 1 -> key on   0 -> key_off

static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off)
{
  pr_info("mywrite call...\n");

  return size;
}

static int __init mykey_init(void)
{
  int ret = 0;
  // 设备号的申请(10)，自动创建设备节点，杂项设备，属于字符设备的一种
  ret = misc_register(&misc_dev);
  if (ret < 0)
  {
    pr_err("misc_register error\n");
    return ret;
  }
  // 从设备树中，找到对应的节点。
  pnode = of_find_node_by_path("/putekey");
  if (!pnode)
  {
    pr_err("of_find_node_by_path error\n");
    ret = -ENODEV;
    goto del_misc;
  }

  key_gpio = of_get_named_gpio(pnode, "gpio-key", 0);
  if (key_gpio < 0)
  {
    pr_err("of_get_named_gpio error\n");
    ret = -ENODEV;
    goto del_pnode;
  }

  ret = gpio_request(key_gpio, "key");
  if (ret)
  {
    pr_err("gpio_request error\n");
    goto del_pnode;
  }

  gpio_direction_input(key_gpio);
  gpio_set_value(key_gpio, 1);
  // 初始化  等待队列
  init_waitqueue_head(&wait_que);

  // 从设备树中，取出 中断号
  irq_no = of_irq_get(pnode, 0);
  if (irq_no < 0)
  {
    pr_err("of_irq_get error\n");
    ret = irq_no;
    goto del_gpio;
  }

  ret = request_irq(irq_no, myhandler, IRQF_TRIGGER_RISING, "key_irq", NULL);
  if (ret < 0)
  {
    pr_err("request_irq error\n");
    // return ret;
    goto del_gpio;
  }

  INIT_WORK(&mywork, work_handle);
  ret = device_create_file(misc_dev.this_device, &attr);
  if (ret < 0)
  {
    pr_err("device_create_file error\n");
    // return ret;
    goto del_irq;
  }
  pr_info("key drv ok\n");

  return 0;
del_irq:
  free_irq(irq_no, NULL);
del_gpio:
  gpio_free(key_gpio);
del_pnode:
  of_node_put(pnode);
del_misc:
  misc_deregister(&misc_dev);

  return ret;
}
static void __exit key_exit(void)
{
  device_remove_file(misc_dev.this_device, &attr);
  free_irq(irq_no, NULL);
  gpio_free(key_gpio);
  of_node_put(pnode);
  misc_deregister(&misc_dev);

  return;
}

module_exit(key_exit);
module_init(mykey_init);
MODULE_LICENSE("GPL");
