#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>

/* TODO: define mutex variable and initialize */ 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int val;
int arg1 = 0, arg2 = 1;

void *Thread( void* arg ) 
{
    int i, j;
    
    for( i = 0; ; i++ ) {
        /* TODO: lock   the mutex */
        pthread_mutex_lock( &mutex );

        val = *(int*)arg;
        printf( "thread %d: %dth iteration: val = %d\n", *(int*)arg, i, val);

        /* TODO: unlock the mutex */
        pthread_mutex_unlock( &mutex );

        sleep(1);
    }
	pthread_exit(0);
}

int main( void ) {
    pthread_t  thread1, thread2;
    pthread_attr_t attr;
    
    pthread_attr_init( &attr );
    
    pthread_create( &thread1, &attr, (void*(*)(void*))Thread, &arg1 );
    pthread_create( &thread2, &attr, (void*(*)(void*))Thread, &arg2 );
    
    pthread_exit(0);

}
/****************************************
 Run:
*****************************************
thread 0: 0th iteration: i = 0                                                  
thread 1: 0th iteration: i = 1                                                  
thread 0: 1th iteration: i = 0                                                  
thread 1: 1th iteration: i = 1                                                  
thread 0: 2th iteration: i = 0                                                  
thread 1: 2th iteration: i = 1                                                  
thread 0: 3th iteration: i = 0                                                  
thread 1: 3th iteration: i = 1                                                  
thread 0: 4th iteration: i = 0                                                  
thread 1: 4th iteration: i = 1                                                  
thread 0: 5th iteration: i = 0                                                  
thread 1: 5th iteration: i = 1                                                  
thread 0: 6th iteration: i = 0                                                  
thread 1: 6th iteration: i = 1                                                  
thread 0: 7th iteration: i = 0       
...
*****************************************/
