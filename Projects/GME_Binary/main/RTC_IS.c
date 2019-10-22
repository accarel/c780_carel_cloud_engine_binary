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


//depend of device
#include "sntp.h"

/* Functions implementation -------------------------------------------------------*/

/**
 * @brief RTC_Init
 *
 * Description of what the function does. This part may refer to the parameters
 * 
 * @param none
 * @return C_SUCCESS/C_FAIL 
 */
C_RES RTC_Init(void)
{ /* TO BE Implemented */


  Init_RTC();	    // RELATED TO ESP32 BOARD


  /*	
  if ( ... )
  {
      return C_SUCCESS;
  }
  else
  {
	  // in this case the time is set to zero >= UTC 01 1 Jan 1970 00:00:00
	  return C_FAIL;
  }	 */
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
{
  /* TO BE Implemented  */
  C_TIME value = 0;
  
  value = Get_UTC_Current_Time(); 			// RELATED TO ESP32 BOARD
  
  return value;  
}








