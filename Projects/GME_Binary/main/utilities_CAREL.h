/*
 * utilities.h
 *
 *  Created on: Sep 9, 2019
 *      Author: ataayoub
 */

#ifndef MAIN_UTILITIES_H_
#define MAIN_UTILITIES_H_

#include "common.h"
#include "MQTT_Interface_CAREL.h"

size_t getFilesize(const char* filename);
void Utilities__CalcMACAddr(void);
char* Utilities__GetMACAddr(void);
char* Utilities__GetIMEICode(void);
void Utilities__Init(void);
req_set_gw_config_t* Utilities__GetGWConfigData(void);


#endif /* MAIN_UTILITIES_H_ */
