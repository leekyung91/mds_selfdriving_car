#include "2450addr.h"

void Touch_Init(void)
{
	rADCDLY = (50000); 
     /* TO DO : prescaler enable, prescaler value=39, Analog no input, 
      * 		Normal operation mode, Disable start, No operation */
	rADCCON =(1<<14)|(39<<6)|(0<<3)|(0<<2)|(0<<1)|(0);  
	 
	 /* TO DO :  For Waiting Interrupt Mode rADCTSC=0xd3 */
	rADCTSC =(0<<8)|(1<<7)|(1<<6)|(1<<4)|(0<<3)|(0<<2)|(3);	
}


void Touch_Isr_Init(void (*fp)(void))
{
	rADCDLY=(50000);	
	// Enable Prescaler,Prescaler,AIN5/7 fix (MUX don't care),Normal,Disable read start,No operation
	rADCCON = (1<<14)+(39<<6)+(0<<3)+(0<<2)+(0<<1)+(0);	
	// Wait Down,YM:GND,YP:Hi-z,XM:Hi-z,XP:DIS,XP pullup En,Normal,Waiting for interrupt mode     
	rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);

	pISR_ADC = (unsigned int)fp;
}

void Touch_ISR_Enable(int enable)
{	
	enable? (rINTMSK1 &= (unsigned)(~(1<<31))) : (rINTMSK1 |= (unsigned)(1<<31));
	enable? (rINTSUBMSK &= (unsigned)(~(1<<9))) : (rINTSUBMSK |= (unsigned)(1<<9));	
}
