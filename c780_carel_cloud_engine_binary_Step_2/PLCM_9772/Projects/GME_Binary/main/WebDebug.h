/**
 * @file   WebDebug.h
 * @author carel
 * @date   22 Oct 2020
 * @brief   
 *
 */

/* Define to prevent recursive inclusion */
#ifndef _WEBDEBUG_H
#define _WEBDEBUG_H

/* ==== Include ==== */
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

/* ==== Define fixed don't touch it ==== */


/* ==== Define customizable ==== */


/* the monitored FW zones */
#define WEBDBG_MAIN_DEVICE  0
#define WEBDBG_MODBUS_RTU   1
#define WEBDBG_POLLING      2
#define WEBDBG_WIFI         3
#define WEBDBG_MQTT			4
#define WEBDBG_OTA_STATUS   5
#define WEBDBG_OTA_CONLEN   6
#define WEBDBG_OTA_TRASLEN  7
#define MAX_WEBDBG  	    8

/* ==== Global Variables ==== */

typedef struct debug_data_s{
	 C_BYTE dbgMain;
	 C_UINT32 dbgRtu;
	 C_BYTE dbgPolling;
	 C_BYTE dbgWifi;
	 C_BYTE dbgMqtt;
	 C_BYTE dbgOtaStatus;
	 C_UINT32 dbgOtaConlen;
	 C_UINT32 dbgOtaTrasflen;
}debug_data_t;

/* ==== Function prototype ==== */
void RetriveDataDebug(C_INT16 type, C_INT32 val);

C_CHAR * ReturnDataDebugBuffer(void);
C_CHAR * ReturnStaticDataDebugBuffer(void);

#endif //_WEBDEBUG
