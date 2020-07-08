/**
 * @file   radio.h
 * @author carel
 * @date   9 Sep 2019
 * @brief  high level functions related to the radio management platform independent
 *
 */
#ifndef MAIN_RADIO_H_
#define MAIN_RADIO_H_

#include "gme_types.h"

typedef enum{
	DISCONNECTED = 0,
	CONNECTED,
}connection_status_t;

gme_sm_t Radio__Config(void);
void Radio__WaitConnection(void);
connection_status_t Radio__GetStatus(void);
int8_t Radio__GetRSSI(void);

#endif
