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

/* ==== Define fixed don't touch it ==== */


/* ==== Define customizable ==== */


/* the monitored FW zones */
#define WEBDBG_MAIN_DEVICE  0
#define WEBDBG_MODBUS_RTU   1
#define WEBDBG_POLLING      2
#define WEBDBG_WIFI         3
#define WEBDBG_MQTT			4
#define MAX_WEBDBG  	    5

/* ==== Global Variables ==== */

typedef struct debug_data_s{
	 uint8_t dbgMain;
	 uint32_t dbgRtu;
	 uint8_t dbgPolling;
	 uint8_t dbgWifi;
	 uint8_t dbgMqtt;
}debug_data_t;

/* ==== Function prototype ==== */
void RetriveDataDebug(C_INT16 type, C_INT32 val);

C_CHAR * ReturnDataDebugBuffer(void);
C_CHAR * ReturnStaticDataDebugBuffer(void);

#endif //_WEBDEBUG
