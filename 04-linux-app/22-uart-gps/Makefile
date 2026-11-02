OBJS+=main.c
OBJS+=uart.c
OBJS+=gps.c

OBJ:=uart_gps

CC:=arm-linux-gnueabihf-gcc

$(OBJ):$(OBJS)
	$(CC) $^ -o $@
	cp $(OBJ) ~/nfs/rootfs 
.PHONY:
clean:
	rm $(OBJ)
distclean:
	rm ~/nfs/rootfs/$(OBJ)