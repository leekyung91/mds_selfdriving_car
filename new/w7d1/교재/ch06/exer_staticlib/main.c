#include "mylib.h"
#include <stdio.h>

int main()
{
	int a, b;
	a=100;
	b=50;

	printf("sum; %d\n", sum(a, b));
	printf("diff: %d\n", diff(a,b));
	return 0;
}
