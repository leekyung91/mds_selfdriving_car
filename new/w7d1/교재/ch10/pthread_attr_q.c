#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>

int thread_args[3] = { 0, 1, 2 };

void* Thread( void *arg )
{
    int i;
    nice(10); 
	for ( i=0; i<100000; i++ ){
        printf( "%d", *(int*)arg);
    }

	//fflush(stdout);	// try if you have not displayed 2s all enough !
	
	sleep(100);	//sleep for probe

    pthread_exit(0);  
}

int main( void ) {
    int i;
    pthread_t threads[3];
    
    /*TODO: thread attribute variable here */


    /*TODO: thread schedule parameter here */


    for ( i=0; i<3; i++ ) {
        /*TODO: initialize thread atrribute variable here */ 


        /*TODO: set schedule policy : round-robin here */ 


        /*TODO: set thread priority : 65 */ 



        pthread_create( &threads[i], 
                        &thread_attrs, 
                        ( void* (*)(void*) )Thread, 
                        &thread_args[i] );

    }
    
	while(1)sleep(1);
    pthread_exit(0); 
}

