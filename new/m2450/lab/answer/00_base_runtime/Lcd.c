/*
 * =====================================================================
 * NAME         : Lcd.c
 *
 * Descriptions : Main routine for S3C2450
 *
 * IDE          : GCC-4.1.0
 *
 * Modification
 *	  
 * =====================================================================
 */

#include "2450addr.h"
#include "my_lib.h"
#include "option.h"

#define LCD_SIZE_X (480)
#define LCD_SIZE_Y (272)
#define CLKVAL  	13
#define HOZVAL 	   (LCD_SIZE_X-1)
#define LINEVAL    (LCD_SIZE_Y-1)

#define VBPD (10)	
#define VFPD (1)	
#define VSPW (1)	
#define HBPD (43)	
#define HFPD (1)	
#define HSPW (1)	

#define  NonPal_Fb   ((volatile unsigned short(*)[480]) FRAME_BUFFER)
#define  Fb   ((volatile unsigned long(*)[80]) FRAME_BUFFER)
//unsigned short int (* NonPal_Fb)[480]; 
//unsigned long (* Fb)[80];


//Function Declaration
void Lcd_Port_Init(void);
void NonPal_Lcd_Init(void);
void NonPal_Put_Pixel(int x, int y, int color);
void Lcd_Draw_BMP(int x, int y, const unsigned char *fp);

static unsigned short bfType;
static unsigned int bfSize;
static unsigned int bfOffbits;
static unsigned int biWidth, biWidth2;
static unsigned int biHeight;

void Lcd_Port_Init(void)
{
	
	rGPCUDP = 0xffffffff; 
	rGPCCON &= ~(0xffffffff);
	rGPCCON |= 0xaaaa02aa; //GPC5,6,7 = output
	
	rGPDUDP = 0xffffffff; 
	rGPDCON &= ~(0xffffffff);
	rGPDCON |= 0xaaaaaaaa;
	
	/* GPG12 is setted as LCD_Display ON/OFF */
	rGPGCON=rGPGCON&(~(3<<24))|(1<<24); //GPG12=OUTPUT
	rGPGDAT|=(1<<12);
	
	/* GPB0 backlight Enable */
	rGPBCON=rGPBCON&(~(3<<0))|(1<<0);
	rGPBDAT|= (1<<0);
	
	
}

void NonPal_Lcd_Init()
{		
	rVIDW00ADD0B0 = FRAME_BUFFER;
	rVIDW00ADD1B0 = 0;
	rVIDW00ADD2B0 = (0<<13)|((LCD_SIZE_X*4*2)&0x1fff);

	rVIDW00ADD1B0 = 0+(LCD_SIZE_X*LCD_SIZE_Y);
	
	/* TO DO : setting for LCD control 
	* RGB I/F,PNRMODE(BGR),VCLK=9MHz,VCLK=Enable,CLKSEL=HCLK,CLKDIR=Divided,ENVID=disable
	*/
	rVIDCON0=(0x0<<22)+(0x1<<13)+((CLKVAL)<<6)+(1<<5)+(1<<4)+(0<<2);
	rVIDCON1= (1<<6)+(1<<5);
	
	rVIDTCON0=((VBPD)<<16)+((VFPD)<<8)+(VSPW);
	rVIDTCON1=((HBPD)<<16)+((HFPD)<<8)+(HSPW);
	rVIDTCON2=(LINEVAL<<11)+(HOZVAL);
	
	rVIDOSD0A		= 	(((0)&0x7FF)<<11) | (((0)&0x7FF)<<0);
	rVIDOSD0B	 	= 	(((LCD_SIZE_X-1)&0x7FF)<<11) | (((LCD_SIZE_Y-1)&0x7FF)<<0);

	rVIDOSD1A 		= 	(((0)&0x7FF)<<11) | (((0)&0x7FF)<<0);
	rVIDOSD1B	 	= 	(((LCD_SIZE_X-1)&0x7FF)<<11) | (((LCD_SIZE_Y-1)&0x7FF)<<0);
	/* TO DO 
	* Half swap Enable, 5:5:5:I format, Window0 ON
	*/
	rWINCON0=(0x1<<16)+(0x7<<2)+(1<<0); 
	
	/* TO DO : ENVID Enable for video output and LCD control signal */
	rVIDCON0 |= (3<<0);	
}

void NonPal_Put_Pixel(int x, int y, int color)
{
	/* TO DO : Fill FRAMEBUFFER with color value  */
	NonPal_Fb[y][x] = (unsigned short int)color;	
	
}

void Lcd_Draw_BMP(int x, int y, const unsigned char *fp)
{
     int xx=0, yy=0;	
     unsigned int tmp;
     unsigned char tmpR, tmpG, tmpB;
	
     bfType=*(unsigned short *)(fp+0);
     bfSize=*(unsigned short *)(fp+2);
     tmp=*(unsigned short *)(fp+4);
     bfSize=(tmp<<16)+bfSize;
     bfOffbits=*(unsigned short *)(fp+10);
     biWidth=*(unsigned short *)(fp+18);    
     biHeight=*(unsigned short *)(fp+22);    
     biWidth2=(bfSize-bfOffbits)/biHeight;	
     for(yy=0;yy<biHeight;yy++)
     {
         for(xx=0;xx<biWidth;xx++)
         {
             tmpB=*(unsigned char *)(fp+bfOffbits+(biHeight-yy-1)*biWidth*3+xx*3+0);
             tmpG=*(unsigned char *)(fp+bfOffbits+(biHeight-yy-1)*biWidth*3+xx*3+1);
             tmpR=*(unsigned char *)(fp+bfOffbits+(biHeight-yy-1)*biWidth*3+xx*3+2);
             tmpR>>=3;
             tmpG>>=3;
             tmpB>>=3;
             
             if(xx<biWidth2) NonPal_Put_Pixel(x+xx,y+yy,(tmpR<<10)+(tmpG<<5)+(tmpB<<0));
         } 
     }
    
}