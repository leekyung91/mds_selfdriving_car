#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define FIFO_FILE "/tmp/fifo"
#define BUFF_SIZE 1024

int main( void)
{

	int fd;
	char buff[BUFF_SIZE];

	if ( access(FIFO_FILE,0) != 0 )
	{
		if ( -1 == mkfifo( FIFO_FILE, 0666))
		{
			perror( "mkfifo() error");exit( 1);
		}

		system("ls -l /tmp/fifo");

	}

	if ( -1 == ( fd = open( FIFO_FILE, O_RDONLY)))
	{
		perror( "open() error");
		exit( 1);
	}

	sleep(1);
		
	memset( buff, 0, BUFF_SIZE);
	read( fd, buff, BUFF_SIZE);

	printf("received string is :  %s\n", buff);

	close( fd);
}
