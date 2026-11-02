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

 * @version 0.1
 * @date 2026-10-09
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "asm-generic/errno-base.h"
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

#define DEV_NAME "key0"  // /dev/key0
// static dev_t dev_no;     // 主设备号(12bit)+次设备号(20bit)  32bit  ，
// static struct cdev cdev;

#define BITS_OFFSET 18
static int myopen(struct inode *node, struct file *f);
static int myrelease(struct inode *node, struct file *f);
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off);
static ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t *off);
static ssize_t show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

static volatile void *io_mux = NULL;
static volatile void *io_pad = NULL;
static volatile void *io_data = NULL;
static volatile void *io_dir = NULL;

static struct device_node *pnode = NULL;

// static struct class *pclass = NULL;
// static struct device *pdev;
static struct device_attribute attr = __ATTR(key_state, 0664, show, store);

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
  u32 v;
  v = readl(io_data);
  //  1 11100  00 0 0 00 0110
  if (v & (1 << BITS_OFFSET))  //  0100 0000 0000 0000 0000
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
static ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off)
{
  u32 v;
  u32 key_status = 0;
  unsigned long ret = 0;
  v = readl(io_data);
  //  1 11100  00 0 0 00 0110
  if (v & (1 << BITS_OFFSET))  //  0100 0000 0000 0000 0000
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

static int __init key_init(void)
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
  pnode = of_find_node_by_path("/putekey");
  if (!pnode)
  {
    pr_err("of_find_node_by_path error\n");
    return -ENODEV;
  }

  ret = of_property_read_string(pnode, "compatible", &dts_status);
  if (ret)
  {
    pr_err("of_property_read_string error\n");
    return ret;
  }

  pr_info("status is %s\n", dts_status);

  ret = of_property_read_u32_array(pnode, "reg", reg_arr, 8);
  if (ret)
  {
    pr_err("of_property_read_u32_array error\n");
    return ret;
  }

  for (i = 0; i < 4; i++)
  {
    pr_info("0x%x\n", reg_arr[i * 2]);  // 0 2 4 6
  }

  io_mux = of_iomap(pnode, 0);
  io_pad = of_iomap(pnode, 1);
  io_data = of_iomap(pnode, 2);
  io_dir = of_iomap(pnode, 3);
  if (NULL == io_mux || NULL == io_pad || NULL == io_data || NULL == io_dir)
  {
    ret = -ENOMEM;
    goto misc_del;
  }

  // 设置复用寄存器 -> GPIO
  v = readl(io_mux);
  v = 0x05;
  writel(v, io_mux);

  // 设置电气特性寄存器 -> 默认设置
  v = readl(io_pad);
  v = 0x10b0;
  writel(v, io_pad);

  // 设置GPIO1_3  1  key_off
  v = readl(io_data);
  v = v | (1 << BITS_OFFSET);
  writel(v, io_data);
  mykey_state = MYkey_OFF;

  // 设置GPIO1_3  input
  v = readl(io_dir);
  v &= ~(1 << BITS_OFFSET);
  writel(v, io_dir);

  ret = device_create_file(misc_dev.this_device, &attr);
  if (ret < 0)
  {
    pr_err("device_create_file error\n");
    // return ret;
    goto addr_unmap;
  }
  pr_info("key drv ok\n");

  return 0;
addr_unmap:
  iounmap(io_mux);
  iounmap(io_pad);
  iounmap(io_data);
  iounmap(io_dir);
misc_del:
  misc_deregister(&misc_dev);
  return ret;
}
static void __exit key_exit(void)
{
  device_remove_file(misc_dev.this_device, &attr);
  of_node_put(pnode);
  iounmap(io_mux);
  iounmap(io_pad);
  iounmap(io_data);
  iounmap(io_dir);
  misc_deregister(&misc_dev);

  return;
}

module_exit(key_exit);
module_init(key_init);
MODULE_LICENSE("GPL");
