/***************************************
 * Filename: use-alarms.c
 * Title: Signal 
 * Desc: Alarm Signal Example
 * Revision History & Comments
 *
 * ***************************************/
#include <stdio.h>     /* standard I/O functions                         */
#include <unistd.h>    /* standard unix functions, like alarm()          */
#include <signal.h>    /* signal name macros, and the signal() prototype */

char user[40];		/* buffer to read user name from the user */

/* define an alarm signal handler. */
void catch_alarm(int sig_num)
{
    printf("Operation timed out. Exiting...\n\n");
    exit(0);
}

int main(int argc, char* argv[])
{
    /* set a signal handler for ALRM signals */
    signal(SIGALRM, catch_alarm);

    /* prompt the user for input */
    printf("10-secs timeout input system activated !!!\n");
    printf("Username: ");
    fflush(stdout);
    /* start a 30 seconds alarm */
    alarm(10);
    /* wait for user input */
    gets(user);
    /* remove the timer, now that we've got the user's input */
    alarm(0);

    printf("User name: '%s'\n", user);

    return 0;
}


