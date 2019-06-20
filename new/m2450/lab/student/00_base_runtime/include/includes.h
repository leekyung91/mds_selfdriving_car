#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

#include <locale.h> //uartdrv.c

#include "../bsp/typedef.h"
#include "../bsp/mC100Config.h"
//#include "../bsp/BoardConfig.h" //#include "../bsp/mC100Config.h"
#include "../bsp/C100RegAddr.h"
#include "../bsp/clkcon.h"
#include "../bsp/macro.h"
#include "../bsp/libc.h"

#include "../bsp/intc.h"
#include "../bsp/timer.h"
#include "../bsp/uartdrv.h"
#include "../bsp/lcddrv.h"
#include "../bsp/touch.h"

//#include "option.h"

//#include "COMMRTOS.H"
//#include "PC.h"

void UART0_IntHandler(void);


#endif /* __INCLUDES_H__ */
