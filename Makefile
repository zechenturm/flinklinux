# Makefile for the fLink linux kernel modules
# 2014-01-14 martin@zueger.eu

ifeq ($(KERNELRELEASE),)
ifeq ($(CHROOT),)
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
else
	KERNELDIR ?= /usr/src/linux
	CHROOT_CMD ?= schroot -c $(CHROOT) --
endif
	PWD := $(shell pwd)
modules:
	$(CHROOT_CMD) $(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers

.PHONY: modules clean

else
	ccflags-y := -std=gnu99
	obj-m := flink.o flink_pci.o flink_spi.o
	flink-objs := flink_core.o
endif
