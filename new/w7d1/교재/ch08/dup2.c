#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2
int main(int argc, char **argv)
{

/*
	see hello world on screen

*/
	int fd1, fd2;

	fd1 = open("test.log", O_RDWR|O_CREAT);
	
	fd2 = dup(fd1);

	//dup2(STDOUT,fd2); // fd2 becomes STDOUT
	dup2(fd1,STDOUT); // STDOUT becomes fd1

//	write(fd2, "\n helloworld \n", 14);

	printf("hello world : %d \n",fd1);

	return 0;
}
