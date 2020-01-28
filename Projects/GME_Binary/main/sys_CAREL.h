/**
 * @file   sys_CAREL.h
 * @author carel
 * @date 20 Nov 2019
 * @brief ... TODO
 *        
 */ 

#ifndef SYS_CAREL_H_
#define SYS_CAREL_H_

//#include "state_machine.h"
#include "data_types_CAREL.h" 
#include "common.h"

C_RES Sys__Init (void);

void Sys__CertAlloc(void);

char* Sys__GetCert(uint8_t cert_num);

#endif /*SYS_CAREL_H_ */
