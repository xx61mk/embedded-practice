# 嵌入式学习代码集合

个人学习过程中积累的嵌入式代码，按平台与方向分类整理。全部为源码，已剔除编译产物（`.o` / `.ko` / `.hex` / 可执行文件）与第三方工具。

## 目录结构

```
embedded-practice/
├── 01-baremetal-imx6ull/   i.MX6ULL 裸机实验（C + 汇编，无操作系统）
├── 02-c51/                 C51 单片机实验（Keil 工程）
├── 03-linux-driver/        Linux 内核驱动与并发编程实验
└── 04-linux-app/           Linux 用户态应用（串口 / 摄像头 / 图形）
```

| 分类 | 内容 | 文件数 |
|---|---|---|
| 01-baremetal-imx6ull | adc、beep、clock、epit、i2c、key、lcd、led、led_asm、uart 共 11 个实验 | 327 |
| 02-c51 | led、timer、ds18b20、digiter（数码管）、k 共 5 个 Keil 工程 | 39 |
| 03-linux-driver | 18 个内核模块实验 + bh1750 光照驱动 + 16-lock 并发控制 | 220 |
| 04-linux-app | 串口 GPS 解析、V4L2 摄像头采集、Framebuffer 五子棋 | 20 |

## 01-baremetal-imx6ull

i.MX6ULL（ARM Cortex-A7）裸机程序，每个子目录是一个独立实验，含 `start.S` 启动文件、链接脚本 `*.lds`、BSP 驱动与 `Makefile`。

- `led_asm/` 纯汇编点灯，其余为 C 语言 + 汇编启动。
- 各实验内的 `imx6ull/MCIMX6Y2.h` 是 NXP 官方寄存器定义头文件（约 3.3MB），每个实验各带一份，方便单独编译。
- 编译：进入实验目录执行 `make`，产物为 `*.bin`，可用 `imxdownload` 烧写到 SD 卡。

## 02-c51

Keil C51 工程，含 `.uvproj` 工程文件，可直接用 Keil 打开编译。

- `ds18b20/` 单总线温度传感器
- `digiter/` 数码管动态扫描
- `timer/` 定时器中断

## 03-linux-driver

按实验序号排列，每个目录是一个独立的内核模块工程，含 `Makefile` 与源码。

- `05led_misc` ~ `11key_pin`：杂项设备、设备树、pinctrl 子系统
- `12key_wq_irq` ~ `15atomic`：工作队列、中断下半部、内核线程、原子操作
- `16-lock/`：自旋锁、互斥锁、信号量、完成量四种同步机制对比
- `20key_timer` ~ `26spi_drv`：定时器、input 子系统、platform 总线、I2C、SPI 子系统
- `bh1750/`：光照传感器完整驱动 + 用户态测试程序

编译需指定内核源码路径：

```bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
```

## 04-linux-app

- `22-uart-gps`：串口读取 GPS 模块，解析 NMEA 报文
- `25-v4l2-camera`：V4L2 采集摄像头图像，Framebuffer 显示（含字库文件）
- `wuziqi-framebuffer`：Framebuffer + 触摸屏实现的五子棋

编译：进入目录执行 `make`。

## 实验笔记

每个分类目录下有 `NOTES.md`，记录每个实验的关键知识点、踩过的坑和熟练度。代码会忘，笔记不会。

## 环境

- 交叉编译工具链：`arm-linux-gnueabihf-gcc`
- 开发板：正点原子 i.MX6ULL（Cortex-A7）
- 内核版本：4.1.15
