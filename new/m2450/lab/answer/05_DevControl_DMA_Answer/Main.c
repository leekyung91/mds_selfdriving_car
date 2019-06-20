#include "my_lib.h"
#include "option.h"
#include "2450addr.h"

extern unsigned int HandleDMA;
void WDT_Init(int timeout);

int laptime;
int dmaComplete= 0;

void DMA0_SW_Init(void);
void DMA0_Timer_Init(void);
void DMA0_UART_Init(void);
void DMA0_SW_Start(void);
void DMA0_HW_Start(void);

#define EXAMPLE 420
/*	
 * 420 : SW_DMA (�ǽ� 8-1 : SW DMA Trigger Test )
 *
 * 421 : UART_DMA (�ǽ� 8-2 : UART0 �� ���� DMA trigger Test)
 *	
 * Advanced Course
 * 1. make macro function 
 */

 void MEMCPY_SINGLE(unsigned long dst, unsigned long src, int size);
 
 /***************************************
 * 
 * Title: SW_DMA
 * 
 ***************************************/

#if EXAMPLE == 420

//** ISR Function declaration 
 __attribute__((interrupt("IRQ"))) void DMA0_ISR(void);
// Global Variables Declaration
// �������� CACHE ������ �ƴҰ�
unsigned int src=0x33F10000;
unsigned int dst=0x33F20000;
unsigned int size = 480; /* byte size */
unsigned int pattern;

 __attribute__((interrupt("IRQ"))) void DMA0_ISR(void)
{
	/* TO DO: Pendng Clear on DMA1 */
	rSUBSRCPND = 0x1<<18;
	rSRCPND1 = 0x1<<17;
	rINTPND1 = 0x1<<17;

	/*  TO DO: Stop DMA0 trigger  */
	rDMASKTRIG0 |= 0x1<<2;

	Uart_Printf("__irq ISR������  ");	
	MemDump(dst, size/4+4); 	
}

void Main(void)
{	
	char value;

	// MMU �ʱ�ȭ
	MMU_Init();
	
	Uart_Init(115200);	
	DMA0_SW_Init();

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* ���ͷ�Ʈ ���Ϳ� DMA0_ISR �Լ� ��� */
	HandleDMA = (unsigned int)DMA0_ISR;

	/*  ���ͷ�Ʈ ��� on DMA0 */		
	rINTMSK1 &= ~(0x1<<17);

	DMA0_SW_Start();
}
# endif
 

/***************************************
 * 
 * Title: UART_DMA
 * 
 ***************************************/

#if EXAMPLE == 421


//** ISR Function declaration 
 __attribute__((interrupt("IRQ"))) void DMA0_ISR(void);
// Global Variables Declaration
// �������� CACHE ������ �ƴҰ�
unsigned int src=0x33F10000;
unsigned int dst=0x33F20000;
unsigned int size = 480; /* byte size */
unsigned int pattern;

 __attribute__((interrupt("IRQ"))) void DMA0_ISR(void)
{
	/* TO DO: Pendng Clear on DMA1 */
	rSUBSRCPND = 0x1<<19;
	rSRCPND1 = 0x1<<17;
	rINTPND1 = 0x1<<17;

	dmaComplete = 1; // DMA ���ۿϷ�
}

void Main(void)
{	
	// MMU �ʱ�ȭ
	MMU_Init();
	
	Uart_Init(115200);	
	DMA0_UART_Init();

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* ���ͷ�Ʈ ���Ϳ� DMA0_ISR �Լ� ��� */
	HandleDMA = (unsigned int)DMA0_ISR;
		
	/*  ���ͷ�Ʈ ��� on DMA */		
	rINTMSK1 &= ~(0x1<<17);
	
	while(1){
	Uart_Printf("your input character\n");	
	    while(dmaComplete == 0); // DMA ������ �Ϸ�ɶ����� ��ٸ�...
	dmaComplete= 0;
	Uart_Printf("your input character is %s \n",  (unsigned char*)dst);
	}
}
# endif