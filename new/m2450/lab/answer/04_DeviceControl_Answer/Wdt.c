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
	// WDT ���ļ� ���� (���� PCLK�� 66MHz)
	// 1Ŭ�� ���� ��ǥ���ļ� = PCLK/(���ֺ�)
	// �̶�, ���ֺ��� �ִ밪�� 256*128 = 32768�̹Ƿ� 
	// �ּ� timer time tick�� 32768/66000000�� ���� �ð��̴�
	// ��, �� 496usec ������ �ȴ�
	// �ִ� ���� 496 * 65536usec = �� 33�� �̴�
	
	// WDT : 500usec(0.5msec) unit clock���� ���� = (1/500)MHz = 2KHz)
	// �� ��� 16bit timer�̹Ƿ� 0.5msec ~ 65536*0.5(= 32768msec) ���� (�� 32��)
	
	// ��ǥġ 500usec => 2000hz = 49391600 / (N) �� �����ȴ�
	// ���� N = (49391600/2000) = 24695.8
	// 24695.8�� Prescler �ִ밪 256�� �ʰ���
	// ���� �޴ܿ��� MUX�� ���� ���ָ� �Ͽ� ����Ͽ��� ��
	// 24695.8�� MUX�� 16,32,64,128 ���ְ� �����ϳ� 256�� �ʰ����� �ʴ� ���� 128���̴�
	// ���� MUX�� 1/128, PRESCLER�� int(24695.8/128) - 1�̹Ƿ� 192�̴�
	
	// ���� ������ ��Ȯ�� tick ���ļ��� 66000000/(256*128) = 2014Hz (�� 2000Hz)
	// ���� tick time�� �� 0.5msec�̴� 	

// WDT Ŭ�� ���ļ� : 2,023HZ
	rWTCON = (255 <<8) + (0<<7) + (0<<5) + (3 <<3) + (0<<2) + (0<<1) + (1<<0);
}

void WDT_Start(int start)
{
	start? (Macro_Set_Bit(rWTCON ,5)) : (Macro_Clear_Bit(rWTCON ,5));
}

void WDT_Clear(unsigned int msec)
{
	//unit�� 0.5msec �̹Ƿ� 
	rWTCNT = msec * 2;
	rWTDAT = msec * 2;
}

void WDT_Clear2(void)
{
	//unit�� 0.5msec �̹Ƿ� 
	rWTCNT = 2014*1.02;
}

// system lockup code
void WDT_Test(void){ while(1) ; }


// WDT_Reset_On_Off
void WDT_Reset_Enable(int enable)
{
	enable? (Macro_Set_Bit(rWTCON ,0)) : (Macro_Clear_Bit(rWTCON ,0));
}
