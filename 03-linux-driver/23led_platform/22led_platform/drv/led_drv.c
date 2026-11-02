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

#define DEV_NAME "led0"  //  /dev/led0

static int myprobe(struct platform_device *pdev);
static int myremove(struct platform_device *pdev);
ssize_t mywrite(struct file *, const char __user *, size_t, loff_t *);
int myopen(struct inode *, struct file *);
int myclose(struct inode *, struct file *);

static struct device_node *pnode = NULL;
static int led_gpio = 0;
static struct file_operations fops = {
    .open = myopen,
    .release = myclose,
    .write = mywrite,
};
static struct miscdevice misc_dev = {
    .name = DEV_NAME,
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &fops,
};
struct platform_device_id id_table[] = {{
                                            .name = "ptled",
                                        },
                                        {}};
struct of_device_id of_table[] = {{
                                      .compatible = "pute,led0",
                                  },
                                  {}};

static struct platform_driver plat_drv = {
    .probe = myprobe,
    .remove = myremove,
    .id_table = id_table,  // 适用于 非设备树的驱动
    .driver =
        {
            .name = "ptled",
            .owner = THIS_MODULE,
            .of_match_table = of_table, // 设备树的匹配规则

        },
};

ssize_t mywrite(struct file *f, const char __user *data, size_t size, loff_t * off)
{

  int sw =0;
  int ret = 0 ;
  ret = copy_from_user(&sw, data, sizeof(sw));
  if(ret)
  {
    pr_err("mywrite copy_from_user error\n");
    return ret;
  }
  if(1==sw)
  {
    //led on
    gpio_set_value(led_gpio,0);
  }
  else  if(0 ==sw)
  {
    gpio_set_value(led_gpio,1);
    //led off
  }


  return size;
}
int myopen(struct inode *node, struct file * f)
{
  pr_info("myopen called\n");
  return 0;
}
int myclose(struct inode *node, struct file *f)
{
  pr_info("myclose called\n");
  return 0;
}

//当驱动和设备匹配(compatible)成功，才会调用这个函数
static int myprobe(struct platform_device *pdev)
{
  int ret = 0;
  // 驱动开始的地方在这里
  pr_info("led0  probe caller...\n");

  ret = misc_register(&misc_dev);
  if (ret < 0)
  {
    pr_err("misc_register error\n");
    return ret;
  }
  pnode = pdev->dev.of_node;  // of_find_node_by_path

  led_gpio = of_get_gpio(pnode, 0);
  if (led_gpio < 0)
  {
    pr_err("of_get_gpio error\n");
    ret = led_gpio;
    goto del_misc;
  }

  ret = gpio_request(led_gpio, "myled0");
  if (led_gpio < 0)
  {
    pr_err("gpio_request error\n");
    //return led_gpio;
    goto del_misc;
  }

  gpio_direction_output(led_gpio, 1);

  return 0;
del_misc:
    misc_deregister(&misc_dev);

    return ret;
}
static int myremove(struct platform_device *pdev)
{
  gpio_free(led_gpio);
  misc_deregister(&misc_dev);

  return 0;
}

static int __init led_init(void)
{
  int ret = 0;
  ret = platform_driver_register(&plat_drv);
  if (ret < 0)
  {
    pr_err("platform_driver_register error\n");
    return ret;
  }
  return 0;
}
static void __exit led_exit(void)
{
  platform_driver_unregister(&plat_drv);
  return;
}
module_exit(led_exit);
module_init(led_init);
MODULE_LICENSE("GPL");
