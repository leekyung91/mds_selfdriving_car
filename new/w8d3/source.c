/*****************************************************
 *
 * myproject.c
 *
 * Description : Hello World in C, ANSI-style
 *
 */

#include <stdio.h>
enum tag
{
	A = 100,	// 8bit로 충분해, 256이면 9bit 필요 -> 데이터가 가변할 수 있어
	B	//101
};
enum tag1
{
	C = 123453,	// 8bit로 충분해, 256이면 9bit 필요 -> 데이터가 가변할 수 있어
	D	//101
};
void func(int arg1, int* arg2)
{

}
int main(void)
{
	volatile float f = 3.14;
	volatile double d = 3.14;
	volatile int tmp = 0;

	volatile enum tag AAA;
	volatile enum tag1 BBB;

	tmp = sizeof(AAA);
	tmp = sizeof(BBB);


	if(f == d)
	{
		tmp = 1;
	}
	else
	{
		tmp = 2;
	}

}
