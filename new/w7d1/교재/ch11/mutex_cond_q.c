#include <stdio.h>
#include <pthread.h>

pthread_t  thread;

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   cond  = PTHREAD_COND_INITIALIZER;

int count = 0;

void* Thread ( void* arg ) {
    
    pthread_mutex_lock ( &mutex ); 
    
    while ( count < 5 ) {
        printf( "count = %d: wait...\n", count );

	/*TODO: wait for condition variable signal  */
;
    }
    	printf( "count = %d: done...\n", count );
    
    pthread_mutex_unlock ( &mutex );

    pthread_exit(0);
}

void main ( void ) {
    int i;
    pthread_create( &thread, NULL, (void*(*)(void*))Thread, NULL );
    
    for ( i = 0; i < 10; i++ ) {
        sleep(1);
        pthread_mutex_lock  ( &mutex );

        count++;

        /* TODO: signal condition variable 'cond' */ 
;

        printf( "condition signal %d\n", i );

        pthread_mutex_unlock( &mutex );
    }

    pthread_exit(0);
}
/****************************************
 Run:
*****************************************
count = 0: wait...
condition signal 0
count = 1: wait...
condition signal 1
count = 2: wait...
condition signal 2
count = 3: wait...
condition signal 3
count = 4: wait...
condition signal 4
count = 5: done...
condition signal 5
condition signal 6
condition signal 7
condition signal 8
condition signal 9
*****************************************/
