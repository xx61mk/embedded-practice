OBJ:=camera_app

OBJS+=main.c 
OBJS+=video.c 
OBJS+=framebuff.c 
OBJS+=font_8x16.c 
OBJS+=font_16x16.c

CC:=arm-linux-gnueabihf-gcc 

$(OBJ):$(OBJS)
	$(CC) -mfpu=neon -mcpu=cortex-a7 -O3 -flax-vector-conversions $^ -o $@
	cp $(OBJ) ~/nfs/rootfs 
.PHONY:
clean:
	rm $(OBJ)
distclean:
	rm ~/nfs/rootfs/$(OBJ)