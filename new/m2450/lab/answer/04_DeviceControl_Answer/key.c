/*
 * =====================================================================
 * NAME: 
 *       key.c
 *
 * Descriptions : 
 *       External Interrupt Test
 *
 * Developing Environment : 
 *       CodeWarrior 2.0 or Later
 *
 * =====================================================================
 */

#include "2450addr.h"
#include "option.h"
#include "my_lib.h"

extern unsigned int HandleEINT2;
extern unsigned int HandleEINT3;
extern unsigned int HandleEINT4_7;

int Key_Get_Pressed(void);
__attribute__ ((interrupt ("IRQ"))) void Key_ISR(void);

/* 
 * static void Key_EINT3_ISR(void)
 * -----------------------------------------------------------------
 */
void Key_ISR(void)
{
	int temp_GPFCON, input_value;
	
	/* GPFCON을 input mode로 변경시키기 위해 interrupt mode의 값을 저장 */
	temp_GPFCON = rGPFCON;	
	/* GPFCON을 input mode로 변경 */
	rGPFCON &= ~(0x3ff<<4);
	rGPFUDP &= ~(0x1f<<2);
	/* input 모드 상황에서 눌려진 키값을 확인 */
	while (! (input_value= Key_Get_Pressed()));
	Uart_Printf("Key Value =%d\n",  input_value);
	/* 다시 interrupt 모드로 변경 */
	rGPFCON = temp_GPFCON;
	/* Keyout 값을 0으로 설정해놓고 ISR을 빠져 나옴 */
	rGPGDAT &= ~0x1;
	rGPFDAT &= ~(0x1<<7);
	 
	/* clear global interrupt pending */
	rEINTPEND = (0x7<<4);
	rSRCPND1 = (0x7<<2);
	rINTPND1 = (0x7<<2);
}

void Key_Port_Init(void)
{
	
	/* GPFCON -Input Mode GPF[2:6] */
	/* YOUR CODE HERE */
	rGPFCON &= ~(0x3ff<<4);
	rGPFUDP &= ~(0x1f<<2);
	
	/* GPFCON, GPGCON - outputmode GPF7, GPG0 */
	/* YOUR CODE HERE */	
	rGPGDAT |= 0x1;
	rGPFDAT |= 0x1<<7;
	
	rGPGCON &= ~(0x3);
	rGPGCON |= 0x1;	
	rGPFCON &= ~(0x3<<14);
	rGPFCON |= 0x1<<14;
}

void Key_INT_Init(void)
{
	/* Set External Interrupt Vector */
	HandleEINT2 = (unsigned int)Key_ISR;
	HandleEINT3 = (unsigned int)Key_ISR;
	HandleEINT4_7 = (unsigned int)Key_ISR;
	
	/* GPFCON, GPGCON - outputmode GPF7, GPG0*/
	/* Intialize GPGDAT, GPFDAT low */
	/* YOUR CODE HERE */
	rGPGDAT &= ~0x1;
	rGPFDAT	&= ~(0x1<<7);
	
	rGPGCON &= ~(0x3);
	rGPGCON |= 0x1;	
	rGPFCON &= ~(0x3<<14);
	rGPFCON |= 0x1<<14;
		
	/* Intialize GPIO and External Interrupt Input */
	/* 
	 * GPFCON - interrupt mode
	 *	  bit[5:4] - GPF2, 10=EINT2
	 *    bit[7:6] - GPF3, 10=EINT3
	 *    bit[9:8] - GPF4, 10=EINT4
	 *    bit[11:10] - GPF5, 10=EINT5
	 *	  bit[13:12] - GPF6, 10=EINT6
	 */
	/* YOUR CODE HERE */
	rGPFCON = rGPFCON & ~(0x3ff<<4);
	rGPFCON = rGPFCON |  (0x2aa<<4);

	/* 
	 * EXTINT0
	 *    bit[10:8]  - EINT2, 000=low level, 001=high level,
	 *                01x=falling edge, 10x=rising edge, 11x=both edge
	 *    bit[14:12] - EINT3,
	 *    bit[18:16] - EINT4, 
	 *    bit[22:20] - EINT5,
	 *    bit[26:24] - EINT6,      
	 */
	rEXTINT0 = (rEXTINT0 & ~(0xfffff<<8))|(0x22222<<8);
	
	/* Intialize Interrupt Controller */
	/* EINT2~6에 해당 */
	rINTMSK1 &= ~(0x7<<2);	//2,3,4~7
	rEINTMASK &= ~(0x7<<4);	//4,5,6
}

int Key_Get_Pressed(void)
{
	int i, keyval;
	
	/* YOUR CODE HERE */
	for(i=0; i<2; i++)
	{	
		rGPFDAT |= 0x1<<7;
		rGPGDAT |= 0x1<<0;
		(i==1)?(rGPGDAT &= ~0x1):(rGPFDAT &= ~(0x1<<7));

		keyval = ((~rGPFDAT>>2) & 0x1F);
		switch(keyval)
		{
			case 1 : return (1+(5*i));
			case 2 : return (2+(5*i));
			case 4 : return (3+(5*i));
			case 8 : return (4+(5*i));
			case 16 : return (5+(5*i));
		}	
	}

	return 0;
}

int Key_Wait_Get_Pressed(void)
{
	/* YOUR CODE HERE */
	int a; 
	while (! (a= Key_Get_Pressed()));
	return a; 

}

void Key_Wait_Get_Released(void)
{
	/* YOUR CODE HERE */
	while (Key_Get_Pressed());
	Uart_Send_String("released\n");
}







