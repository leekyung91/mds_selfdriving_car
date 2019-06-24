#include <unistd.h> 
#include <stdio.h>

main() 
{ 
	printf("before executing ls -l\n"); 
	execl("/bin/ls", "ls", "-l", (char *)0); 	/* 인자의 나열이 끝났음을 의미한다. */
	printf("after executing ls -l\n");  /* exec 호출이 성공하면 실행되지 않는다*/

	return 0;
} 
