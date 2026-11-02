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
#include <linux/platform_device.h>

static int myprobe(struct platform_device * pdev);
static int myremove(struct platform_device *pdev);

struct platform_device_id id_table[]={
  {.name="ptled",},{}
};
struct of_device_id of_table[]={
  {
    .compatible = "pute,led0",
  },{}
};

static struct platform_driver  plat_drv={
  .probe = myprobe,
  .remove = myremove,
  .id_table = id_table, // 适用于 非设备树的驱动
  .driver={
    .name = "ptled",
    .owner = THIS_MODULE,
    .of_match_table = of_table,

  },
};

static int myprobe(struct platform_device * pdev)
{
  return 0;
}
static int myremove(struct platform_device *pdev)
{
  return 0;
}

static int __init led_init(void)
{
  int ret = 0 ;
  ret = platform_driver_register(&plat_drv);
  if(ret<0)
  {
    pr_err("platform_driver_register error\n");
    return ret;
  }
  return 0;
}
static void __exit led_exit(void)
{
  platform_driver_unregister(&plat_drv);
  return ;
}
module_exit(led_exit);
module_init(led_init);
MODULE_LICENSE("GPL");
