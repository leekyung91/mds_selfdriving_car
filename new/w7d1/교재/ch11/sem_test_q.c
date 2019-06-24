#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

/*TODO: semaphore variable - sem */
pthread_t   task;

void Thread( void ) {
    int i = 0;
    int val;
  
    while(1) {
        i++;

/* TODO: take semaphore */

        sem_getvalue( &sem, &val ); /* get sem value to val */
        printf( "wakeup %d: value = %d\n", i ,val );
        
        if ( i > 5 ) break;
    }
}
//----------------------------------------
void main( void )
{
    int i = 0, j, val;

/* TODO: init unnamed semaphore */

    pthread_create( &task, NULL, (void*(*)(void*))Thread, NULL );
    
    while(1) {
        sleep(1);
        i++;

        printf( "sem post %d\n", i );
/* TODO: give semaphore */

        sem_getvalue( &sem, &val ); /* get sem value to val */
        printf( "sem value = %d\n", val );

        if ( i > 5 ) break;
        
    }
}
/****************************************
 Run:
*****************************************
wakeup 1: value = 2
wakeup 2: value = 1
wakeup 3: value = 0
sem post 1
wakeup 4: value = 0
sem value = 0
sem post 2
wakeup 5: value = 0
sem value = 0
sem post 3
wakeup 6: value = 0
sem value = 0
sem post 4
sem value = 1
sem post 5
sem value = 2
sem post 6
sem value = 3
*****************************************/
