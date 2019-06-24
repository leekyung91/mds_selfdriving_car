/***************************************
 * Filename: sig_talk.c
 * Title: Signal
 * Desc: Example of how 2 processes can talk
 * to each other using kill() and signal()
 * Revision History & Comments
 *
 ****************************************/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void sighup(); /* routines child will call upon sigtrap */
void sigint();
void sigquit();

int main(void)
{ 
	int pid;

  /* get child process */
  
	pid = fork();
  
   if (pid<0) {
		perror("fork");
		exit(1);
   }
   
	printf("pid is : %d \n", pid);

 
   if(pid == 0)
     { /* child */
	 
		signal(SIGHUP,sighup); 
		signal(SIGINT,sigint);
		signal(SIGQUIT,sigquit);
   
		while(1){
			printf(".");
			fflush(stdout);
			sleep(1);
		}; /* loop for ever */
	   
     }
  else /* parent */
     {  /* pid hold id of child */

		sleep(1);

		printf("\nPARENT: sending SIGHUP\n\n");
		kill(pid,SIGHUP);
		sleep(3); /* pause for 3 secs */
		printf("\nPARENT: sending SIGINT\n\n");
		kill(pid,SIGINT);
		sleep(3); /* pause for 3 secs */
		printf("\nPARENT: sending SIGQUIT\n\n");
		kill(pid,SIGQUIT);
		sleep(3);
     }
}

void sighup()
{  
	signal(SIGHUP,sighup); /* reset signal */
	printf("CHILD: I have received a SIGHUP\n");
}

void sigint()
{  
	signal(SIGINT,sigint); /* reset signal */
	printf("CHILD: I have received a SIGINT\n");
}

void sigquit()
{ 
	printf("My DADDY has Killed me!!!\n");
	exit(0);
}

