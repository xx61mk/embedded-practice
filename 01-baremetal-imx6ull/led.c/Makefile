target = led
objs = start.o main.o led.o

$(target).bin : $(objs)
	arm-linux-gnueabihf-ld -Timx6ull.lds $(objs) -o $(target).elf
	arm-linux-gnueabihf-objcopy -O binary -S -g $(target).elf $(target).bin
	arm-linux-gnueabihf-objdump -D $(target).elf > $(target).dis

%.o : %.S
	arm-linux-gnueabihf-gcc -g -c $^ -o $@	
%.o : %.c
	arm-linux-gnueabihf-gcc -g -c $^ -o $@	

clean:
	rm $(objs) $(target).elf $(target).bin $(target).dis  