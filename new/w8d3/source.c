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

	/*
	volatile float f = 6.625;
	volatile double d = 6.625;

	volatile int tmp;

	volatile double a = d + 1.1;

	func(tmp, &tmp);
	// 다시 함수를 호출하면 중요하지 않은 레지스터가 중요한 레지스터로 변경
	tmp = arg1 + *arg2;
	// 손상을 입힌 상태에서 접근,,,?
	// 백업을 안해둔 상태에서 접근 ?
	// mov16 d8, d4
	// mov16.aa a15, a4
	// lower context는 중요하지 않아서 백업이 안되 -> 근데 가끔 이렇게 중요해 질수도
	// 그래서 자동으로 백업이 된다.
	// a15, d8이 중요한 레지스터인데 여기로 백업이 된다.
	// 상황에 따라 중요하지않다. 중요해지면 디버거에서 자둉적으로 중요 레지스터로 백업이된다.

	// call : 해당 주소에 점프뛰는 것,


	func(tmp, &tmp);

	if(sizeof(f) == sizeof(d))
	{
		tmp = 1 ;

	}
	else
	{
		tmp = 2;
	}
	*/
}
int main(void)
{
	volatile float f = 3.14;
	// st16 w (4byte)
	volatile double d = 3.14;
	// st.b (8byte)
	volatile int tmp = 0;

	volatile enum tag AAA;
	volatile enum tag1 BBB;

	tmp = sizeof(AAA);
	tmp = sizeof(BBB);

//	AAA + 10;

	if(f == d)
	{
		tmp = 1;
	}
	else
	{
		tmp = 2;
	}
	/*
	volatile float f = 6.625;
	volatile double d = 6.625;

	volatile int tmp;

	volatile double a = d + 1.1;

	func(tmp, &tmp);
		// ld.w 	d4,[a10]0x8
		// d4에 tmp 주소를 가져오는 것
		// lea 	a4,[a10]0x8

	if(sizeof(f) == sizeof(d))
	{
		tmp = 1;

	}
	else
	{
		tmp = 2;
	}
	*/
}
