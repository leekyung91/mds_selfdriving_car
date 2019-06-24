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
    /* TODO: set a signal handler for ALRM signals */

    /* prompt the user for input */
    printf("Username: ");
    fflush(stdout);
    /* TODO: start a 30 seconds alarm */

    /* wait for user input */
    gets(user);
    /* TODO: remove the timer, now that we've got the user's input */


    printf("User name: '%s'\n", user);

    return 0;
}


