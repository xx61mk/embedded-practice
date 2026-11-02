CC:=arm-linux-gnueabihf-gcc

$(MODULE_APP):$(MODULE_APP).c
	$(CC) $^ -o $@
	cp $(MODULE_APP) ~/nfs/rootfs

.PHONY:
clean:
	rm $(MODULE_APP)
distclean:
	rm $(MODULE_APP)
	rm ~/nfs/rootfs $(MODULE_APP)