/*=========================*/
#include "my_lib.h"
#include "option.h"
#include "2450addr.h"
#include "./images/bicycle.h"

extern unsigned int HandleTIMER0;

void Led_Port_Init(void);
void Led_Display(int data);
void Timer_Init(void);
void Timer_Delay(int msec);
void BuzzerTest(void) ;
void Display_Rtc(void);
void WDT_Init(int timeout);
void WDT_Start(int start);
void Rtc_TimeSet(void);

#define EXAMPLE 427
/*	
 * 420 : UART_TEST (실습 UART 테스트 프로그램)
 *
 * 421 : TIMER_TEST (실습 TIMER 테스트 프로그램)
 *	
 * 422 : TIMER_INTERRUPT_TEST (실습 TIMER 인터럽트 테스트 프로그램)
 *	
 * 423 : KEY_TEST (실습 KEY 테스트 프로그램)
 *	
 * 424 : KEY_INTERRUPT_TEST (실습 KEY 인터럽트 테스트 프로그램)
 *
 * 425 : BUZZER_TEST (실습 BUZZER 테스트 프로그램)
 *
 * 426 : WDT_TEST (실습 WDT 테스트 프로그램)
 *
 * 427 : RTC_TEST (실습 RTC 테스트 프로그램)
 *
 * Advanced Course
 * 1. make macro function 
 */

// ISR Declaration
 __attribute__((interrupt("IRQ"))) void  Timer0_ISR(void)
{
        /* TODO : Pending Register Clear */
        // rSRCPND1 = (0<<0);
        // rINTPND1 = (0<<0);
       rSRCPND1 = (1<<10);
       rINTPND1 = (1<<10);

        Uart_Send_String("Timer ISR\n");  
}

void Main(void)
{
	int i, j;

	MMU_Init();

	/* Testing LED with GPIO */
	Led_Port_Init();
	for(i=0;i<16;i++) {
		Led_Display(i);
		for(j=0;j<0x1ffff;j++);
	}
	
	/* Initialize UART */
	Uart_Init(115200);
	Uart_Send_String("\nHello ARM !!!\n\n");
 
 /***************************************
 * 
 * Title: UART_TEST
 * 
 ***************************************/
#if EXAMPLE == 420
	while(1)
	{
//		str = Uart_get_File();
	Uart_Send_String("Press any Key (Waiting for key input) : ");
	Uart_Printf("\nInput character is [%c]\n",Uart_GetChar());
	}
#endif // UART_TEST

 /***************************************
 * 
 * Title: TIMER_TEST
 * 
 ***************************************/
#if EXAMPLE == 421
 	int msec;

	Uart_Send_String("TIMER TEST !!!\n\n");
	Timer_Init();

	msec= 1000;
	while(1)
	{
		Uart_Printf("%d milisecond passed...\n", msec);
		Timer_Delay(msec);
	}	 
#endif // TIMER_TEST

/***************************************
 * 
 * Title: TIMER_INTERRUPT_TEST
 * 
 ***************************************/
#if EXAMPLE == 422
 	int msec;
 	
	Uart_Send_String("TIMER TEST !!!\n\n");
	Timer_Init();

       HandleTIMER0 = (unsigned int)Timer0_ISR; //timer time out되는시간 

        /* TODO : Interrupt Unmasking */
       rINTMSK1 &= ~(1<<10);

	msec= 1000;

	Timer_Delay(msec);

	while(1)
	{
		
	}	 
#endif // TIMER_INTERRUPT_TEST

 /***************************************
 * 
 * Title: KEY_TEST
 * 
 ***************************************/
#if EXAMPLE == 423
	Uart_Send_String("Press any key : waiting .....\n");
	Key_Port_Init();
	

	for(;;)
	{
		Uart_Printf("Key Value =%d\n",  Key_Wait_Get_Pressed()); 
		Key_Wait_Get_Released();
	}
#endif // KEY_TEST

 /***************************************
 * 
 * Title: KEY_INTERRUPT_TEST
 * 
 ***************************************/
#if EXAMPLE == 424
	/* External Interrupt Test */
	Uart_Send_String("Press any key : waiting key interrupt.....\n");	
	Key_INT_Init();
	for(;;); // endless loop
#endif // KEY_INTERRUPT_TEST
	
 /***************************************
 * 
 * Title: BUZZER_TEST
 * 
 ***************************************/
#if EXAMPLE == 425
	BuzzerTest();
	for(;;); // endless loop
#endif

 /***************************************
 * 
 * Title: WDT_TEST
 * 
 ***************************************/
#if EXAMPLE == 426
	WDT_Init(3); // limitation (1sec ~ 32sec), timer를 3초로 세팅, 3초후 리셋
	WDT_Start(1);
	
	for(;;){
		WDT_Clear2();  // endless loop(not reset)
	}
#endif

 /***************************************
 * 
 * Title: RTC_TEST
 * 
 ***************************************/
#if EXAMPLE == 427
	Rtc_TimeSet();
	Display_Rtc();
#endif

	while(1);
}
