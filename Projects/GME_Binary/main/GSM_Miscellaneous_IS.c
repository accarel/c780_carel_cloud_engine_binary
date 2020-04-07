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

#include "nvm_CAREL.h"


#include "IO_Port_IS.h"

#include "GSM_Miscellaneous_IS.h"


/* take a look to M95_Hardware_Design time are in ms */
#define PWRKEY_ON_TIME		800
#define PWRKEY_OFF_TIME		700



/* ========================================================================== */
/* DEVELOPMENT / TARGET PLATFORM CUSTOMIZATION                                */
/* ========================================================================== */
#define PWRKEY_OFF    1
#define PWRKEY_ON     0


#define GSM_INIT "gsm_init"

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


void GSM_Module_IO_Init(void)
{
  #ifdef __USE_USR_2G_HW



  gpio_pad_select_gpio(PWRKEY_PIN);
  gpio_set_direction(PWRKEY_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(PWRKEY_PIN, 0);

  #ifdef __DEBUG_GSM_MISCELLANEOUS_IS_LEV_2
  printf("GSM_Module_IO_Init Done! \r\n");
  #endif
  #endif
}

/**
 * @brief GSM_Module_Pon_Poff
 *        logically power on/off module (see M95 manual)
 *
 * @param param1 PWRKEY_ON / PWRKEY_OFF
 *
 * @return none
 */

void GSM_Module_Pon_Poff(C_BYTE set_status)
{
	C_BYTE gsm_init;

#ifdef __USE_USR_2G_HW

    #ifdef __DEBUG_GSM_MISCELLANEOUS_IS_LEV_2
    printf("GSM_Module_Pon_Poff set_status %X \r\n", set_status);
    #endif

	switch (set_status)
	{




	   case PWRKEY_ON :
		   /* pulse the pin to enable the module see M95 manual */
           #if 1

		   NVM__ReadU8Value(GSM_INIT, &gsm_init);

		   printf("GSM PON >>>> %X \r\n",gsm_init);
		   if (gsm_init == 0)
		   {
			 gsm_init = 1;
			 NVM__WriteU8Value(GSM_INIT, gsm_init);
		     Sys__Delay(PWRKEY_ON_TIME);
		     gpio_set_level(PWRKEY_PIN, 1);
		     Sys__Delay(PWRKEY_ON_TIME);
		     gpio_set_level(PWRKEY_PIN, 0);
		     Sys__Delay(PWRKEY_ON_TIME);
		     printf("GSM PON DONE ...........\r\n");
		   }
		   else
		   {
	         gsm_init = 0;
			 NVM__WriteU8Value(GSM_INIT, gsm_init);
			 printf("GSM PON NOT DONE ######\r\n");
			 Sys__Delay(PWRKEY_ON_TIME);
		   }
           #endif
		   break;

	   case PWRKEY_OFF:
		   /* pulse the pin to enable the module see M95 manual */
		   gpio_set_level(PWRKEY_PIN, 0);
		   Sys__Delay(PWRKEY_OFF_TIME);
		   gpio_set_level(PWRKEY_PIN, 1);
		   break;
	}

#endif
}


