ifndef KERNEL_SRC 
  KERNEL_SRC=/usr/src/linux-headers-`uname -r` 
endif

obj-m := debugfsDemo.o
CFLAGS_debugfsDemo.o := -DDEBUG

.PHONY: clean

default: 
	$(MAKE) -C $(KERNEL_SRC) M=`pwd` modules

clean:
	rm debugfsDemo.ko debugfsDemo.mod.* debugfsDemo.o modules.order Module.symvers

