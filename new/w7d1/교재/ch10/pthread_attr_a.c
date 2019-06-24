#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>

int thread_args[3] = { 0, 1, 2 };

void* Thread( void *arg )
{
    int i;
//    nice(10); 	// only for SCHED_NORMAL
	for ( i=0; i<100; i++ ){
        printf( "%d", *(int*)arg);
    }

	fflush(stdout);	// try if you have not displayed 2s all enough !
	
	//system("ps -l -L ");

	sleep(100);	//sleep for probe

    pthread_exit(0);  
}

int main( void ) {
    int i;
    pthread_t threads[3];
	
	int policy;
	
	
 	pid_t mypid;
	
    /*TODO: thread attribute variable here */
    pthread_attr_t  thread_attrs;

    /*TODO: thread schedule parameter here */
    struct sched_param  param;

	mypid = getpid();

	policy = sched_getscheduler(0);	// get policy of self
	printf("current policy is : %s \n", (policy==0) ? "SCHED_NORMAL" : (policy==1) ? "SCHED_FIFO" :(policy==2) ? "SCHED_RR" : "else" );
	
/*
#define SCHED_NORMAL            0
#define SCHED_FIFO              1
#define SCHED_RR                2
#define SCHED_BATCH             3
*/	

#if 1
	param.sched_priority = 30;	/*	1 - 99 */
	sched_setscheduler(mypid, SCHED_FIFO, &param);	

	policy = sched_getscheduler(0);	// get policy of self
	printf("current policy is : %s \n", (policy==0) ? "SCHED_NORMAL" : (policy==1) ? "SCHED_FIFO" :(policy==2) ? "SCHED_RR" : "else" );
#endif	
	
	
    for ( i=0; i<3; i++ ) {
		
#if 1

	/*
		2 options of  : inherit( default: follow Mom's ) or explicit ( my own )
	**/

	
		/*TODO: initialize thread atrribute variable here */ 
        pthread_attr_init( &thread_attrs );

        /*TODO: set schedule policy : round-robin here */ 
        pthread_attr_setschedpolicy( &thread_attrs, SCHED_FIFO );  /* you can't change under inherit. just follow leader thread's **/

        /*TODO: set thread priority : 15 */   /* you can't change under inherit. just follow leader thread's **/
        param.sched_priority = 15;
        pthread_attr_setschedparam( &thread_attrs, &param );
#endif
        pthread_create( &threads[i], 
                        &thread_attrs, 
                        ( void* (*)(void*) )Thread, 
                        &thread_args[i] );

    }

    
	system("ps -l -L ");
	while(1)sleep(1);
    pthread_exit(0); 
}

