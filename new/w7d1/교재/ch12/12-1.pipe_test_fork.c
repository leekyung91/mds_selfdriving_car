#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define BUFSIZE 30

//--------------------------------------
// Create a pipe, mom write to it, and son read from it.
int main( int argc, char **argv )
{
	static const char mesg[] = "www.mdsacademy.co.kr";
	char buf[BUFSIZE];
	ssize_t rcount, wcount;
	int pipefd[2]; /*pipe descriptor */
	size_t len;

	pid_t son;
	
	/* TODO: create pipe descriptors pair */

	if ( pipe(pipefd) < 0 ) 
	{
		fprintf(stderr, "%s: pipe failed : %s\n", argv[0], strerror(errno));
		exit(1);
	}

	son = fork();

	printf( "Read end = fd %d, write end = fd %d\n",pipefd[0], pipefd[1]);
	len = strlen(mesg);

	if (son != 0)	// mom
	{
		/* put to write end */	
		wcount = write( pipefd[1], mesg, len);
		printf( "mom: Writ <%s> To. pipe \n", mesg );
		wait(NULL);
	}
	
	if (son == 0)	// son
	{
		sleep(1);
		/* get fr. read end */
		rcount = read( pipefd[0], buf, BUFSIZE);

		buf[rcount] = '\0'; /* make null-term string */
		printf( "son: Read <%s> Fr. pipe \n", buf );
	}
	
	close( pipefd[0] );
	close( pipefd[1] );

	return 0;
}
