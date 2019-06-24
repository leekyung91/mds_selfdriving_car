#include <stdio.h>
#include <sys/types.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	struct sched_param param;
	int policy;
	char *policy_name;

	pid_t current_pid = getpid();
/*
	only superuser may lower the priorities and change schedule
*/
	sched_getparam(current_pid, &param);
	param.sched_priority = 65;
	sched_setscheduler(current_pid, SCHED_FIFO, &param);

	sched_getparam(current_pid, &param);
	policy = sched_getscheduler(current_pid);

	if      (policy == 0) 	policy_name = "SCHED_OTHER";
	else if (policy == 1) 	policy_name = "SCHED_FIFO";
	else if (policy == 2) 	policy_name = "SCHED_RR";
	else 			policy_name = "Unknown";
	printf("Current PID: %d\n", (int)current_pid);
	printf("Scheduling Policy: %d, %s\n", policy, policy_name );
	printf("RT Sched Priority: %d  \n\n", param.sched_priority);

	puts("#ps -l to check the priority");
	sleep(100);

	return 0;
}

