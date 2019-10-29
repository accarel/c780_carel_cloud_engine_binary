/*
 * sys.h
 *
 *  Created on: Jun 14, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_SYS_H_
#define MAIN_SYS_H_

//#include "state_machine.h"
#include "common.h"

esp_err_t Sys__Init (void);
gme_sm_t Sys__Config (config_sm_t sm);
config_sm_t Sys_GetConfigSM(void);
void Sys_SetConfigSM(config_sm_t config_state);
bool Sys__ResetCheck(void);
bool Sys__FirmwareFactoryReset(void);

void Sys__CertAlloc(void);
char* Sys__GetCert(uint8_t cert_num);
esp_err_t Sys__SetFactoryBootPartition(void);

#endif /* MAIN_SYS_H_ */
