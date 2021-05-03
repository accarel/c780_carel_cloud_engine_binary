/*
 * utilities.h
 *
 *  Created on: Sep 9, 2019
 *      Author: ataayoub
 */

#ifndef MAIN_UTILITIES_H_
#define MAIN_UTILITIES_H_

#include "CAREL_GLOBAL_DEF.h"
#include "common.h"
#include "MQTT_Interface_CAREL.h"

/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

//this define enable the output of the communications errors
//#define __DEBUG_UTILITIES_CAREL_LEV_1

//this define enable the output of others debug informations
//#define __DEBUG_UTILITIES_CAREL_LEV_2

#endif




size_t getFilesize(const char* filename);
void Utilities__CalcMACAddr(void);
char* Utilities__GetMACAddr(void);
void Utilities__Init(void);
req_set_gw_config_t* Utilities__GetGWConfigData(void);
C_UINT32 Utilities__GetStatusPeriod(void);
char* Utilities__GetPN(void);


#endif /* MAIN_UTILITIES_H_ */
