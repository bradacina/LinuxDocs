ifndef KERNEL_SRC 
  KERNEL_SRC=/usr/src/linux-headers-`uname -r` 
endif

obj-m := sysfsDemo.o
CFLAGS_sysfsDemo.o := -DDEBUG

.PHONY: clean

default: 
	$(MAKE) -C $(KERNEL_SRC) M=`pwd` modules

clean:
	rm sysfsDemo.ko sysfsDemo.mod.* sysfsDemo.o modules.order Module.symvers

