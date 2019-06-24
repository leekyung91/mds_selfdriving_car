#include <signal.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

#include <pthread.h>
pthread_t  thread;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

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
	while(1) sleep(5);
	
    } else {          /* parent process */

	while(1)
	{
		pthread_mutex_lock 	( &mutex ); 
		kill(pid, SIG_LEDONN );
	        printf("mom: my son!  turn ONN LED !\n");
		pthread_mutex_unlock 	( &mutex );

	        sleep(2);

		pthread_mutex_lock 	( &mutex ); 
		kill(pid, SIG_LEDOFF );
	        printf("mom: my son!  turn OFF LED !\n");
		pthread_mutex_unlock 	( &mutex );
			        	
	        sleep(2);
	    }
    }
}

void sig_usr(int signo)
{

	pthread_mutex_lock 	( &mutex ); 	
	switch(signo) {
	case SIG_LEDONN:
	        printf("son: yes! I turned ONN LED !\n");
		break;
		
	case SIG_LEDOFF:
	        printf("son: yes! I turned OFF LED !\n");
		break;
	}
	        printf("============================\n");
	pthread_mutex_unlock 	( &mutex );

}
