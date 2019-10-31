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
void Init_RTC(void);
void sntp_get_time(char * buffer, size_t size);
uint32_t Get_UTC_Current_Time(void);
#ifdef __cplusplus
}
#endif

#endif /* __SNTP_EXAMPLE_H__ */
