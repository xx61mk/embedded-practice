/**
 * @file led_drv.c
 * @author yas (rage_yas@hotmail.com)
 * @brief
 iic demo
 lm75 温度传感器

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
#include <linux/platform_device.h>
#include <linux/i2c.h>

#define DEV_NAME "lm75"  //  dev/lm75

static int myprobe(struct i2c_client *, const struct i2c_device_id *);
static int myremove(struct i2c_client *);
static int myopen(struct inode *, struct file *);
static int myrelease(struct inode *, struct file *);
static ssize_t myread (struct file *, char __user *, size_t, loff_t *);

static struct i2c_client *pcli;
static struct file_operations fops = {
.open=myopen,
.release= myrelease,
.read = myread,
};
static struct miscdevice misc_dev = {
    .name = DEV_NAME,
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &fops,

};
static struct i2c_device_id id_table[] = {{.name = "ptlm75"}, {}};

static struct of_device_id of_table[] = {{
                                             .compatible = "pt,lm75",
                                         },
                                         {}};
static struct i2c_driver lm75_drv = {.probe = myprobe,
                                     .remove = myremove,
                                     .id_table = id_table,
                                     .driver = {.name = "ptlm75", .owner = THIS_MODULE, .of_match_table = of_table}

};

static int myopen(struct inode *node, struct file *f)
{
  //如果有初始化的，放在这个位置
  pr_info("myopen call\n");
  return 0;
}
static int myrelease(struct inode *node, struct file *f)
{
  pr_info("myopen close\n");
  return 0;
}
static ssize_t myread (struct file *f, char __user *data, size_t size, loff_t * off)
{
  int ret = 0 ;
  char reg_addr=0x00;
  char buf[2]={0};
  i2c_master_send(pcli, &reg_addr, 1);
  i2c_master_recv(pcli,buf, 2);

  ret = copy_to_user(data, buf, 2);
  if(ret)
  {
    pr_err("myread");
    return ret;
  }
  return sizeof(buf);
}

static int myprobe(struct i2c_client *cli, const struct i2c_device_id *id)
{
  int ret = 0;
  pcli = cli;
  ret = misc_register(&misc_dev);
  if (ret < 0)
  {
    pr_err("myprobe misc_register\n");
    return ret;
  }
  pr_info("lm75 probe ok\n");
  return 0;
}
static int myremove(struct i2c_client *pcli)
{
  misc_deregister(&misc_dev);
  return 0;
}

module_i2c_driver(lm75_drv);

MODULE_LICENSE("GPL");
