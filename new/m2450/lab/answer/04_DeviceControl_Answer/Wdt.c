/* WDT Related Libraies */

#include "2450addr.h"
#include "option.h"
#include "macro.h"

void WDT_Init(int timeout);
void WDT_ISR_Enable(int enable);
void WDT_Start(int start);
void WDT_ISR_Init(void (*fp)(void));
void WDT_Clear(unsigned int msec);
void WDT_Reset_Enable(int enable);

void WDT_Init(int timeout)
{
	if(timeout)
	{
		rWTDAT = 2014*timeout;
		rWTCNT = 2014*timeout;
	}
	else
	{
		rWTDAT = 2014*10; // default 10 sec
		rWTCNT = 2014*10; // default 10 sec
	}
	// interrupt Disable, WDT disable, no reset mode
	// prescler[15:8] = (0~255)  , MUX[4:3] = (16, 32, 64, 128)
	// WDT 주파수 설정 (현재 PCLK는 66MHz)
	// 1클럭 기준 목표주파수 = PCLK/(분주비)
	// 이때, 분주비의 최대값이 256*128 = 32768이므로 
	// 최소 timer time tick은 32768/66000000초 이하 시간이다
	// 즉, 약 496usec 정도가 된다
	// 최대 값은 496 * 65536usec = 약 33초 이다
	
	// WDT : 500usec(0.5msec) unit clock으로 생성 = (1/500)MHz = 2KHz)
	// 이 경우 16bit timer이므로 0.5msec ~ 65536*0.5(= 32768msec) 가능 (약 32초)
	
	// 목표치 500usec => 2000hz = 49391600 / (N) 이 성립된다
	// 따라서 N = (49391600/2000) = 24695.8
	// 24695.8은 Prescler 최대값 256를 초과함
	// 따라서 뒷단에서 MUX로 재차 분주를 하여 사용하여야 함
	// 24695.8을 MUX가 16,32,64,128 분주가 가능하나 256을 초과하지 않는 값은 128뿐이다
	// 따라서 MUX는 1/128, PRESCLER는 int(24695.8/128) - 1이므로 192이다
	
	// 최종 결정된 정확한 tick 주파수는 66000000/(256*128) = 2014Hz (약 2000Hz)
	// 따라서 tick time은 약 0.5msec이다 	

// WDT 클럭 주파수 : 2,023HZ
	rWTCON = (255 <<8) + (0<<7) + (0<<5) + (3 <<3) + (0<<2) + (0<<1) + (1<<0);
}

void WDT_Start(int start)
{
	start? (Macro_Set_Bit(rWTCON ,5)) : (Macro_Clear_Bit(rWTCON ,5));
}

void WDT_Clear(unsigned int msec)
{
	//unit은 0.5msec 이므로 
	rWTCNT = msec * 2;
	rWTDAT = msec * 2;
}

void WDT_Clear2(void)
{
	//unit은 0.5msec 이므로 
	rWTCNT = 2014*1.02;
}

// system lockup code
void WDT_Test(void){ while(1) ; }


// WDT_Reset_On_Off
void WDT_Reset_Enable(int enable)
{
	enable? (Macro_Set_Bit(rWTCON ,0)) : (Macro_Clear_Bit(rWTCON ,0));
}
