
CFLAGS= -lrt
all:
	$(CC) -o c $(CFLAGS) named_semaphore_customer.c 
	$(CC) -o p $(CFLAGS) named_semaphore_producer.c
	echo "run $./c & and $./p & "
clean:
	rm -rf p c

