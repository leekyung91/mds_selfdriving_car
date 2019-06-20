//=============================================================================
// File Name : my_lib.h
//=============================================================================

// Key.c
__attribute__ ((interrupt ("IRQ"))) void Key_ISR(void);

// UART.c
extern char Uart_GetKey(void);
extern char Uart_Getch(void);
extern void Uart_Init(int baud);
extern void Uart_Printf(char *fmt,...);
extern void Uart_Send_Byte(int data);
extern void Uart_Send_String(char *pt);

// Lcd.c
extern void Graphic_Init(void);
extern void Lcd_Put_Pixel(int x, int y, int c);
extern void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp);

//Libraries on the libc.c
void MMU_Init(void);
void ChangeRomCacheStatus(int attr);
void MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr);

void *malloc(unsigned nbyte); 
void free(void *pt);

int  Uart_GetIntNum(void);
void Uart_Printf(char *fmt,...);

void MemFill(unsigned long ptr, unsigned long pattern, int size);
void MemDump(unsigned long ptr, int size);

// Libraries on the libs.s

int SET_IF(void);
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
