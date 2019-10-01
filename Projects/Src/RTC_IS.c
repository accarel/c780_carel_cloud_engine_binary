/**
 * @file RTC_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  implementations target specific related to the managment of the
 *         real time clock.   
 */
#include "RTC_IS.h"



/* Functions implementation -------------------------------------------------------*/

/**
 * @brief RTC_Init
 *
 * Description of what the function does. This part may refer to the parameters
 * 
 * @param C_URI ntp_server
 * @param C_UINT16 ntp_port
 * @return C_SUCCESS/C_FAIL 
 */
C_RES RTC_Init(C_URI ntp_server, C_UINT16 ntp_port)
{ /* TO BE Implemented */
    
  	
  if (1)
  {
      return C_SUCCESS;
  }
  else
  {
	  /* in this case the time is set to zero >= UTC 01 1 Jan 1970 00:00:00 */
	  return C_FAIL;
  }	    	
    	
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
  
 
  
  
  return value;  
}








