#include "2450addr.h"
#include "option.h"
//#include "macro.h"

// for GPH10
void Buzzer_Init(void);
void Buzzer_Beep(int tone, int duration);
/* Buzzer Beep with Timer4 */
void Beep(int tone, int duration);
void DelayForPlay2(unsigned short time);

#define BASE10	10000

#define TONE_BEEP		1000
#define DURATION_5SEC	5000
#define DURATION_1SEC	1000

#define C1      523     // Do
#define C1_     554
#define D1      587     // Re
#define D1_     622
#define E1      659     // Mi
#define F1      699     // Pa
#define F1_     740
#define G1      784     // Sol
#define G1_     831
#define A1      880     // La
#define A1_     932
#define B1      988     // Si
#define C2      C1*2    // Do
#define C2_     C1_*2
#define D2      D1*2    // Re
#define D2_     D1_*2
#define E2      E1*2    // Mi
#define F2      F1*2    // Pa
#define F2_     F1_*2
#define G2      G1*2    // Sol
#define G2_     G1_*2
#define A2      A1*2    // La
#define A2_     A1_*2
#define B2      B1*2    // Si

void Buzzer_Init(void)
{
	// Buzzer = GPB1
	rGPBDAT |= (0x1<<1);
	rGPBCON &= ~(0x3 << 2);
	rGPBCON |= 0x1<<2;
}

void Buzzer_Beep(int tone, int duration)
{
	unsigned int temp;

	for( ;(unsigned)duration > 0; duration--)
	{
		rGPBDAT &= ~(0x1<<1);
		for(temp = 0 ; temp < (unsigned)tone; temp++);
		rGPBDAT |= (0x1<<1);
		for(temp = 0 ; temp < (unsigned)tone; temp++);
	}
}

void Beep(int tone, int duration)
{
	rTCFG0 = (0xff<<8)|(0); 
	rTCFG1 = (0<<20)|(3<<16); 
	
	/* TCON¼³Á¤ :Dead zone disable,  auto reload on, output inverter off
	 * manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
	 */
	rTCNTB4 = 16.113*duration;
	rTCON &=~  (1<<22);
	rTCON |=  (1<<21);
	rTCON &= ~(1<<21);
	rTCON |=  (1<<20);

	while(rTCNTO4 !=0) 
	{
		rGPBDAT &= ~(0x1<<1);
		DelayForPlay2(BASE10/tone);
		rGPBDAT |= (0x1<<1);
		DelayForPlay2(BASE10/tone);
	}
	rTCON &= ~(1<<20);
}

void DelayForPlay2(unsigned short time)	// resolution=0.1ms
{
	/* Prescaler value : 39  */
	/* Clock Select    : 128 */
	rWTCON=(37<<8)|(3<<3);			// resolution=0.1ms
	rWTDAT=time+10;					// Using WDT
	rWTCNT=time+10;
	rWTCON|=(1<<5);

	while(rWTCNT>10);
	rWTCON = 0;
}

void BuzzerTest(void) 
{
	Buzzer_Init();
//	Beep(TONE_BEEP, DURATION_5SEC);
	Beep(C1, DURATION_1SEC);
	Beep(D1, DURATION_1SEC);
	Beep(E1, DURATION_1SEC);
	Beep(F1, DURATION_1SEC);
	Beep(G1, DURATION_1SEC);
	Beep(A1, DURATION_1SEC);
	Beep(B1, DURATION_1SEC);
	Beep(C2, DURATION_1SEC);
}
