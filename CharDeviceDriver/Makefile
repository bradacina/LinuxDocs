ifndef KERNEL_SRC 
  KERNEL_SRC=/usr/src/linux-headers-`uname -r` 
endif

obj-m := charDeviceDriver.o
CFLAGS_charDeviceDriver.o := -DDEBUG

.PHONY: clean

default: 
	$(MAKE) -C $(KERNEL_SRC) M=`pwd` modules

clean:
	rm charDeviceDriver.ko charDeviceDriver.mod.* charDeviceDriver.o modules.order Module.symvers

