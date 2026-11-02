MODULENAME:=bh1750
MODULE_APP:=$(MODULENAME)_app
MODULE_DRV:=$(MODULENAME)_drv

export MODULE_APP MODULE_DRV

all:
	make -C $(MODULE_APP)
	make -C $(MODULE_DRV)

.PHONY:
clean:
	make -C $(MODULE_APP) clean	
	make -C $(MODULE_DRV) clean
distclean:
	make -C $(MODULE_APP) distclean	
	make -C $(MODULE_DRV) distclean