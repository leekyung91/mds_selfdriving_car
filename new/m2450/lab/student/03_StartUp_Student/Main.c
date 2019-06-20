/*=========================*/
#include "my_lib.h"
#include "option.h"
#include "2450addr.h"

void Led_Port_Init(void);
void Led_Display(int data);

void Main(void)
{
	int i, j;
	
	Uart_Init(115200);
	Uart_Send_String("\nLED Test !!!\n");

	/* LED Test */
	Led_Port_Init();
	while(1){
		for(i=0;i<16;i++) {
			Led_Display(i);
			for(j=0;j<0x1ffff;j++);
		}
	}
}
