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
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers mpc5200/*.ko mpc5200/*.mod.c mpc5200/*.o

.PHONY: modules clean

else
	ccflags-y := -std=gnu99
	obj-m := flink.o
ifeq ($(CONFIG_PCI),y) 
	obj-m += flink_pci.o 
endif

ifeq ($(CONFIG_SPI),y) 
	obj-m += flink_spi.o 
endif

ifeq ($(CONFIG_PPC_MPC5200_SIMPLE),y)
	obj-m += mpc5200/flink_lpb.o #mpc5200/flink_spi.o
endif
	flink-objs := flink_core.o
endif
