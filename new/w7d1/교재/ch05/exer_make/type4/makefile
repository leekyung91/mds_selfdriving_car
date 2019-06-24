.SUFFIXES = .c .o
OBJECTS= main.o func1.o func2.o
SRCS= main.c func1.c func2.c
TARGET= makeprog
CC= gcc
CFLAGS= -g -c

$(TARGET):	$(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

.c.o :
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS) core

main.o: io.h main.c
func1.o: io.h func1.c
func2.o: io.h func2.c
