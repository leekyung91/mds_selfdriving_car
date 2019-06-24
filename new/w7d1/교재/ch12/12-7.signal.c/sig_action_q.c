/***************************************
 * Filename: sig_action.c
 * Title: Signal 
 * Desc: Alarm Signal Example
 * Revision History & Comments
 *
 * ***************************************/
#include <signal.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdio.h> 

void sig_int(int signo);
void sig_usr(int signo);

//---------------------------------------------------------------
int main()
{
    int i = 0;
    struct sigaction intsig, usrsig;

    usrsig.sa_handler = sig_usr;
    sigemptyset(&usrsig.sa_mask);
    usrsig.sa_flags = 0;

    intsig.sa_handler = sig_int;
    sigemptyset(&intsig.sa_mask);
    intsig.sa_flags = 0;

	/* TODO: SIGINT에 대해 sigaction으로 intsig를 핸들링한다. */

	/* TODO: SIGUSR2에 대해 sigaction으로 usrsig를 핸들링한다. */

	while(1)
    {
        printf("%d\n", i);
        i++;
        sleep(1);
    }
}
//---------------------------------------------------------------
void sig_int(int signo)
{
    sigset_t sigset, oldset;
    sigfillset(&sigset);
    // 새로들어오는 모든 시그널에 대해서 block 한다. 
    if (sigprocmask(SIG_BLOCK, &sigset, &oldset) < 0)
    {
        printf("sigprocmask %d error \n", signo);
    }
    fprintf(stderr, "SIGINT !!!!\n");
    sleep(5);
}
//---------------------------------------------------------------
void sig_usr(int signo)
{
    printf("sig_usr2\n");
}
//---------------------------------------------------------------

