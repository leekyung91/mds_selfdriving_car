#include "2450addr.h"
#include "option.h"
#include "my_lib.h"

void Timer0_Init(void)
{
	/* 
	* 	Timer0 Init 
	* Prescaler value : 255, dead zone length = 0
	* Divider value : 1/16, no DMA mode
	* New frequency : (PCLK/(Prescaler value+1))*Divider value = (66Mhz/(256))*(1/16)
	*				= 16.113Khz(16113Hz)
	*/
	rTCFG0 = (0<<8)|(0xff); 
	rTCFG1 = (0<<20)|(3); 
	
	/* TCON���� :Dead zone disable,  auto reload on, output inverter off
	*  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
	*/
	rTCON  = (0<<4)|(0<<3)|(0<<2)|(0<<1)|(0);
	rTCNTB0 = 0;
	rTCMPB0 = 0;
 
}

void Timer0_Delay(int msec)
{
	/*
	* 1) TCNTB0���� : �Ѱܹ޴� data�� ������ msec�̴�.
	*                  ���� msec�� �״�� TCNTB0������ ������ ���� ����.
	* 2) manual update�Ŀ�  timer0�� start��Ų��. 
	* 	 note : The bit has to be cleared at next writing.
	* 3) TCNTO0���� 0�� �ɶ����� ��ٸ���. 	
	*/
	rTCNTB0 = 16.113*msec;	

	rTCON |= (1<<1)|(0);
	rTCON &= ~(1<<1);
	
	rTCON |= 1;	
	
	while(rTCNTO0 != 0);
	rTCON &=~ 1;	
}


//
// Use Timer0 for PC_ElapsedStart
// Timer INPUT clock Resolution 2.0625MHz
void ElapseTimer_Start(void)
{
 rTCFG0 = (rTCFG0 & ~0xff) | 1;
 rTCFG1 = (rTCFG1 & ~0xf) | 3; 
 
 /* TCON���� :Dead zone disable,  auto reload on, output inverter off
 *  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
 */
 rTCON  = (rTCON & ~((1<<4)|(1<<2)|(1<<1)|(1))) | (1<<3);

 rTCNTB0 = 0;
 rTCMPB0 = 0;

 rTCNTB0 = 0xffff; // initial value(65535) for timer

 rTCON |= (1<<1)|(0);
 rTCON &= ~(1<<1);
 rTCON |= 1;
}

//
// Use Timer0 for PC_ElapsedStart
// Timer Resolution 1.5625MHz
unsigned int ElapseTimer_Stop(void)
{
 rTCON &= ~1;

 return (0xffff - rTCNTO0);
}

