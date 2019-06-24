#include <signal.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

#define	SIG_LEDONN	SIGUSR1
#define	SIG_LEDOFF	SIGUSR2

void sig_usr(int signo);

int main()
{
    int i = 0	;
    int pid	;

    if (signal(SIG_LEDONN, &sig_usr) == SIG_ERR)
    {
        printf ("signal(SIGUSR2) error");
        return -1;
    }    
    if (signal(SIG_LEDOFF, &sig_usr) == SIG_ERR)
    {
        printf ("signal(SIGUSR2) error");
        return -1;
    }    

    
    pid = fork();
    
    if(pid < 0) {
        exit(-1);
    } else if (pid== 0) {
	while(1) sleep(1);
	
    } else {          /* parent process */

	while(1)
	{
		kill(pid, SIG_LEDONN );
	        sleep(1);
	        printf("mom: SIG_USR1\n");
		kill(pid, SIG_LEDOFF );
	        printf("mom: SIG_USR1\n");		
	        sleep(1);
	    }
    }
}

void sig_usr(int signo)
{
	switch(signo) {
	
	case SIG_LEDONN:
	        printf("son: turn onn LED\n");
		break;
		
	case SIG_LEDOFF:
	        printf("son: turn off LED\n");
		break;
		
	}

}
