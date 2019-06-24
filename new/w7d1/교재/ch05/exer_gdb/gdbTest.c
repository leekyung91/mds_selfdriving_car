#include <stdio.h>
#include <stdlib.h>
main()
{
	int i;
	double j;

	for(i=0; i<5; i++){

		j=i/2+i;
		printf("j is %f\n", j);
		*(volatile unsigned int*)(main+10) = 1; 
	}
}
