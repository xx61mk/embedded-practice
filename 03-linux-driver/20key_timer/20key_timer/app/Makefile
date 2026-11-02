SRC := app.c 
DST := app 
CC := arm-linux-gnueabihf-gcc

all:$(SRC)
	$(CC) $(SRC) -o $(DST)  
	cp app ~/nfs/rootfs


clean:
	rm $(DST)  ~/nfs/rootfs/$(DST) 