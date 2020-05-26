/**
 * @file RTC_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  implementations target specific related to the managment of the
 *         real time clock.   
 */
#include "RTC_IS.h"
#include <sys/time.h>
#include "data_types_IS.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT 

#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/apps/sntp.h"

#endif
/* Functions implementation -------------------------------------------------------*/

C_TIME Boot_Time = 0;
C_TIME MQTTConnect_Time = 0;

/**
 * @brief RTC_Init
 *
 * Description of what the function does. This part may refer to the parameters
 * 
 * @param  ntp_server url of the NTP server ie. "pool.ntp.org"
 * @param  ntp_port   NTP server port       ie. 123
 * @return C_SUCCESS/C_FAIL 
 */
C_RES RTC_Init(C_URI ntp_server, C_UINT16 ntp_port)
{ /* TO BE Implemented */

  #ifdef INCLUDE_PLATFORM_DEPENDENT  
  #ifdef __DEBUG_RTC_IS_LEV_2
  printf("Initializing SNTP, server: %s, port: %d\n", ntp_server, ntp_port);
  #endif
  sntp_stop();
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, ntp_server);
  sntp_init();
  //sntp_esp_cli_commands_init();  
  #endif
  
  return C_SUCCESS;
}

/**
* @brief RTC_Sync
*
* This function waits for some time until ntp clock can be read
* This is especially needed on GSM model, where ntp clock acquisition takes some more time (with respect to wifi model)
*
* @return C_SUCCESS/C_FAIL
*/
C_RES RTC_Sync(void)
{
	time_t now = 0;
	struct tm timeinfo = { 0 };
	int retry = 0;
	const int retry_count = 10;
	sntp_sync_status_t stat = sntp_get_sync_status();

	while (stat == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        #ifdef __DEBUG_RTC_IS_LEV_2
		printf("Waiting for system time to be set... (%d/%d)\n", retry, retry_count);
        #endif
		vTaskDelay(2000 / portTICK_PERIOD_MS);
		stat = sntp_get_sync_status();
	}
	time(&now);
	localtime_r(&now, &timeinfo);

	if(stat == SNTP_SYNC_STATUS_COMPLETED) {
        #ifdef __DEBUG_RTC_IS_LEV_2
		printf("got time: year:%d, month:%d, day:%d, hour:%d. minute:%d\n", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min);
        #endif
		return C_SUCCESS;
	}
	else return C_FAIL;
}

/**
 * @brief RTC_Get_UTC_Current_Time
 *        return the time in UTC format referred to GMT0 
 * 
 * @param none
 * @return C_TIME UTC Time
 */
C_TIME RTC_Get_UTC_Current_Time(void)
{ /* TO BE Implemented  */
  C_TIME value = 0;
  
  #ifdef INCLUDE_PLATFORM_DEPENDENT  
  struct timeval now_timeval;
  gettimeofday(&now_timeval, NULL);
  value = (uint32_t)now_timeval.tv_sec;
  #endif  
  
  return value;
}
  
  
/**
 * @brief RTC_Get_UTC_Boot_Time
 *        return boot time in UTC format
 * 
 * @param none
 * @return C_TIME UTC Time
 */
C_TIME RTC_Get_UTC_Boot_Time(void)
{
	return Boot_Time;
}

/**
 * @brief RTC_Set_UTC_Boot_Time
 *        set boot time in UTC format 
 * 
 * @param none
 * @return none
 */
void RTC_Set_UTC_Boot_Time(void)
{
	if (Boot_Time == 0)
		Boot_Time = RTC_Get_UTC_Current_Time();
	
	return;
}


/**
 * @brief RTC_Set_UTC_MQTTConnect_Time
 *        set MQTT connect time in UTC format
 *
 * @param none
 * @return none
 */
void RTC_Set_UTC_MQTTConnect_Time(void)
{
	MQTTConnect_Time = RTC_Get_UTC_Current_Time();
	return;
}


/**
 * @brief RTC_Get_UTC_MQTTConnect_Time
 *        return MQTT connect time in UTC format
 *
 * @param none
 * @return C_TIME UTC Time
 */
C_TIME RTC_Get_UTC_MQTTConnect_Time(void)
{
	return MQTTConnect_Time;
}
