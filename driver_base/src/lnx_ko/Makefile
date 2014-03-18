ifneq ($(KERNELRELEASE),)
obj-m := biokernbase.o
biokernbase-objs := main.o event.o usbreader.o memory.o misc.o


ARCH=arm
CROSS_COMPILE = arm-linux-gnueabihf-
CC := arm-linux-gnueabihf-gcc
LD := arm-linux-gnueabihf-ld
#ifeq ($(DKMS),y)
#EXTRA_CFLAGS += -I$(src)/inc -I$(src)/inc/hw -I$(src)/inc/linux
#KO_PATH = .
#SYMBOL_PATH = /lib/modules/$(KVERSION)

EXTRA_CFLAGS += -I$(src)/../../../inc -I$(src)/../../../inc/hw -I$(src)/../../../inc/linux
else
KO_PATH ?= ../../../bin
SYMBOL_PATH ?= ../../../lib
#endif

#UDEV_PATH=/etc/udev/rules.d/
KERNELDIR ?= /mnt/adam3660/rt-linux/linux-3.2.0-psp04.06.00.11-rt
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	-cp biokernbase.ko $(KO_PATH) 
	-cp Module.symvers $(SYMBOL_PATH)
	#-cp 71-bionic-daq.rules $(UDEV_PATH)
endif
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf *.o *~ .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers
	rm $(SYMBOL_PATH)/Module.symvers
	#rm $(UDEV_PATH)/71-bionic-daq.rules

.PHONY: all clean
