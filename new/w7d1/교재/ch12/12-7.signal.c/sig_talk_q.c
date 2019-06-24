/***************************************
 * Filename: sig_talk.c
 * Title: Signal
 * Desc: Example of how 2 processes can talk
 * to each other using kill() and signal()
 * Revision History & Comments
 *
 ****************************************/
#include <stdio.h>
#include <signal.h>

void sighup(); /* routines child will call upon sigtrap */
void sigint();
void sigquit();

main()
{ int pid;

  /* get child process */
  
   if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    
   if (pid == 0)
     { /* child */
       signal(SIGHUP,sighup); /* set function calls */
       signal(SIGINT,sigint);
       signal(SIGQUIT, sigquit);
       for(;;); /* loop for ever */
     }
  else /* parent */
     {  
		/* TODO: Child 프로세스에게 SIGHUP, SIGINT, SIGQUIT 보내기 */
     }
}

void sighup()

{  signal(SIGHUP,sighup); /* reset signal */
   printf("CHILD: I have received a SIGHUP\n");
}

void sigint()

{  signal(SIGINT,sigint); /* reset signal */
   printf("CHILD: I have received a SIGINT\n");
}

void sigquit()

{ printf("My DADDY has Killed me!!!\n");
  exit(0);
}

