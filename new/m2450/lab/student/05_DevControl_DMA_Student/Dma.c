/*
 * =====================================================================
 * NAME         : Dma.c
 *
 * Descriptions : DMA Test Driver
 *
 * IDE          : CodeWarrior 2.0 or Later
 *
 * Modification
 *     Modified by MDS Tech. NT Div.(2Gr) (2007.3.1~4)
 * =====================================================================
 */
#include "2450addr.h"
#include "my_lib.h"

// Global Variables Declaration
// 목적지는 CACHE 영역이 아닐것
extern unsigned long src;
extern unsigned long dst;
extern unsigned int size;
extern unsigned long pattern;

//Function Declatation
void DMA0_SW_Init(void);
void DMA0_Timer_Init(void);
void DMA0_UART_Init(void);
void DMA0_SW_Start(void);
void DMA0_HW_Start(void);

void DMA0_SW_Init(void)
{
	/* Init DMASKTRIG on DMA0*/
	rDMASKTRIG0 = 0x0;
	
	/* TODO : Select Base Address of Source and Destination*/
	rDISRC0 = src;
	rDIDST0 = dst;
	
	/*  Init Source & Destination Control Register on DMA0 
	 * (memory -> AHB bus, INC mode, CHK_INT = after auto-reload)
	 */
	/* TODO : */ 
	rDISRCC0 = (0 << 1) | (0 << 0);
	rDIDSTC0 = (0 << 2) | (0 << 1) | (0 << 0);

	/* TODO : setting for S/W DMA request
	 * DCON0 : Handshake[31-1], HCLK[30-1], INT mode[29-1], Single Unit[28-0], Whole service[27-1],
	 * HW_select[26:24],  S/W trigger[23-0], Auto Reload ON[22-0], Byte size[21:20](Note: Word size<->Byte size)
	 */
	rDCON0 = (1 << 31) | (1 << 30) | (1 << 29) | (0 << 28) | (1 << 27) | (0 << 24) | (0 << 23) \	
	| (1 << 22) | (2 << 20) | ((120/4) << 0);
	// size/4 : size 계산해서 120 나온것!! size/4이렇게 써도 상관은 없엉 

	rDMAREQSEL0 &=~ (1<<0); // software trigger // 	SWHW_SEL

}

void DMA0_UART_Init(void)
{
	/* Init DMASKTRIG on DMA0*/
	rDMASKTRIG1 = 0x0;
	
	/* TODO : Select Base Address of Source and Destination*/
	rDISRC1 = &rURXH1; //rURXH1 register's address // rs buffer address 주소 적어!!
	rDIDST1 = dst;
	
	/* TODO : Init Source & Destination Control Register on DMA1 
	 * (memory -> AHB bus, INC mode, CHK_INT -> after auto-reload )
   	 */
	rDISRCC1 = (1 << 1) | (1 << 0); // APB, FIXED
	rDIDSTC1 = (0 << 2) | (0 << 1) | (0 << 0);

	
	/* TODO : setting for H/W DMA request
	 * DCON1 : Handshake[31-1], HCLK[30-1], INT mode[29-1], Single Unit[28-0], Whole service[27-1], 
	 * Uart1[26:24], H/W trigger[23-1], Auto Reload ON[22-0], Byte size[21:20](Note: Word size<->Byte size)
	 */
	// 주석은 2440 전 시리즈인듯... 23,24 비트 주의 깊게 보기!!
	rDCON1 = (1 << 31) | (0 << 30) | (1 << 29) | (0 << 28) | (0 << 27) | (1 << 24) | (0 << 23) \
	| (0 << 22) | (0 << 20) | (8 << 0); // 8<<0 -> 8글자만 바꾸는것 

//DMA channel : 1
	rDMAREQSEL1 = (rDMAREQSEL1 &~ (0x1f<<1))|(22<<1); // hardware trigger ,DMA REQUEST SOURCES
	rDMAREQSEL1 |= 1;	// IO 장치 사용하기 때문에?

	 /* TODO : Turn on Trigger*/
	rDMASKTRIG1 |= (1 << 1); // DMA CHANNEL 'ON'	
}

void DMA0_SW_Start(void)
{
	MemFill(src, 0x00000000, size);
	MemFill(dst, 0x00000000, size);	
	Uart_Printf("\nStart Memory copy test With DMA\n");	
	
	/* memory copy test with DMA */
	pattern = 0x5555aaaa;
	Uart_Printf("Fill pattern [0x%08x] to [0x%08x]\n",pattern, src);
	MemFill(src, pattern, size);
	MemDump(src, size);
	Uart_Printf("\nCopy from [0x%08x] to [0x%08x] by DMA\n",src, dst);
	
	/* TODO : Turn on Trigger*/
	rDMASKTRIG0 |= (1<<1); // DMA CHANNEL 'ON'
	
	/* TODO : Start S/W Trigger in S/W Request mode */
	rDMASKTRIG0 |= 1; // DMA START!!!
}

void DMA0_HW_Start(void)
{	
	MemFill(src, 0x00000000, size);
	MemFill(dst, 0x00000000, size);	
	Uart_Printf("\nStart Memory copy test With DMA\n");	
}
