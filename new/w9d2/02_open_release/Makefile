obj-m   := sk.o

#KDIR    := /work/REBIS_BSP/linux-2.6.17.13-rebis
KDIR	:= /root/work/embedded/linux-3.12.14

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:	
	make -C $(KDIR) SUBDIRS=$(PWD) clean
