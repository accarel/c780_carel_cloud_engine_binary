/**
 * @file   WebDebug.c
 * @author carel
 * @date   22 Oct 2020
 * @brief  monitor the system via dbg webpage
 *
 */

/* decomment to enable output debug */
//#define __SWWDT_ENABLE_DEBUG_PRINT

#include "CAREL_GLOBAL_DEF.h"
#include "types.h"
#include "data_types_CAREL.h"
#include "utilities_CAREL.h"

#include "WebDebug.h"


#ifdef __CCL_DEBUG_MODE
static const char *TAG = "WEBDEBUG";
#endif


#define WEB_DEBUG_BUF_SIZE 200


C_CHAR response_debug[200];

static debug_data_t dbgData = {
		 .dbgMain = 0,
		 .dbgRtu  = 0,
		 .dbgPolling = 0,
		 .dbgWifi = 0,
		 .dbgMqtt = 0,
};


void RetriveDataDebug(C_INT16 type, C_INT32 val)
{
	switch(type)
	{
		default:
		case WEBDBG_MAIN_DEVICE:
			dbgData.dbgMain = val;
			break;

		case WEBDBG_MODBUS_RTU:
			dbgData.dbgRtu = val;
			break;

		case WEBDBG_POLLING:
			dbgData.dbgPolling = val;
			break;

		case WEBDBG_WIFI:
			dbgData.dbgWifi = val;
			break;

		case WEBDBG_MQTT:
			dbgData.dbgMqtt = val;
			break;
	}
}


C_CHAR * ReturnDataDebugBuffer(void)
{
		sprintf(response_debug, "%s%d%s%d%s%d%s%d%s%d",
				    "MT=", dbgData.dbgMain,
				"\r\nPT=", dbgData.dbgPolling,
				"\r\nWT=", dbgData.dbgWifi,
				"\r\nMQT=",dbgData.dbgMqtt,
				"\r\nMBE=",dbgData.dbgRtu
				);

        #ifdef __CCL_DEBUG_MODE
        if (strlen(response_debug)>= WEB_DEBUG_BUF_SIZE)
        {
          PRINTF_DEBUG("%s ReturnDataDebugBuffer OWFL %d\n",TAG, strlen(response_debug));
        }
        #endif


		return (C_CHAR*)response_debug;
}


C_CHAR * ReturnStaticDataDebugBuffer(void)
{
	char ntp_tmp[NTP_SERVER_SIZE+1];

	GetNtpServer(&ntp_tmp[0]);

	sprintf(response_debug, "%s%d%s%s%s%s%s%s%s%s%s%s%s%s",
			"Utc Time= ", RTC_Get_UTC_Current_Time(),
			"\r\nNTP=", ntp_tmp,
			"\r\nPN= ", Utilities__GetPN(),
			"\r\nMAC=", Utilities__GetMACAddr(),
			"\r\nFW Ver.= ", GW_FW_REV,
			"\r\nHW Ver.= ", GW_HW_REV,
			"\r\nSpi Ver.= ", GW_SPIFFS_REV
			);

    #ifdef __CCL_DEBUG_MODE
	if (strlen(response_debug)>= WEB_DEBUG_BUF_SIZE)
	{
	  PRINTF_DEBUG("%s ReturnStaticDataDebugBuffer OWFL %d\n",TAG, strlen(response_debug));
	}
    #endif


	return (C_CHAR*)response_debug;
}

