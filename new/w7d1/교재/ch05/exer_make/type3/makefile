OBJECTS= main.o func1.o func2.o
TARGET= makeprog
CC= gcc
CFLAGS= -c
makeprog:	$(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

func1.o: func1.c
	$(CC) $(CFLAGS) func1.c

func2.o: func2.c
	$(CC) $(CFLAGS) func2.c

clean:
	rm -f $(TARGET) $(OBJECTS)
