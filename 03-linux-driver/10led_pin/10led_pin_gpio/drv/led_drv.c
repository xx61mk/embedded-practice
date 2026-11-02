/**
 * @file led_drv.c
 * @author yas (rage_yas@hotmail.com)
 * @brief
 1.这个一个led 的测试的驱动
 2.加入自动创建设备节点，不用在mknod
 3. 使用misc 方式，编写驱动
 4. 加入dts的支持
 5. pinctrl+ gpio

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
#include "linux/kdev_t.h"
#include "linux/kernel.h"
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

#define DEV_NAME "led0"  // /dev/led0
// static dev_t dev_no;     // 主设备号(12bit)+次设备号(20bit)  32bit  ，
// static struct cdev cdev;

static int myopen(struct inode *node, struct file *f);
static int myrelease(struct inode *node, struct file *f);
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off);
static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off);
static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

// #define IO_MUX 0x20E0068
// #define IO_PAD 0x20E02F4
// #define IO_DATA 0x209C000
// #define IO_DIR 0x209C004

// static volatile void *io_mux = NULL;
// static volatile void *io_pad = NULL;
// static volatile void *io_data = NULL;
// static volatile void *io_dir = NULL;

static struct device_node *pnode = NULL;

// static struct class *pclass = NULL;
// static struct device *pdev;
static struct device_attribute attr = __ATTR(led_state, 0664, show, store);
static int LED_GPIO = 0;
typedef enum
{
  MYLED_OFF,
  MYLED_ON
} MYLED_STATE;
MYLED_STATE myled_state;

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

// sys 目录下 led状态文件被读出调用
// cat /sys/class/LED/led0/led_state
// LED_OFF  LED_ON
ssize_t show(struct device *dev, struct device_attribute *attr, char *buf)
{
  switch (myled_state)
  {
    case MYLED_ON:
      strcpy(buf, "LED_ON\n");
      break;
    case MYLED_OFF:
      strcpy(buf, "LED_OFF\n");
      break;
    default:
      strcpy(buf, "LED_OFF\n");
  }
  return strlen(buf);
}
// sys 目录下 led状态文件被写入调用
// echo "1\n" > /sys/class/LED/led0/led_state
// echo "0\n" > /sys/class/LED/led0/led_state
ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
  unsigned long ret = 0;
  
  unsigned long val = 0;
  pr_info("store caller\n");
  ret = kstrtoul(buf, 0, &val);
  if (ret)
  {
    return ret;
  }
  pr_info("buf:[%s], to ul %lu\n", buf, val);
  if (0 == val)
  {
    // led off
    // v = readl(io_data);
    // v = v | (1 << 3);
    // writel(v, io_data);
    gpio_set_value(LED_GPIO,1);
    myled_state = MYLED_OFF;
    pr_info("want to off\n");
  }
  else
  {
    // led_on
    // v = readl(io_data);
    // v &= ~(1 << 3);  //   0000 0100  ->1111 1011
    // writel(v, io_data);
    // myled_state = MYLED_ON;
    gpio_set_value(LED_GPIO,0);
    pr_info("want to on\n");
  }
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
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off)
{
  pr_info("myread call...\n");
  return 0;
}

// 1 -> led on   0 -> led_off

static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off)
{
  unsigned long ret = 0;
  int led_start = 0;
  

  ret = copy_from_user(&led_start, data, 4);

  if (0 == led_start)
  {
    // led off
    // v = readl(io_data);
    // v = v | (1 << 3);
    // writel(v, io_data);
    gpio_set_value(LED_GPIO,1);
    myled_state = MYLED_OFF;
  }
  else if (1 == led_start)
  {
    // led_on
    // v = readl(io_data);
    // v &= ~(1 << 3);  //   0000 0100  ->1111 1011
    // writel(v, io_data);
     gpio_set_value(LED_GPIO,0);
    myled_state = MYLED_ON;
  }
  return size;
}

static int __init led_init(void)
{
  int ret = 0;
  u32 v = 0;
  u32 reg_arr[8] = {0};
  int i = 0;
  const char *dts_status = NULL;
  // 设备号的申请(10)，自动创建设备节点，杂项设备，属于字符设备的一种
  ret = misc_register(&misc_dev);
  if (ret < 0)
  {
    pr_err("misc_register error\n");
    return ret;
  }
  // 从设备树中，找到对应的节点。
  pnode = of_find_node_by_path("/puteleds");
  if (!pnode)
  {
    pr_err("of_find_node_by_path error\n");
    return -ENODEV;
  }

  // 从设备树获得，GPIO 的编号
  LED_GPIO = of_get_named_gpio(pnode, "gpio-leds", 0);
  if (LED_GPIO < 0)
  {
    pr_err("of_get_named_gpio error\n");
     return LED_GPIO;
    //goto addr_unmap;
  }

  // 向内核申请 这个gpio 的资源
  ret = gpio_request(LED_GPIO, "PTLED");
  if(ret)
  {
    pr_err("gpio_request error\n");
     return ret;
  }

  gpio_direction_output(LED_GPIO, 1);
  gpio_set_value(LED_GPIO, 1);

  ret = device_create_file(misc_dev.this_device, &attr);
  if (ret < 0)
  {
    pr_err("device_create_file error\n");
    // return ret;
    goto addr_unmap;
  }
  pr_info("led drv ok\n");

  return 0;
addr_unmap:
 
misc_del:
  misc_deregister(&misc_dev);
  return ret;
}
static void __exit led_exit(void)
{
  device_remove_file(misc_dev.this_device, &attr);
  of_node_put(pnode);

  misc_deregister(&misc_dev);

  return;
}

module_exit(led_exit);
module_init(led_init);
MODULE_LICENSE("GPL");
