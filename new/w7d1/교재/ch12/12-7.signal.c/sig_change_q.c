/***************************************
 * Filename: sig_change.c
 * Title: Signal
 * Desc: Signal Change Example
 * Revision History & Comments
 *
 * ***************************************/
#include <stdio.h>
#include <signal.h>
 
void sigproc(void);
 
void quitproc(void); 
 
main()
{ 
	/* TODO: SIGINT를 잡아 sigproc 핸들러를 구동한다. */
	/* TODO: SIGQUIT를 잡아 quitproc 핸들러를 구동한다. */

	printf("ctrl-c disabled use ctrl-\\ to quit\n");
	for(;;); /* infinite loop */
}
 
void sigproc()
{
	signal(SIGINT, sigproc); /*  */
	printf("you have pressed ctrl-c \n");
}
 
void quitproc()
{
	printf("ctrl-\\ pressed to quit\n");
	exit(0); /* normal exit status */
}
