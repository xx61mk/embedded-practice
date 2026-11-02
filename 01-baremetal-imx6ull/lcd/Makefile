target = lcd

cross_compiler = arm-linux-gnueabihf-

cc = $(cross_compiler)gcc
ld = $(cross_compiler)ld
objcopy = $(cross_compiler)objcopy
objdump = $(cross_compiler)objdump

incdirs = bsp imx6ull stdio/include graphics
srcdirs = bsp project stdio/lib graphics

libpath = -lgcc -L/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4

include = $(patsubst %, -I%, $(incdirs))

cfiles = $(foreach dir, $(srcdirs), $(wildcard $(dir)/*.c))
sfiles = $(foreach dir, $(srcdirs), $(wildcard $(dir)/*.S))

cfilenodir = $(notdir $(cfiles))
sfilenodir = $(notdir $(sfiles))

cobjs = $(patsubst %, obj/%, $(cfilenodir:.c=.o))
sobjs = $(patsubst %, obj/%, $(sfilenodir:.S=.o))

objs = $(cobjs) $(sobjs)

VPATH = $(srcdirs)

$(target).bin : $(objs)
	$(ld) -Timx6ull.lds -o$(target).elf $^ $(libpath)
	$(objcopy) -O binary -S -g $(target).elf $@
	$(objdump) -D $(target).elf > $(target).dis

$(sobjs) : obj/%.o : %.S
	@mkdir -p obj
	$(cc) -Wall -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c $(include) -o $@ $<

$(cobjs) : obj/%.o : %.c
	@mkdir -p obj		
	$(cc) -Wall -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c $(include) -o $@ $<

.PHONY : clean
clean:
	rm -rf $(objs) $(target).elf $(target).bin $(target).dis
load:
	./../imxdownload $(target).bin /dev/sdb
