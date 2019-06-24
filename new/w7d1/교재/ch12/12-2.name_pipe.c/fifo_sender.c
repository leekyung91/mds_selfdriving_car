#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo"

int main(void)
{
	int fd;
	char *str = "mdsacademy.co.kr";

	if ( access(FIFO_FILE,0) != 0 )
	{
		if ( -1 == mkfifo( FIFO_FILE, 0666))
		{
			perror( "mkfifo() error");exit( 1);
		}

		system("ls -l /tmp/fifo");

	}
	
	if ( -1 == ( fd = open( FIFO_FILE, O_WRONLY)))
	{
		perror( "open() error");
		exit( 1);
	}
	write( fd, str, strlen(str));
	close( fd);
}
