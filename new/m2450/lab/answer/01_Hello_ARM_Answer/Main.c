/*=========================*/
#include "my_lib.h"
#include "option.h"
#include "2450addr.h"

 void HOW_TO_RETURN(void);
 int CONDITIONAL_EXECUTE(int a, int b);
 int DATA_PROCESS1(int a, int b, int c);
 unsigned long DATA_PROCESS2(unsigned long a, unsigned long b);
 int SUM_OF_DEC(int start, int end);
 int GRT_COM_MES(int a, int b);
 void MEMCPY_SINGLE(unsigned long dst, unsigned long src, int size);
 void MEMCPY_MULTIPLE(unsigned long dst, unsigned long src, int size);
 void MEMCPY(unsigned long dst, unsigned long src, int size);

void Main(void)
{
	Uart_Init(115200);
	Uart_Printf("\nHello ARM Processor!!!\n\n");
	
	/* 
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	 *   FROM HERE WE WILL TEST ARM INSTRUCTION
	 */

#if 1
	/* Testing how to return */
	Uart_Send_String("Start How-to-return test\n");
	HOW_TO_RETURN();
    	Uart_Send_String("Success\n");
#endif

#if 1
	/* Testing conditional execution */
{
	/* 
	 * GET A RESULT
	 *    if (a < b) result = 1;
	 *    else if (a > b) result = 2
	 *    else if (a == b) result 3
	 */
	int a,b, result;
	
	a=9;b=11; /* a < b condition */
	Uart_Printf("Compare %d and %d => ", a, b);
    	result = CONDITIONAL_EXECUTE(a,b);
    	Uart_Printf("Result : %d (must 1)\n",result);
    
	a=11;b=10; /* a > b condition */
	Uart_Printf("Compare %d and %d => ", a, b);
    	result = CONDITIONAL_EXECUTE(a,b);
    	Uart_Printf("Result : %d (must 2)\n",result);
    
	a=10;b=10; /* a == b condition */
	Uart_Printf("Compare %d and %d => ", a, b);
    	result = CONDITIONAL_EXECUTE(a,b);
    	Uart_Printf("Result : %d (must 3)\n",result);        
}
#endif


#if 1
	/* IMPLEMENT the function for calcurate result=(a+b)-c */
{
	int a,b,c;
	int result;
	
	a = 11;
	b = 22;
	c = 30;
	
	Uart_Printf("Calcurate [(%d+%d)-%d] ",a,b,c);
	result = DATA_PROCESS1(a,b,c);
	Uart_Printf("=> Result is %d (must 3)\n",result);	
}
#endif

#if 1
	/* IMPLEMENT the function for calcurate result=(a<<2) | (b&15) */
{
	unsigned long a,b;
	unsigned long result;
	
	a = 0x10;
	b = 0x33;
	/*
	 * unsigned long function(a,b)
	 */
	Uart_Printf("Calcurate [(0x%08x << 2) | (0x%08x & 15)] \n",a,b);	
	result = DATA_PROCESS2(a,b);
	Uart_Printf("   => Result is 0x%08x (must 0x00000043)\n",result);	
}
#endif

#if 1
	/* Sum of Decimal Test */
{
	int start, end, sum;
	start = 1;
	end = 100;

	Uart_Printf("Start sum of decimal test\n");
	
	sum = SUM_OF_DEC(start,end);
	Uart_Printf("ASM : Sum from %d to %d is [%d]\n",start,end,sum);

	sum = 0;
	for (start=1;start<=end;start++) {
		sum += start;
	}
	Uart_Printf("C   : Sum from %d to %d is [%d]\n",1,end,sum);
}
#endif

	
#if 1
	/* Testing MES Test */
{
	
	int a=30;
	int b=18;
	int mes;
	Uart_Printf("\n*** Test GRT_COM_MES ***\n");
	mes = GRT_COM_MES(a,b);
	Uart_Printf("MES = %d \n", mes); 
}
#endif 

#if 1
	/* Memory Copy */
{
	unsigned long src=0x33000000;
	unsigned long dst=0x33100000;
	int size = 12; /* word size */
	unsigned long pattern;

	MemFill(src, 0x00000000, size);
	MemFill(dst, 0x00000000, size);	
	Uart_Printf("Start Memory copy test\n");

#if 1
	/* memory copy test with single LDR/STR */
	pattern = 0x5555aaaa;
	Uart_Printf("\nFill pattern [0x%08x] to [0x%08x]\n",pattern, src);
	
	MemFill(src, pattern, size);	
	MemDump(src, size);

	Uart_Printf("\nCopy from [0x%08x] to [0x%08x] by use LDR/STR\n",src, dst);
	MEMCPY_SINGLE(dst,src,size);
	MemDump(dst, size);
#endif

#if 1	
	/* memory copy test with multiple LDM/STM */
	pattern = 0x5a5a5a5a;
	Uart_Printf("\nFill pattern [0x%08x] to [0x%08x]\n",pattern, src);
	MemFill(src, pattern, size);
	MemDump(src, size);
	Uart_Printf("\nCopy from [0x%08x] to [0x%08x] by use LDM/STM\n",src, dst);
	MEMCPY_MULTIPLE(dst,src,size);
	MemDump(dst, size);
#endif

#if 1
	/* 
	 * IMPLEMENT user library for memory copy 43 byte 
	 */
	pattern = 0x5555aaaa;
	Uart_Printf("\nFill pattern [0x%08x] to [0x%08x]\n",pattern, src);
	MemFill(src, pattern, size);
	MemDump(src, size);

	Uart_Printf("\nCopy 43 byte from [0x%08x] to [0x%08x] \n",src, dst);
	MEMCPY(dst,src,43); /* 43 is byte count */
	MemDump(dst, size);	
#endif
}
#endif

	Uart_Send_String("\n\n");
}

