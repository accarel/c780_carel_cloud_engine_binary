/*
 * utilities.h
 *
 *  Created on: Sep 9, 2019
 *      Author: ataayoub
 */

#ifndef MAIN_UTILITIES_H_
#define MAIN_UTILITIES_H_

#include "common.h"


/*****************
* Cloud to GME
*****************/
#pragma pack(1)
typedef struct req_set_gw_config_s{
	char hostname[20];
	uint32_t valuesPeriod;
	uint32_t statusPeriod;
	uint32_t mqttKeepAliveInterval;
	uint32_t lowspeedsamplevalue;
	uint32_t hispeedsamplevalue;
}req_set_gw_config_t;
#pragma pack()



size_t getFilesize(const char* filename);
void Utilities__CalcMACAddr(void);
char* Utilities__GetMACAddr(void);
char* Utilities__GetIMEICode(void);
void Utilities__Init(void);
req_set_gw_config_t* Utilities__GetGWConfigData(void);


#endif /* MAIN_UTILITIES_H_ */
