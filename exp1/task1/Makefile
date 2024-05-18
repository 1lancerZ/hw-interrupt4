ifneq ($(KERNELRELEASE),)
	obj-m := tasklet_interrupt.o
else
	KERNELDIR ?= /usr/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
.PHONY:clean
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko

install:
	insmod tasklet_interrupt.ko irq=31 devname="tasklet_dev"

uninstall:
	rmmod tasklet_interrupt.ko
