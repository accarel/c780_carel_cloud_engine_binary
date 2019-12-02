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

C_RES Sys__SetFactoryBootPartition(void);
C_BOOL Sys__ResetCheck(void);
C_BOOL Sys__FirmwareFactoryReset(void);
C_UINT32 Sys__GetFreeHeapSize(void);

#endif /*SYS_IS_H_ */
