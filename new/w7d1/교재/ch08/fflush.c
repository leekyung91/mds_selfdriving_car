#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	int i=1;
	
	while(1)
	{
		i++;
		printf(".");
		fflush(stdout);
		usleep(2000);
	}

	return 0;
}

