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

#endif
