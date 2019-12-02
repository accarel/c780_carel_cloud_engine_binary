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
gme_sm_t Sys__Config (config_sm_t sm);
config_sm_t Sys_GetConfigSM(void);
void Sys_SetConfigSM(config_sm_t config_state);

void Sys__CertAlloc(void);

char* Sys__GetCert(uint8_t cert_num);

#endif /*SYS_CAREL_H_ */
