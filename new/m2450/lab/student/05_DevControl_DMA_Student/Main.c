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
 * 420 : SW_DMA (실습 8-1 : SW DMA Trigger Test )
 *		메모리에서 메모리 복사?, mem to mem
 * 421 : UART_DMA (실습 8-2 : UART0 에 의한 DMA trigger Test)
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
// 목적지는 CACHE 영역이 아닐것(DCache 'off')
unsigned int src=0x33F10000;
unsigned int dst=0x33F20000;
unsigned int size = 480; /* byte size */
unsigned int pattern;

 __attribute__((interrupt("IRQ"))) void DMA0_ISR(void)
{
	/* TODO: Pendng Clear on DMA0 */
	// 인터럽트 상태 클리어
	rSUBSRCPND = 0x1<<18;
	rSRCPND1 = 0x1<<17;	// INT_DMA req
	rINTPND1 = 0x1<<17;

	/*  TODO: Stop DMA0 trigger  */
	rDMASKTRIG0 |= 0x1<<2;

	Uart_Printf("__irq ISR실행결과  ");	
	dmaComplete=1;
	MemDump(dst, size+16); 	
}

void Main(void)
{	
	char value;

	// MMU 초기화
	MMU_Init();
	
	Uart_Init(115200);	
	DMA0_SW_Init();	//DMA 초기화 

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* 인터럽트 벡터에 DMA0_ISR 함수 등록 */
	HandleDMA = (unsigned int)DMA0_ISR; //  DMA 완료후 인터럽트 핸들러 실행

	/*  인터럽트 허용 on DMA0 */		
	rINTMSK1 &= ~(0x1<<17);

	DMA0_SW_Start();	//DMA start
	// ElapseTimer_start();
	cnt=0;
	while(1)
	{
		cnt++;
		if(dmaComplete){
			// elaspeTime = ElapseTimer_stop();
			// printf(elaspeTime);
			printf("cnt=%d\n",cnt);
		}
	}
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
// 목적지는 CACHE 영역이 아닐것
unsigned int src=0x33F10000;
unsigned int dst=0x33F20000;
unsigned int size = 480; /* byte size */
unsigned int pattern;

 __attribute__((interrupt("IRQ"))) void DMA0_ISR(void)
{
	/* TODO: Pendng Clear on DMA1 */
	rSUBSRCPND = 0x1<<19; //DMA 1 -> 19bit 
	rSRCPND1 = 0x1<<17;
	rINTPND1 = 0x1<<17;

	Uart_Printf("__irq ISR실행결과  ");	
	MemDump(dst, 12); 	// 화면에 찍고싶으면 
	
	dmaComplete = 1; // DMA 전송완료
}

void Main(void)
{	
	// MMU 초기화
	MMU_Init();
	
	Uart_Init(115200);	
	DMA0_UART_Init();

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* 인터럽트 벡터에 DMA0_ISR 함수 등록 */
	HandleDMA = (unsigned int)DMA0_ISR;
		
	/*  인터럽트 허용 on DMA */		
	rINTMSK1 &= ~(0x1<<17);

	DMA0_HW_Start();
	
	while(1){
	Uart_Printf("your input character\n");	
	    while(dmaComplete == 0); // DMA 전송이 완료될때까지 기다림...
	dmaComplete= 0;
	Uart_Printf("your input character is %s \n",  (unsigned char*)dst);
	}
}
# endif
