#include <stdio.h>
#include "2450addr.h"
#include "my_lib.h"
#include "macro.h"

void MMU_Init(void);

// 아이콘 그림들의 지정
#include "./button/RUN_ER.H"
#include "./button/RUN_EP.H"

// 불러올 그래픽 화일을 지정
#include "./Images/myimg.h"

#include "./Images/S01.H"

// Color Define
// [D15-D11]RED, [D10-D6]GREEN, [D5-D1]BLUE
#define BLACK	0x0000
#define WHITE		0xfffe
#define BLUE		0x003e
#define GREEN	0x07c0
#define RED		0xf800
#define YELLOW	0xffc0

// ISR Functions declaration

#define LCD_XSIZE 		(480)	
#define LCD_YSIZE 		(272)

// Functions Declaration

void HW_Initial(void);
void Show_Welcome(char * msg);
char* Display_Rtc(void);
void Rtc_TimeSet(void);

// Global Variables Declaration

// 터치패드가 눌림을 알수 있는 값
volatile int Touch_pressed = 0;
// ADC 값
volatile int ADC_x=0, ADC_y=0;

// Calibration 정보 저장 값
volatile int Cal_x1=848;
volatile int Cal_y1=656;
volatile int Cal_x2=186;
volatile int Cal_y2=349; 

// 좌표 변환 값
volatile int Touch_x, Touch_y;
// Calibration 완료 값
volatile unsigned int Touch_config=1;

void Touch_ISR(void) __attribute__ ((interrupt ("IRQ")));

void Touch_ISR()
{
	// rINTSUBMSK |= (0x1<<9);
	// rINTMSK1 |= (0x1<<31);	
	
	/* TO DO: Pendng Clear on Touch */	
		rSUBSRCPND = (0x1<<9);
		rSRCPND1 = (0x1<<31);
		rINTPND1 = (0x1<<31);
	
	// bad 
	// rSUBSRCPND |= (0x1<<9);
	// rSRCPND1 |= (0x1<<31);
	// rINTPND1 |= (0x1<<31);
	

	// Touch UP
	if(rADCTSC&0x100)
	{
		rADCTSC&=0xff;
		Touch_pressed = 0;
	}
	// Touch Down
	else 
	{
		rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(0);
		// SADC_ylus Down,Don't care,Don't care,Don't care,Don't care,XP pullup Dis,Auto,No operation
		rADCCON|=0x1;
		while(rADCCON & 0x1);
		while(!(0x8000&rADCCON));
		ADC_x=(int)(0x3ff&rADCDAT0);
		ADC_y=(int)(0x3ff&rADCDAT1);
		// Touch calibration complete
		if(Touch_config)
		{
			Touch_y=(ADC_y-Cal_y1)*(LCD_YSIZE-10)/(Cal_y2-Cal_y1)+5; 
			//LCD_YSIZE : 480, +5는 경계선 까지 손이 못갈수도 있으니까 5픽셀 오차 허용 
			Touch_x=(ADC_x-Cal_x2)*(LCD_XSIZE-10)/(Cal_x1-Cal_x2)+5; //LCD_XSIZE : 272
			Touch_x=LCD_XSIZE-Touch_x;
			// 경계선 밖은 예외 처리
			if(Touch_x<0) Touch_x=0;
			if(Touch_x>=LCD_XSIZE) Touch_x=LCD_XSIZE-1;
			if(Touch_y<0) Touch_y=0;
			if(Touch_y>=LCD_YSIZE) Touch_y=LCD_YSIZE-1;
		}
		// before calibration		
		else
		{
			Touch_x = ADC_x;
			Touch_y = ADC_y;
		}

		rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3); 
		// SADC_ylus Up,Don't care,Don't care,Don't care,Don't care,XP pullup En,Normal,Waiting mode
		Touch_pressed = 1; 
	}

	// rINTSUBMSK &= ~(0x1<<9);
	// rINTMSK1 &= ~(0x1<<31);
}
// Lower Layer Functions

void HW_Initial(void)
{
	// 필요한 초기화 루틴들을 기입한다
	Exception_Init();
	Uart_Init(115200);	
//	Led_Init();
//	Buzzer_Init();

	Timer0_Init();
	Graphic_Init();
	
	Touch_Isr_Init(Touch_ISR);
	MMU_Init();
}

void Show_Welcome(char * msg)
{
	int a, j; 
	
	Uart_Printf("\n%s\n", msg);
	
	for(a=0;a<2;a++)
	{
		Led_Display(7);
		for(j=0; j<0x0ffff; j++);
		Led_Display(0);
		for(j=0; j<0x0ffff; j++);
	}
}

void main(void)
{
	int xtmp, ytmp;
	char* myPtr;
	
	char* buffer;
	int i=0;
	
	Uart_Init(115200);

	HW_Initial();

/* malloc test */
	myPtr= malloc(4096);
	*myPtr= 'W';
/* printf test */
	printf("c=%c\n", *myPtr);
	free(myPtr);
// rtc test
	Rtc_TimeSet();

	Show_Welcome("LCD & Touch Test");
//	buffer = Display_Rtc();

	Lcd_Select_Frame_Buffer(0);
	Lcd_Clr_Screen(BLUE);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, s01);
	Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), s01);  
//	Lcd_Printf(10,254, YELLOW, BLACK, 1,1,Display_Rtc());
//다음단계로 넘어가려면 터미널에서 아무키나 입력하세요

	Lcd_Select_Frame_Buffer(1);
	Lcd_Clr_Screen(BLACK);
	Lcd_Get_Info_BMP(&xtmp, &ytmp, MYIMG);
	Lcd_Draw_BMP(((LCD_XSIZE/2)-(xtmp/2)), ((LCD_YSIZE/2)-(ytmp/2)), MYIMG); 
//	Lcd_Printf(10,254, YELLOW, BLACK, 1,1,Display_Rtc());
	// lcd 화면 출력 
	//다음단계로 넘어가려면 터미널에서 아무키나 입력하세요

	Uart_Printf("\n다음단계로 넘어가려면 아무키나  누르시오!!\n");
	// console 출력

	do
	{
//		Lcd_Clr_Screen(BLACK);

		Lcd_Printf(10,254, YELLOW, BLACK, 1,1,Display_Rtc());

		Lcd_Wait_Blank();
//		buffer = Display_Rtc();
		Lcd_Display_Frame_Buffer(1);
		Lcd_Printf(10,254, YELLOW, BLACK, 1,1,Display_Rtc());
		Timer0_Delay(1000);
		
		if(i==0) i++;
		else i=0;
/*
		Lcd_Wait_Blank();
//		buffer = Display_Rtc();
		Lcd_Display_Frame_Buffer(1);
		Lcd_Printf(10,254, YELLOW, BLACK, 1,1,Display_Rtc());
		Timer0_Delay(1000);
		*/
	}while(!Uart_Get_Pressed());	
	
	Uart_Printf("\n터치2 Calibration\n");
	
	Touch_ISR_Enable(1);   //
    	Lcd_Clr_Screen(WHITE); // 전부 지우는 것 

#if 1 // 캘리브레이션 생략 
  
	Lcd_Hline(5,0,10-1,BLUE);
	Lcd_Vline(5,0,10-1,BLUE);
	Lcd_Put_Pixel(240,136,RED);
	Lcd_Put_Pixel(241,137,RED);
	Lcd_Put_Pixel(240,137,RED);
	Lcd_Put_Pixel(241,136,RED);


	Lcd_Printf(10,40, BLUE, WHITE, 1,1,"좌측상단에 보이는 '+' 표시를 누르세요");    
	Timer0_Delay(200);
	while(Touch_pressed == 0);	// 사용자가 손가락 가져다 댈때까지 기다린다. 
	Uart_Printf("\nADC_x:%d, ADC_y:%d", ADC_x, ADC_y); 
	Touch_pressed = 0;
	
	Cal_x1=ADC_x;  // 좌상단의 캘리브 값
	Cal_y1=ADC_y;  // 
	
	Lcd_Hline(5,0,10-1,WHITE);
	Lcd_Vline(5,0,10-1,WHITE);
    	Lcd_Clr_Screen(WHITE);	     

	Lcd_Hline(LCD_YSIZE-5,LCD_XSIZE-0,LCD_XSIZE-10+1,BLUE);
	Lcd_Vline(LCD_XSIZE-5,LCD_YSIZE-0,LCD_YSIZE-10+1,BLUE);
	Lcd_Printf(80,200, BLUE, WHITE, 1,1,"우측하단에 보이는 '+' 표시를 누르세요");
	Timer0_Delay(200);
	while(Touch_pressed == 0);
	Uart_Printf("\nADC_x:%d, ADC_y:%d", ADC_x, ADC_y); 
	Touch_pressed = 0;
	
	Cal_x2=ADC_x;  // 우하단의 캘리브 값
	Cal_y2=ADC_y;  //     
	
	Lcd_Hline(LCD_YSIZE-5,LCD_XSIZE-0,LCD_XSIZE-10+1,WHITE);
	Lcd_Vline(LCD_XSIZE-5,LCD_YSIZE-0,LCD_YSIZE-10+1,WHITE);
    	Lcd_Clr_Screen(WHITE);	     

       Touch_config=1;  // 캘리브레이션이 완료 

	Lcd_Printf(0,100,BLUE,WHITE,1,1,"터치패드를 눌러보세요 x,y 좌표정보를 표시해 줍니다");   
	Lcd_Printf(0,140,BLUE,WHITE,1,1,"[터미널 키보드을 아무키나 누르면 종료]");  
	
	Uart_Printf("\n터치패널을 누르면 위치가 표시됩니다.\n");
	Uart_Printf("키보드를 누르면 다음동작으로 넘어갑니다.\n");	 	
	
	do
	{
		if(Touch_pressed == 1)
		{
			Uart_Printf("\nX:%d, Y:%d", Touch_x, Touch_y); 
			Touch_pressed = 0;
		}
	}while(!Uart_Get_Pressed());	
	
	
	/* Touch-panel Test */
	Uart_Printf("\n터치패널 응용 실습\n");
	Uart_Printf("그림을 눌러봅시다\n");	
#endif

	Uart_Printf("\nCal_x1=%d\n", Cal_x1);	
	Uart_Printf("\nCal_x2=%d\n", Cal_x2);	
	Uart_Printf("\nCal_y1=%d\n", Cal_y1);	
	Uart_Printf("\nCal_y2=%d\n", Cal_y2);
	
	Lcd_Clr_Screen(BLACK);    
	Lcd_Get_Info_BMP(&xtmp, &ytmp, run_er);     

	Lcd_Printf(10,40, YELLOW, BLACK, 1,1,"아이콘을 눌러보세요");
	Lcd_Printf(10,200, RED, BLACK, 1,1,"저는 예쁜 강이경 입니다.찡긋 ~~IT 인재!");
	for(;;)
	{
		Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), run_er);    
	
		for(;;)
		{
			if(Touch_pressed == 1)
			{
				if((Touch_x >= (160 - (xtmp/2))) && (Touch_x <= (160 + (xtmp/2))) && (Touch_y >= (120 - (ytmp/2))) && (Touch_y <= (120 + (ytmp/2))))
				{
					Uart_Printf("\n Pressed"); 
					Lcd_Printf(80,120, BLACK, RED, 1,1," PUSH ");
					break;
				}
			}
		}
	
		Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), run_ep);    
	
		for(;;)
		{
			if(Touch_pressed == 0)
			{
				Uart_Printf("\n Released"); 
				Lcd_Printf(80,120, BLACK, BLACK, 1,1," PULL ");
				break;
			}
		}
	}	
}
