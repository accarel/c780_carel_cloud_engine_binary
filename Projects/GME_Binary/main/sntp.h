/*
 * sntp_example_main.h
 *
 *  Created on: 17 lug 2019
 *      Author: ITAdmin
 */

#ifndef __SNTP_EXAMPLE_H__
#define __SNTP_EXAMPLE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* INCLUDE */

#include "stdint.h"
#include <string.h>

/* DEFINE */

/* STRUCT */

/* PROTOTYPES */
//void Init_RTC(void);
//uint32_t Get_UTC_Current_Time(void);
/* SNTP operating modes: default is to poll using unicast.
   The mode has to be set before calling sntp_init(). */
#define SNTP_OPMODE_POLL            0
#define SNTP_OPMODE_LISTENONLY      1

void sntp_get_time(char * buffer, size_t size);
void sntp_stop(void);
void sntp_setoperatingmode(uint8_t operating_mode);
void sntp_setservername(uint8_t idx, const char *server);
void sntp_init(void);



#ifdef __cplusplus
}
#endif

#endif /* __SNTP_EXAMPLE_H__ */
