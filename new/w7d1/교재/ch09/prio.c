#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

int main(void)
{
	//get process id of this process
	pid_t current_pid = getpid();

	//get nice   of this process	
	int nicevalue = getpriority(PRIO_PROCESS, current_pid);
	printf("Process: %d, nice: %d\n", current_pid, nicevalue);
	sleep(1);

/*
	Try cases below... 

	case 1: $ ./a.out &
		$ ps -l
	case 2: # ./a.out &
		# ps -l
	case 3: $ nice  -3 ./a.out &
		$ ps -l
	case 4: # nice --3 ./a.out &
		# ps -l

*/
	/*  nice by +5 */
	setpriority(PRIO_PROCESS, current_pid, 5);
	nicevalue = getpriority(PRIO_PROCESS, current_pid);
	printf("Process: %d, nice: %d\n", current_pid, nicevalue);

	system("ps -l");

	sleep(1);

	nice(-10); /* only superuser may lower nice */
	nicevalue = getpriority(PRIO_PROCESS, current_pid);
	printf("Process: %d, nice: %d\n", current_pid, nicevalue);

	system("ps -l");

	return 0;
}

