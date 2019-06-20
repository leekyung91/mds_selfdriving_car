/*=========================*/
#include "my_lib.h"
#include "option.h"
#include "2450addr.h"

void start_timer0(void);
void stop_timer0(void);
int time (void);

void Main(void)
{
	int i;
	Uart_Init(115200);
	/* 
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	 *   FROM HERE WE WILL TEST MMU
	 */
	
#if 1 /* MMU INIT */
	Uart_Printf("\nInitialize MMU..\n");
	
	/* 
	 * Initialize MMU 
	 * The MMU initialize routine was implemented at libc.c
	 */
	MMU_Init();
#else
	Uart_Printf("No-MMU..\n");
#endif

#if 1
{
	volatile unsigned long *value;
	value = (volatile unsigned long *)0x33000000;
	*value = 0x0;

	Uart_Printf("Performance test start ..\n");
	start_timer0();
	for (i=0;i<0x50000;i++)
		*value = *value + 1;
	
	Uart_Printf("Time spend for testing 0x%x.\n", time());
	/* 
	 * PLEASE COMPARE the each time
	 *	1. Cache, Write Buffer disable for TEXT/DATA and TEST area
	 *  2. TEXT/DATA area : Cache, Write Buffer enable
	 *     TEST area : Cache, Write Buffer DISable
	 *	3. Cache, Write Buffer ENable for TEXT/DATA and TEST area
	 */
	stop_timer0();
}
#endif

	Uart_Printf("MMU Test End\n");
	Uart_Send_String("\n\n");
}
