ifndef KERNEL_SRC 
  KERNEL_SRC=/usr/src/linux-headers-`uname -r` 
endif

obj-m := helloWorld.o
CFLAGS_helloWorld.o := -DDEBUG

.PHONY: clean

default: 
	$(MAKE) -C $(KERNEL_SRC) M=`pwd` modules

clean:
	rm helloWorld.ko helloWorld.mod.* helloWorld.o modules.order Module.symvers

