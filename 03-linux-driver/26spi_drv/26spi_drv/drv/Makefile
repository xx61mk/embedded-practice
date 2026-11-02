
DST :=adxl345
KER_DIR := /home/linux/imx6ull/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
CUR_DIR := $(shell pwd)
obj-m += $(DST).o

# -C  进入目录  M  指定目录模块所在的路径
#make modules  只编译模块  

all:
	$(MAKE) -C $(KER_DIR) M=$(CUR_DIR) modules
	cp $(DST).ko ~/nfs/rootfs
clean:
	$(MAKE) -C $(KER_DIR) M=$(CUR_DIR) clean 
	rm ~/nfs/rootfs/$(DST).ko



