obj-m   := sk.o

KDIR	:= /root/work/embedded/linux-3.12.14

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:	
	make -C $(KDIR) SUBDIRS=$(PWD) clean
