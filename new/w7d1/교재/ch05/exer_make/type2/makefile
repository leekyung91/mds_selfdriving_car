OBJECTS= main.o func1.o func2.o
TARGET= makeprog

makeprog:	$(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS)

main.o: main.c
	gcc -c main.c

func1.o: func1.c
	gcc -c func1.c

func2.o: func2.c
	gcc -c func2.c

clean:
	rm -f $(TARGET) $(OBJECTS)
