/*
 * mobile.h
 *
 *  Created on: Jan 24, 2019
 *      Author: cellini
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_MOBILE_H_
#define MAIN_MOBILE_H_

#include "common.h"
#include "data_types_CAREL.h"
#include "wifi.h"

gme_sm_t Mobile__Config();
void Mobile__WaitConnection(void);
void Mobile__SetStatus(connection_status_t status);
connection_status_t Mobile__GetStatus(void);
void Mobile__SaveImeiCode(char* buf);
char* Mobile__GetImeiCode(void);
void Mobile__SetConnTime(void);
C_TIME Mobile__GetConnTime(void);

#endif
