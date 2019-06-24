baz:foo.o      bar.o
	gcc -o baz foo.o bar.o

foo.o:foo.c
	gcc -c foo.c

bar.o:     bar.c
	gcc -c bar.c


clean:
	rm -f baz foo.o bar.o
