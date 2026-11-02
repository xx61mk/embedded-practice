start.bin : start.S
	arm-linux-gnueabihf-gcc -g -c start.S -o start.o
	arm-linux-gnueabihf-ld -Ttext 0x87800000 start.o -o start.elf
	arm-linux-gnueabihf-objcopy -O binary -S -g start.elf start.bin
	arm-linux-gnueabihf-objdump -D start.elf > start.dis

clean:
	rm start.o start.elf start.bin start.dis