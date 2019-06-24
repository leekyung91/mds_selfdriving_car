#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
  
void children_role(void) {
	printf("[CHILD]Yes, Mom!!!!\n");
	printf("[CHILD]OK! OK!\n");
	
	exit(0xAB);	// 1 Byte of exit code
}

void parent_role(void) {
	printf("[PARENT] Do you homework!\n");
	printf("[PARENT] Clean your room!\n");
	sleep(1);
	printf("[PARENT] Be quiet!\n");
	printf("[PARENT] Do not mess up the table!\n");
}

int main(void) {
	pid_t 	pid;

	printf("[App] fork() at here!\n");
	
	pid = fork();

	switch(pid) {
		case -1: 
			perror("fork failed");
			exit(1);
		case 0: 
			children_role();
			break;
		default:
			parent_role();
			break;
	}

	if(pid != 0) {
		int status;
		pid_t child_pid;
		child_pid = wait(&status);
		printf("[PARENT] child finished: %d\n", child_pid);
//		printf("%x \n", WIFEXITED(status) );	
		if (WIFEXITED(status)) {
			printf("[PARENT] child exited with code %X\n", WEXITSTATUS(status));
		}
	}

	return 0;
}

	
