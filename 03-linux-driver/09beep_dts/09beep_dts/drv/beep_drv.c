/**
 * @file beep_drv.c
 * @author yas (rage_yas@hotmail.com)
 * @brief
 这个一个beep 的测试的驱动

 GPIO5_1
 低电平 峰鸣
 不电平 不峰鸣

 1.加入 misc_register
2. dts
 sysfs
 * @version 0.1
 * @date 2026-10-09
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "asm-generic/errno-base.h"
#include "asm/uaccess.h"
#include "linux/device.h"
#include "linux/export.h"
#include "linux/fs.h"
#include "linux/kdev_t.h"
#include "linux/printk.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
// static dev_t dev_no;  // 主设备号(12bit)+次设备号(20bit)  32bit  ，

static int myopen(struct inode *node, struct file *f);
static int myrelease(struct inode *node, struct file *f);
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off);
static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off);

static ssize_t myshow(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t mystore(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

// gpio5_1
#define BIT_OFFSET 1
#define DEV_NAME "beep"
static volatile void *io_mux = NULL;
static volatile void *io_pad = NULL;
static volatile void *io_data = NULL;
static volatile void *io_dir = NULL;

struct device_node *pnode = NULL;

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = myopen,
    .release = myrelease,
    .write = mywrite,
    .read = myread,
};

static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME,
    .fops = &fops,
};
static struct device_attribute attr = __ATTR(beep_state, 0664, myshow, mystore);

// cat /sys/class/misc/beep/beep_start
static ssize_t myshow(struct device *dev, struct device_attribute *attr, char *buf)
{
  pr_info("myshow caller\n");
  return strlen(buf);
}

// echo "1" >/sys/class/misc/beep/beep_start   1\n
static ssize_t mystore(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
  u32 v = 0;
  if (0 == strcmp(buf, "1\n"))
  {
    // beep_on
    v = readl(io_data);
    v &= ~(1 << BIT_OFFSET);  //   0000 0001  ->1111 1110
    writel(v, io_data);
  }
  else if (0 == strcmp(buf, "0\n"))
  {
    // beep_off
    v = readl(io_data);
    v = v | (1 << BIT_OFFSET);
    writel(v, io_data);
  }
  return count;
}

static int myopen(struct inode *node, struct file *f)
{
  return 0;
}
static int myrelease(struct inode *node, struct file *f)
{
  pr_info("close call...\n");
  return 0;
}
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off)
{
  return 0;
}

// 1 -> beep on   0 -> beep_off

static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off)
{
  unsigned long ret = 0;
  int beep_start = 0;
  u32 v = 0;

  ret = copy_from_user(&beep_start, data, 4);

  if (0 == beep_start)
  {
    // beep off
    v = readl(io_data);
    v = v | (1 << BIT_OFFSET);
    writel(v, io_data);
  }
  else if (1 == beep_start)
  {
    // beep_on
    v = readl(io_data);
    v &= ~(1 << BIT_OFFSET);  //   0000 0001  ->1111 1110
    writel(v, io_data);
  }
  return size;
}

static int __init beep_init(void)
{
  int ret = 0;
  u32 v = 0;
 // u32 reg_arr[8] = {0};
  ret = misc_register(&misc_dev);
  if (ret < 0)
  {
    pr_info("misc_register error\n");
    return ret;
  }

  pnode = of_find_node_by_path("/putebeep");
  if (!pnode)
  {
    pr_err("beep_init of_find_node_by_path error\n");
    return -ENODEV;
  }

  // ret = of_property_read_u32_array(pnode, "reg", reg_arr, 8);
  // if (ret)
  // {
  //   pr_err("beep_init of_property_read_u32_array error\n");
  //   return ret;
  // }
  //初始化  复用寄存器，电气寄存器，方向寄存器，数据寄存器

  // io_mux = ioremap(IO_MUX, 4);
  // io_pad = ioremap(IO_PAD, 4);
  // io_data = ioremap(IO_DATA, 4);
  // io_dir = ioremap(IO_DIR, 4);
  io_mux = of_iomap(pnode, 0);
  io_pad = of_iomap(pnode, 1);
  io_data = of_iomap(pnode, 2);
  io_dir = of_iomap(pnode, 3);

  // 设置复用寄存器 -> GPIO
  v = readl(io_mux);
  v = v | 0x05;
  writel(v, io_mux);

  // 设置电气特性寄存器 -> 默认设置
  v = readl(io_pad);
  v = 0x10a0;
  writel(v, io_pad);

  // 设置GPIO5_1  1  beep_off
  v = readl(io_data);
  v = v | (1 << BIT_OFFSET);
  writel(v, io_data);

  // 设置GPIO5_1  output
  v = readl(io_dir);
  v = v | (1 << BIT_OFFSET);
  writel(v, io_dir);

  device_create_file(misc_dev.this_device, &attr);
  pr_info("beep drv ok\n");
  return 0;
}
static void __exit beep_exit(void)
{
  device_remove_file(misc_dev.this_device, &attr);
  of_node_put(pnode);
  iounmap(io_dir);
  iounmap(io_data);
  iounmap(io_pad);
  iounmap(io_mux);

  misc_deregister(&misc_dev);

  return;
}

module_exit(beep_exit);
module_init(beep_init);
MODULE_LICENSE("GPL");
