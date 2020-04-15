/**
 * @file GSM_Miscellanous_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         real time clock.
 */


/* Includes ------------------------------------------------------------------*/
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "sys_IS.h"
#endif




#include "IO_Port_IS.h"

#include "GSM_Miscellaneous_IS.h"


/* ========================================================================== */
/* DEVELOPMENT / TARGET PLATFORM CUSTOMIZATION                                */
/* ========================================================================== */


/* Functions implementations -------------------------------------------------------*/
/**
 * @brief GSM_Misc_SMS_Config_Received
 *        Manage the reception of the SMS that contain configurations informations
 *        like APN name and so on    
 * @param param1 Description of the first parameter of the function.
 * @param param2 The second one, which follows @p param1.
 * @return none
 */
void GSM_Misc_SMS_Config_Received(void)
{ /* TO BE implemented */
	
}


/**
 * @brief GSM_Misc_Get_Communication_Status
 *        Return the status of the communication interface 
 *        
 * @param param1 Description of the first parameter of the function.
 * @param param2 The second one, which follows @p param1.
 *
 * @return none
 */

void GSM_Misc_Get_Communication_Status(void)
{ /* TO BE implemented */

  /* this function return some parameters of the GSM module
     signal strenght
	 connection status
	 SIM status(?)
  */


	
}


void GSM_Misc_Get_Gateway_ID(C_BYTE *s_id)
{ /* TO BE implemented */
  
  /* this function return the IMEI of the GSM module*/
  	
	
}


