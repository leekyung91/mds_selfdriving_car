.SUFFIXES : .c .o 

CC = gcc
AR = ar

INC = -Ilib 
LIBS = -lmyfunc
CFLAGS = -g $(INC)

OBJS = hello.o
SRCS = hello.c

TARGET = hello

all : $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -Llib $(LIBS)

dep :
	gccmakedep $(INC) $(SRCS)

clean :
	rm -rf $(OBJS) $(TARGET)
