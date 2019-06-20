/*
 * =====================================================================
 * NAME: 
 *       rtc.c
 *
 * Descriptions : 
 *       RTC Test Driver
 *
 * Developing Environment : 
 *       CodeWarrior 2.0 or Later
 *
 * =====================================================================
 */

#include "2450addr.h"
#include "option.h"
#include "my_lib.h"

#define TESTYEAR    (0x01)
#define TESTMONTH   (0x12)
#define TESTDATE    (0x31)
#define TESTDAY     (0x02)      //SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7   
#define TESTHOUR    (0x23)
#define TESTMIN     (0x59)
#define TESTSEC     (0x59)

#define TESTYEAR2   (0x02)
#define TESTMONTH2  (0x01)
#define TESTDATE2   (0x01)
#define TESTDAY2    (0x03)      //SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7   
#define TESTHOUR2   (0x00)
#define TESTMIN2    (0x00)
#define TESTSEC2    (0x00)

char Uart_Get_Pressed(void);

char *day[8] = {" ","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

	/* 
	 * void Rtc_Init(void)
	 * -----------------------------------------------------------------
	 */
void Rtc_Init(void)
{
	/* No reset, Merge BCD counters, 1/32768, RTC Control enable */
	rRTCCON  = (rRTCCON  & ~(0xf))  | 0x1;            
    
	rBCDYEAR = (rBCDYEAR & ~(0xff)) | TESTYEAR;
	rBCDMON  = (rBCDMON  & ~(0x1f)) | TESTMONTH;
	rBCDDATE = (rBCDDATE & ~(0x3f)) | TESTDATE;         
	rBCDDAY  =(rBCDDAY  & ~(0x7)) | TESTDAY;       //SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
	rBCDHOUR = (rBCDHOUR & ~(0x3f)) | TESTHOUR;
	rBCDMIN  = (rBCDMIN  & ~(0x7f)) | TESTMIN;
	rBCDSEC  = (rBCDSEC  & ~(0x7f)) | TESTSEC;
    
	/* No reset, Merge BCD counters, 1/32768, RTC Control disable */
	rRTCCON  = 0x0;
}

	/* 
	 * void Rtc_TimeSet(void)
	 * -----------------------------------------------------------------
	 */
void Rtc_TimeSet(void)
{
	int syear,smonth,sdate,shour,smin,ssec;
	int d_syear,d_smonth,d_sdate,d_shour,d_smin,d_ssec;
	int sday;

	Uart_Printf("[ RTC Time Setting ]\n");
	Rtc_Init();         //RTC Initialize
	Uart_Printf("RTC Time Initialized ...\n");
    
	Uart_Printf("Year (Two digit the latest) : ");
	d_syear = Uart_GetIntNum();
	Uart_Printf("d_syear= %d\n", d_syear);
	syear = (((d_syear/10) % 10) << 4) | (d_syear %10);
    
	Uart_Printf("Month                       : ");
	d_smonth = Uart_GetIntNum();
	smonth = ((((d_smonth/10) % 10)&0x1) << 4) | (d_smonth %10);
    
	Uart_Printf("Date                        : ");
	d_sdate = Uart_GetIntNum();
	/* YOUR CODE HERE */
	sdate= ((((d_sdate/10) % 10)&0x3) << 4) | (d_sdate %10);
          
	Uart_Printf("\n1:Sunday  2:Monday  3:Thesday  4:Wednesday  5:Thursday  6:Friday  7:Saturday\n");
	Uart_Printf("Day of the week             : ");
	sday = Uart_GetIntNum();
    
	Uart_Printf("Hour                        : ");
	d_shour = Uart_GetIntNum();
	shour = ((((d_shour/10) % 10)&0x3) << 4) | (d_shour %10);
    
	Uart_Printf("Minute                      : ");
	d_smin = Uart_GetIntNum();
	/* YOUR CODE HERE */
	smin = ((((d_smin/10) % 10)&0x7) << 4) | (d_smin %10);
    
	Uart_Printf("Second                      : ");
	d_ssec = Uart_GetIntNum();
	ssec = ((((d_ssec/10) % 10)&0x7) << 4) | (d_ssec %10);
    
	/* No reset, Merge BCD counters, 1/32768, RTC Control enable */
	rRTCCON  = (rRTCCON  & ~(0xf))  | 0x1;
    
	rBCDYEAR = (rBCDYEAR & ~(0xff)) | syear;
	rBCDMON  = (rBCDMON  & ~(0x1f)) | smonth;
	rBCDDAY  = (rBCDDAY  & ~(0x7)) | sday;           //SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
	rBCDDATE = (rBCDDATE & ~(0x3f)) | sdate;
	rBCDHOUR = (rBCDHOUR & ~(0x3f)) | shour;
	rBCDMIN  = (rBCDMIN  & ~(0x7f)) | smin;
	rBCDSEC  = (rBCDSEC  & ~(0x7f)) | ssec;
    
	/* No reset, Merge BCD counters, 1/32768, RTC Control disable */
	rRTCCON  = 0x0;
}

	/* 
	 * void Display_Rtc(void)
	 * -----------------------------------------------------------------
	 */
void Display_Rtc(void)
{
	int tmp;                   
	int year,month,date,weekday,hour,min,sec;
	int d_year,d_month,d_date,d_hour,d_min,d_sec;

	Uart_Printf("[ Display RTC Test ]\n");

	/* No reset, Merge BCD counters, 1/32768, RTC Control enable */
	rRTCCON = 0x01;
	Uart_Printf("Press any key to exit.\n\n");
	
	while(!Uart_Get_Pressed())
	{    
		while(1)
		{
			if(rBCDYEAR==0x99)
				year = 0x1999;
			else 
				year    = 0x2000 + rBCDYEAR;
			/* YOUR CODE HERE */
				month = rBCDMON;
				weekday = rBCDDAY;
				date = rBCDDATE;
				hour = rBCDHOUR;
				min = rBCDMIN;
				sec = rBCDSEC;
            
			if(sec!=tmp)                //Same time is not display
			{
				tmp = sec;
				break;
			}         
		}

		/* Change BCD data to decimal */
		d_year	= (year & 0xf) + 10 * ((year >> 4) & 0xf) + 
					100 * ((year >> 8) & 0xf) + 1000 * ((year >> 12) & 0xf) ;
		d_month	= (month & 0xf) + 10 * ((month >> 4) & 0x1) ;
		d_date 	= (date & 0xf) + 10 * ((date >> 4) & 0x3) ;
		/* YOUR CODE HERE */
		d_hour = (hour & 0xf) + 10 * ((hour >> 4) & 0x3) ;
		d_min = (min & 0xf) + 10 * ((min>> 4) & 0x7) ;
		d_sec = (sec & 0xf) + 10 * ((sec >> 4) & 0x7) ;

	

		Uart_Printf("%2d:%2d:%2d  %10s,  %2d/%2d/%4d\n",
			d_hour, d_min, d_sec, day[weekday], d_month, d_date, d_year);
	}

	/* No reset, Merge BCD counters, 1/32768, RTC Control disable(for power consumption) */
	rRTCCON = 0x0; 
}
