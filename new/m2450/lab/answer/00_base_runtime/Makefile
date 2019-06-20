####################################################	
#
# NAME: Makefile - mC100 test uC/OS-II Makefile
# Brief history
# -------------
# Created by : Seonghye, Son	2010/12/10
#
####################################################	 

####################################################
####     Tools and Source Path Definition       ####
####################################################
TOPDIR 		= $(PWD)
VERSION 	= 4.5.2
#TARGET		= arm-none-eabi
TARGET		= arm-none-eabi
TOOLDIR		= /opt/CodeSourcery/Sourcery_G++_Lite
LIBCDIR 	= $(TOOLDIR)/$(TARGET)/lib
LIBGCCDIR 	= $(TOOLDIR)/lib/gcc/$(TARGET)/$(VERSION)
OUTDIR      =${TOPDIR}
OBJDIR      =${TOPDIR}
BSPDIR      = bsp
INCDIR      = include

AS			= $(TOOLDIR)/bin/$(TARGET)-as
CC			= $(TOOLDIR)/bin/$(TARGET)-gcc
LD 			= $(TOOLDIR)/bin/$(TARGET)-ld
OBJCOPY	 	= $(TOOLDIR)/bin/$(TARGET)-objcopy
OBJDUMP 	= $(TOOLDIR)/bin/$(TARGET)-objdump	  

SRCS	= libc.c  Main.c Uart.c timer.c exception.c Lcd_480x272.c touch.c syscall.c
ASRCS	= s3c2450_startup.S libs.S

BSPSRC      = $(BSPDIR)/clkcon.c\
							$(BSPDIR)/syscall.c\
              $(BSPDIR)/libc.c\
              $(BSPDIR)/intc.c\
			        $(BSPDIR)/timer.c\
              $(BSPDIR)/uartdrv.c\
              $(BSPDIR)/keypad.c\
              $(BSPDIR)/lcddrv.c\
              $(BSPDIR)/touch.c\
							$(BSPDIR)/Exception.c\
							$(BSPDIR)/malloc_lock.c\
							$(BSPDIR)/PC.c\
              $(BSPDIR)/drv/commrtos.c\
              $(BSPDIR)/drv/key.c
              
BSPASRC 	= $(BSPDIR)/common.S\
              $(BSPDIR)/cp15a.S\
              $(BSPDIR)/libs.S\
              $(BSPDIR)/startup.S\
              $(BSPDIR)/os_cpu_a.S\
              $(BSPDIR)/random.S

APPSRC 		= 
			
OBJS        =  $(SRCS:.c=.o) $(ASRCS:.S=.o)
OBJS    	+= $(APPSRC:.c=.o)

####################################################
####             Option Definition              ####
####################################################
INCLUDE	= -nostdinc  -I. -I$(INCDIR) -I$(LIBGCCDIR)/include -I$(TOOLDIR)/$(TARGET)/include 

CFLAGS	= 	$(INCLUDE) -g -Wall -Wstrict-prototypes -Wno-trigraphs -O0
CFLAGS	+= 	-fno-strict-aliasing -fno-common -Wno-parentheses -Wno-unused-parameter -Wno-implicit-function-declaration
CFLAGS	+= 	-Wno-unused-function -Wno-old-style-declaration -fomit-frame-pointer
CFLAGS	+= 	-mcpu=arm9e -fno-builtin
#CFLAGS	+=	-mfloat-abi=softfp -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=6
#CFLAGS	+=	-mvectorize-with-neon-quad
#CFLAGS  +=  -D__XDEF_REPORT_STAT

LDFLAGS	= 	--cref -Bstatic -nostartfiles -p -EL -T S3C2450-RAM.ld -Map \
			$(OUTDIR)/$(MC100TEST).map 

OCFLAGS = -O binary -R .note -R .comment -S

MC100TEST = MDS2450

all: $(OUTDIR)/$(MC100TEST).bin

$(OUTDIR)/$(MC100TEST).bin : $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/$(MC100TEST) $(OBJS) -L$(LIBCDIR) -L$(LIBGCCDIR) -lc -lgcc
	$(OBJCOPY) $(OCFLAGS) $(OUTDIR)/$(MC100TEST) $@
	cp $(MC100TEST).bin /tftpboot
	
%.o : %.S
	$(CC) -c $(CFLAGS) -o $@ $<
	
%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean:
	rm -f $(OUTDIR)/*.o
	rm -f $(OUTDIR)/$(MC100TEST)*

dep:
	$(CC) -M $(INCLUDE) $(SRCS) $(APPSRC) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
