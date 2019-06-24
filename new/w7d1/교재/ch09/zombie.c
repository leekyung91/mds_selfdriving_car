/***************************************
 * Filename: zombie.c
 * Title: Process Management
 * Desc: Zombie Process
 * ***************************************/
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>

int main()
{
   pid_t pid;
      
   pid = fork();

   if ( pid != 0 )  { /* parent process */
	while(1) sleep(1000);
   }
   else { /* child  process */
	exit(42);
   }

}
