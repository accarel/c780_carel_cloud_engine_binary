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

#endif

#include "mobile.h"
#include "IO_Port_IS.h"
#include "sys_IS.h"
#include "GSM_Miscellaneous_IS.h"


/* ========================================================================== */
/* DEVELOPMENT / TARGET PLATFORM CUSTOMIZATION                                */
/* ========================================================================== */



/* Functions implementations -------------------------------------------------------*/
#ifdef GW_BYPASS_ESP32
/**
 * @brief GSM_BypassEsp32
 *        Bypass ESP32 to let AT commands be directly sent from ttl uart port
 *        to M95 module
 *        This function should only be used when doing RF tests on GSM transmission
 *        NOT TO BE USED IN FINAL FW!!!
 * @param void
 * @param void
 * @return none
 */
void GSM_BypassEsp32(void)
{
    printf("FW for 2G model certification\n");

	// Init IOs
    Init_IO_IS();

    // Power on M95 module
    GSM_Module_Pwr_Supply_On_Off(GSM_POWER_SUPPLY_ON);
    GSM_Module_PwrKey_On_Off(GSM_PWRKEY_ON);

    // Init console on ttl uart
    Sys__ConsoleInit();

    // Init uart for M95 and initialize modem
    Mobile__Init();
    char* line;
    line = malloc(100);
    char answer[100];
    while(1)
    {
    	// read a string from uart
    	/* Get a line using linenoise.
    	* The line is returned when ENTER is pressed.
    	*/
    	line = Sys__GetLine();
    	if (line != NULL) {
        //  printf("received line %s\n", line);

          // send command to M95
          Mobile_SendGenericCommand(line, answer);

          // return answer from M95
          printf("%s", answer);
    	}
    }
}
#endif

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


