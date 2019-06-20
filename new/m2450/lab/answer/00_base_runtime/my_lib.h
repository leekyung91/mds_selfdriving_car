#ifndef _MY_LIB_H_
#define _MY_LIB_H_

//Uart.c 
void Uart_Init(int baud);
void Uart_Printf(char *fmt,...);
void Uart_Send_String(char *pt);
void Uart_Send_Byte(int data);
char Uart_Get_Char(void);
char Uart_Get_Pressed(void);

//Exception.c
void Exception_Init(void);

// Led.c
void Led_Init(void);
void Led_Display(int value);

// Buzzer.c
void Buzzer_Init(void);
void Buzzer_Beep(int tone, int duration);

// Timer
void Timer0_Init(void);
void Timer0_Delay(int msec);

// Touch.c
void Touch_Isr_Init(void (*fp)(void));
void Touch_ISR_Enable(int enable);

// Lcd.c
void Graphic_Init(void);
void Lcd_Wait_Blank(void);
void Lcd_Copy(unsigned from, unsigned int to);
void Lcd_Select_Frame_Buffer(unsigned int id);
void Lcd_Display_Frame_Buffer(unsigned int id);
void Lcd_Set_Tran_Mode(int mode);
void Lcd_Put_Pixel(int x,int y,int c);
void Lcd_Clr_Screen(unsigned long color);
void Lcd_Hline(int y, int x1, int x2, int color);
void Lcd_Vline(int x, int y1, int y2, int color);
void Lcd_Line(int x1,int y1,int x2,int y2,int color);
void Lcd_Rectangular(int x1, int y1, int x2, int y2, int color);
void Lcd_Fill_Rectangular(int x1, int y1, int x2, int y2, int color);
void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...);
void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned char *fp);
void Lcd_Draw_BMP(int x, int y, const unsigned char *fp);

// MMU.c
void MMU_Init(void);
void MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr);

int  SET_IF(void);
void WR_IF(int cpsrValue);
void CLR_IF(void);

void MMU_EnableICache(void);
void MMU_DisableICache(void);
void MMU_EnableDCache(void);
void MMU_DisableDCache(void);
void MMU_EnableAlignFault(void);
void MMU_DisableAlignFault(void);
void MMU_EnableMMU(void);
void MMU_DisableMMU(void);
void MMU_SetTTBase(unsigned long base);
void MMU_SetDomain(unsigned long domain);

void MMU_SetFastBusMode(void);          //GCLK=HCLK
void MMU_SetAsyncBusMode(void);         //GCLK=FCLK @(FCLK>=HCLK)

void MMU_InvalidateIDCache(void);
void MMU_InvalidateICache(void);
void MMU_InvalidateICacheMVA(unsigned long mva);
void MMU_PrefetchICacheMVA(unsigned long mva);
void MMU_InvalidateDCache(void);
void MMU_InvalidateDCacheMVA(unsigned long mva);
void MMU_CleanDCacheMVA(unsigned long mva);
void MMU_CleanInvalidateDCacheMVA(unsigned long mva);
void MMU_CleanDCacheIndex(unsigned long index);
void MMU_CleanInvalidateDCacheIndex(unsigned long index); 
void MMU_WaitForInterrupt(void);
        
void MMU_InvalidateTLB(void);
void MMU_InvalidateITLB(void);
void MMU_InvalidateITLBMVA(unsigned long mva);
void MMU_InvalidateDTLB(void);
void MMU_InvalidateDTLBMVA(unsigned long mva);

void MMU_SetDCacheLockdownBase(unsigned long base);
void MMU_SetICacheLockdownBase(unsigned long base);

void MMU_SetDTLBLockdown(unsigned long baseVictim);
void MMU_SetITLBLockdown(unsigned long baseVictim);

void MMU_SetProcessId(unsigned long pid);

#endif
