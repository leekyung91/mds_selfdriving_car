/***************************************
 * Filename: orphan.c
 * Title: Process Management
 * Desc: Orphan Process
 * ***************************************/
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	printf("I'm the original process : pid = %d, ppid = %d\n", 
						getpid(), getppid());

	pid =  fork();
		
	if ( pid != 0 )  {	/* parent process */
		printf("I'm parent process : pid = %d, ppid = %d\n", 
						getpid(), getppid());
		printf("my child process : pid = %d\n", pid);
	}
	else {				/* child process */
		sleep(5);
		printf("I'm child process : pid = %d, ppid = %d\n", 
						getpid(), getppid());
	}
		/* parent & child  process */
		printf("pid %d terminates.. \n", getpid());  
}
