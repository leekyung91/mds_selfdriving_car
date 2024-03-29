#include "2450addr.h"
#include "option.h"
#include "my_lib.h"

void Timer_Init(void)
{
	/* 
	* 	Timer0 Init 
	* Prescaler value : 255, dead zone length = 0
	* Divider value : 1/16, no DMA mode
	* New frequency : (PCLK/(Prescaler value+1))*Divider value = (66Mhz/(256))*(1/16)
	*				= 16.113Khz(16113Hz)
	*/
 /* TODO : Timer Clock Configure */
	/* prescaler, divider setting */
	rTCFG0 = (rTCFG0 &~ (0xff<<0))|255; 
	rTCFG1 = (rTCFG1 &~ (0xf<<0))|3; 
	
	/* TCON설정 :Dead zone disable,  auto reload on, output inverter off
	*  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
	*/
	rTCON  = (0<<4)|(1<<3)|(0<<2)|(0<<1)|(0);
	rTCNTB0 = 0; //Timer_Delay()
	rTCMPB0 = 0;
 
}

void Timer_Delay(int msec)
{
	/*
	* 1) TCNTB0설정 : 넘겨받는 data의 단위는 msec이다.
	*                  따라서 msec가 그대로 TCNTB0값으로 설정될 수는 없다.
	* 2) manual update후에  timer0를 start시킨다. 
	* 	 note : The bit has to be cleared at next writing.
	* 3) TCNTO0값이 0이 될때까지 기다린다. 	
	*/
 /* TODO : Timer Counter Setting */
	rTCNTB0 = 16.113 * msec; //(16113/1000) * (1000)

	// 삼성칩에서만 국한된 방식
	rTCON |= (1<<1)|(0);	// manual update
	rTCON &= ~(1<<1);
	
	rTCON |= 1;	// trigger(timer start)
	
	while(rTCNTO0 != 0);
	
}


//
// Use Timer0 for PC_ElapsedStart
// Timer INPUT clock Resolution 1.5625MHz
void ElapseTimer_Start(void)
{
 rTCFG0 = (rTCFG0 & ~0xff) | 1;
 rTCFG1 = (rTCFG1 & ~0xf) | 3; 
 
 /* TCON설정 :Dead zone disable,  auto reload on, output inverter off
 *  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
 */
 rTCON  = (rTCON & ~((1<<4)|(1<<2)|(1<<1)|(1))) | (1<<3);

 rTCNTB0 = 0;
 rTCMPB0 = 0;

 rTCNTB0 = 0xffff; // initial value(65535) for timer, 다운카운트이기에 최대치로 

 // manual update
 rTCON |= (1<<1)|(0);
 rTCON &= ~(1<<1);
 // count
 rTCON |= 1;
}

//
// Use Timer0 for PC_ElapsedStart
// Timer Resolution 1.5625MHz
unsigned int ElapseTimer_Stop(void)
{
 rTCON &= ~1;

 return (0xffff - rTCNTO0)/1562.500; // 0xffff : 65535 (max value)
 // 1562.500 : 주기(ms를 받기 위해)
}

