/**
 * @file   sys_CAREL.h
 * @author carel
 * @date 20 Nov 2019
 * @brief ... TODO
 *        
 */ 

#ifndef SYS_IS_H_
#define SYS_IS_H_

#include "data_types_CAREL.h" 
#include "common.h"

#if MYSYS_DEBUG == ENABLED
	#define	PRINTF_DEBUG_SYS	printf
#else
	#define	PRINTF_DEBUG_SYS(...)
#endif

#define BUTTON_WAIT 			0
#define BUTTON_PRESSED			1
#define BUTTON_SLOWBLINK 		2
#define BUTTON_WAITRELEASE		3
#define BUTTON_FASTBLINK	 	4
#define	BUTTON_WAITFACTORY	 	5

void Sys__ResetCheck(void);
C_UINT32 Sys__GetFreeHeapSize(void);
void Sys__Delay(C_UINT32 delay);

#endif /*SYS_IS_H_ */
