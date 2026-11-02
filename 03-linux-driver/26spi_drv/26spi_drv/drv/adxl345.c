/**
 * @file led_drv.c
 * @author yas (rage_yas@hotmail.com)
 * @brief
spi demo
 adxl345 三轴 加速度

CS连接imx6ull P4的17
 SCL              18
 SDO              19
 SDA              20
 GND              47
 VCC              48  3.3V

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
#include <linux/spi/spi.h>

#define DEV_NAME "adxl345"  //  dev/lm75
static struct spi_device *pspi;
static int myprobe(struct spi_device *spi);
static int myremove(struct spi_device *spi);
int myopen(struct inode *, struct file *);
int myrelease(struct inode *, struct file *);
ssize_t myread(struct file *, char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = myopen,
    .release = myrelease,
    .read = myread,
};
static struct miscdevice misc_dev = {
    .name = DEV_NAME,
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &fops,
};
static struct spi_device_id id_table[] = {{
                                              .name = "pt_adxl345",
                                          },
                                          {}};

static struct of_device_id of_table[] = {{
                                             .compatible = "pt,adxl345",
                                         },
                                         {}};

int myopen(struct inode *n, struct file *f)
{
  // adxl 345 初始化
  // 写 `0x31 (DATA_FORMAT)` = `0x08` → 全分辨率 ±2g。
  // 写 `0x2C (BW_RATE)` = `0x0A` → 100 Hz。
  // 写 `0x2D (POWER_CTL)` = `0x08` → 进入测量模式。

  char cmd[2] = {0x31, 0x08};
  char rxbuf[2] = {0};
  spi_write_then_read(pspi, cmd, 2, rxbuf, 0);

  cmd[0] = 0x2c;
  cmd[1] = 0x0a;
  spi_write_then_read(pspi, cmd, 2, rxbuf, 0);
  cmd[0] = 0x2d;
  cmd[1] = 0x08;
  spi_write_then_read(pspi, cmd, 2, rxbuf, 0);
  return 0;
}
int myrelease(struct inode *n, struct file *f)
{
  pr_info("close call\n");
  return 0;
}
ssize_t myread(struct file *f, char __user *data, size_t size, loff_t *off)
{
  int ret = 0;
  char rxbuf[6] = {0};
  // 0x40  多字节操作
  // 0x80 读数据
  char regadd = 0x32 | 0x40 | 0x80;
  spi_write_then_read(pspi, &regadd, 1, rxbuf, 6);

  ret = copy_to_user(data, rxbuf, 6);
  if (ret < 0)
  {
    pr_err("myread copy_to_user error\n");
    return ret;
  }
  return 0;
}

static int myprobe(struct spi_device *spi)
{
  int ret = 0;
  pspi = spi;
  ret = misc_register(&misc_dev);
  if (ret < 0)
  {
    pr_err("myprobe misc_registererror\n");
    return ret;
  }
  pr_err("myprobe ok\n");
  return 0;
}
static int myremove(struct spi_device *spi)
{
  misc_deregister(&misc_dev);
  return 0;
}

static struct spi_driver myspi_drv = {.probe = myprobe,
                                      .remove = myremove,
                                      .id_table = id_table,
                                      .driver =
                                          {
                                              .name = "pt_adxl345",
                                              .owner = THIS_MODULE,
                                              .of_match_table = of_table,
                                          }

};

module_spi_driver(myspi_drv);
MODULE_LICENSE("GPL");
