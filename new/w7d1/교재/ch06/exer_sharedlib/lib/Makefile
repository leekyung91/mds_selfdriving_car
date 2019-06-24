.SUFFIXES : .c .o 

CC = gcc
AR = ar

INC = 
CFLAGS = -g $(INC) -fPIC

OBJS = myfunc.o
SRCS = myfunc.c

TARGET = libmyfunc.so

all : $(TARGET)
libmyfunc.so : $(OBJS)
	$(CC) -shared -W1,-soname,$@.1 -o $@.1.0.1 $(OBJS)
	ln -s $(TARGET).1.0.1 $(TARGET)

libmyfunc.a : $(OBJS)
	$(AR) rcv $@ $(OBJS)
	ranlib $@
dep :
	gccmakedep $(INC) $(SRCS)

clean :
	rm -rf $(OBJS) libmyfunc.so libmyfunc.so.1.0.1 libmyfunc.a core
