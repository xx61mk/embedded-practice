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
 6. 加入阻塞功能
 7. input subsys

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
#include "linux/jiffies.h"
#include "linux/kdev_t.h"
#include "linux/kernel.h"
#include "linux/of_irq.h"
#include "linux/printk.h"
#include "linux/string.h"
#include "linux/timer.h"
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
#include <linux/input.h>
static struct input_dev *inp_dev;
static struct device_node *pnode;
static int key_gpio;
static int key_irq;
struct timer_list tm_key;

void key_tm_handle(unsigned long data)
{
  if (0 == gpio_get_value(key_gpio))
  {  // 低电平  构建  event 结构体
    input_event(inp_dev, EV_KEY, KEY_0, 1);
  }
  else
  {
    //高电平
    input_event(inp_dev, EV_KEY, KEY_0, 0);
  }
  //上报  输入事件
  input_sync(inp_dev);
  return;
}

irqreturn_t key_handler(int no, void *data)
{
  tm_key.expires = jiffies + msecs_to_jiffies(10);
  mod_timer(&tm_key, tm_key.expires);
  return IRQ_HANDLED;
}

static int __init mykey_init(void)
{
  int ret = 0;
  // 1.申请input设备的 结构体
  inp_dev = input_allocate_device();
  if (NULL == inp_dev)
  {
    pr_err("input_allocate_device");
    return -ENOMEM;
  }
  // 2. 设置输入设备，是一个按键
  inp_dev->name = "putekey";
  //按键类的设备
  set_bit(EV_KEY, inp_dev->evbit);
  //支持按住不松手
  set_bit(EV_REP, inp_dev->evbit);
  //分配键位
  set_bit(KEY_0, inp_dev->keybit);

  input_set_capability(inp_dev, EV_KEY, KEY_0);
  // 3. 注册输入设备到内核
  ret = input_register_device(inp_dev);
  if (ret)
  {
    pr_err("input_register_device error\n");
    goto del_input;
  }

  pnode = of_find_node_by_path("/putekey");
  if (!pnode)
  {
    pr_err("of_find_node_by_path error\n");
    goto unreg_input;
  }

  // key_gpio = of_get_gpio(pnode, 0);
  // if (key_gpio < 0)
  // {
  //   pr_err("of_get_gpio error ,%d\n",key_gpio);

  //   goto del_pnode;
  // }
  key_gpio = of_get_named_gpio(pnode, "gpio-key", 0);
  if (key_gpio < 0)
  {
    pr_err("of_get_gpio error ,%d\n", key_gpio);

    goto del_pnode;
  }
  ret = gpio_request(key_gpio, "pykey");
  if (ret)
  {
    pr_err("gpio_request error\n");
    goto del_pnode;
  }


  gpio_direction_input(key_gpio);
  
  key_irq = of_irq_get(pnode, 0);
  if (key_irq < 0)
  {
    pr_err("of_irq_get error\n");
    ret = key_irq;

    goto del_gpio;
  }

  ret = request_irq(key_irq, key_handler, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING , "key_irq", NULL);
  if (ret < 0)
  {
    pr_err("of_irq_get error\n");
    goto del_gpio;
  }

  init_timer(&tm_key);
  tm_key.function = key_tm_handle;
  return 0;

del_gpio:
  gpio_free(key_gpio);
del_pnode:
  of_node_put(pnode);
unreg_input:

  input_unregister_device(inp_dev);
del_input:
  input_free_device(inp_dev);
  return ret;
}
static void __exit mykeyexit(void)
{
  del_timer(&tm_key);
  free_irq(key_irq, NULL);
  gpio_free(key_gpio);
  of_node_put(pnode);
  input_unregister_device(inp_dev);
  input_free_device(inp_dev);
}
module_exit(mykeyexit);
module_init(mykey_init);
MODULE_LICENSE("GPL");
