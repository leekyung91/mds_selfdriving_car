#include <stdio.h>
#include <pthread.h>

int thread_args[3] = { 0, 1, 2 }; 

void* Thread( void *arg )
{
    int i;
    for ( i=0; i<3; i++ )
        printf( "thread %d: %dth iteration\n", *(int*)arg, i );

	//while(1); // TODO: hold here and try $ps -L

	pthread_exit(0);
}

int main( void )
{
    int i, clock_get;
    pthread_t threads[3]; 
    
    for ( i=0; i<3; i++ ) 
        // TODO: create a thread having Thread function 
        pthread_create( &threads[i],                
                        NULL,                      
                        ( void* (*)(void*) )Thread,
                        &thread_args[i] );
    
    while(1)sleep(1);
    
    //TODO: exit
    pthread_exit(0);
}

/****************************************
 Run:
*****************************************
thread 0: 0th iteration
thread 0: 1th iteration
thread 0: 2th iteration
thread 1: 0th iteration
thread 1: 1th iteration
thread 1: 2th iteration
thread 2: 0th iteration
thread 2: 1th iteration
thread 2: 2th iteration
****************************************/
