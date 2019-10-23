/**
 * @file RTC_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  implementations target specific related to the managment of the
 *         real time clock.   
 */
#include "RTC_IS.h"
#include "data_types_CAREL.h"
#include "data_types_IS.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT 
//depend of device
#include "sntp.h"
#endif
/* Functions implementation -------------------------------------------------------*/

C_TIME Boot_Time = 0;

/**
 * @brief RTC_Init
 *
 * Description of what the function does. This part may refer to the parameters
 * 
 * @param none
 * @return C_SUCCESS/C_FAIL 
 */
C_RES RTC_Init(C_URI ntp_server, C_UINT16 ntp_port)
{ /* TO BE Implemented */
#ifdef INCLUDE_PLATFORM_DEPENDENT
  Init_RTC();	    // RELATED TO ESP32 BOARD
#endif

 	return C_TRUE;
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
  value = Get_UTC_Current_Time(); 			// RELATED TO ESP32 BOARD
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
	if(Boot_Time == 0)
		Boot_Time = RTC_Get_UTC_Current_Time();
	
	return;
}









