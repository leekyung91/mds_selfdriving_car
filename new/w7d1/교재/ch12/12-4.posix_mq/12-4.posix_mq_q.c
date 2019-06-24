#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>
#include <time.h>

/* TODO: define message queue variable */

;

void send_thread( void ) {
	char c = 'A';
	unsigned int prio = 0;

	while (1) {
		printf( "S(%c, prio = %d)\n", c, prio );
/* TODO: send c via message-q */ 
;

		if ( ++c > 'z' ) c = 'A';
		if ( ++prio > 10 ) prio = 0;

		sleep(1);
	}
}

void recv_thread( void ) {
	char msg[64];
	int len;
	unsigned int prio=0;

	while (1) {
;
		len = mq_receive(mq, msg, sizeof(msg), &prio);
		sleep(1);

		if ( len > 0 ) {
			msg[len] = '\0';
			printf( "\t\tR(%s, prio = %d)\n", msg, prio );
		}
	}
}

void main( void ) {
	pthread_t threads[2];
	pthread_attr_t      attr0, attr1;

/*TODO: define message-q attribute variable, attr */ 
;

/*TODO: */ 
	attr.mq_maxmsg  =  3;
	attr.mq_msgsize = 10;
	attr.mq_flags   =  0;

/*TODO: open message_q file /w O_RDWR | O_CREAT flags, use attr also */
;
	if(mq == -1)
	{
		perror("mq_open error");
		exit(0);
	}	

	pthread_attr_init(&attr0); pthread_attr_init(&attr1);
	pthread_create(&threads[0], &attr0, (void*)recv_thread, NULL);
	pthread_create(&threads[1], &attr1, (void*)send_thread, NULL);

	pthread_exit(0);
}

