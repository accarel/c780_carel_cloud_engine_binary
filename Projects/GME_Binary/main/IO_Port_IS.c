/**
 * @file   IO_Port_IS.c
 * @author carel
 * @date 9 May 2020
 * @brief  functions implementations specific related to the managment of the
 *         I/O Port. 
 *
 */
 
/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
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
#include "esp_log.h"
#endif

#include "sys_IS.h"

//TODO BILATO eliminare a quando finito debug HW e trovata la sequenza di accensione corretta
#include "nvm_CAREL.h"

#include "IO_Port_IS.h"


/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */

#define GSM_INIT "gsm_init"

int CONFIG_RESET_BUTTON = -1;

int ECHO_TEST_TXD = -1;
int ECHO_TEST_RXD = -1;
int ECHO_TEST_RTS = -1;
int TTL_TXD = -1;
int TTL_RXD = -1;
int TTL_RTS = -1;
/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */
/**
 * @brief Check_HW_Platform_IS
 *        read the pin that return the HW platform
 * @return platform
 */

void Init_Button_Pin(void) {
     if (PLATFORM(PLATFORM_DETECTED_2G) || PLATFORM(PLATFORM_DETECTED_WIFI))
    	 CONFIG_RESET_BUTTON = CONFIG_RESET_BUTTON_WIFI;
     else if(PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT))
    	 CONFIG_RESET_BUTTON = CONFIG_RESET_BUTTON_WROVER;

     if (CONFIG_RESET_BUTTON >= 0) {
    	 gpio_pad_select_gpio(CONFIG_RESET_BUTTON);
    	 gpio_set_direction(CONFIG_RESET_BUTTON, GPIO_MODE_INPUT);
    	 gpio_set_pull_mode(CONFIG_RESET_BUTTON, GPIO_PULLUP_ONLY);
     }
}

int Get_Button_Pin(void) {
	return CONFIG_RESET_BUTTON;
}
void Configure_IO_Check_HW_Platform_IS(void)
{
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  gpio_pad_select_gpio(HW_PLATFORM_DETECT_PIN);
  gpio_set_direction(HW_PLATFORM_DETECT_PIN, GPIO_MODE_INPUT);
  #endif
}

void Init_TTL_Pins(void){
	if (PLATFORM(PLATFORM_DETECTED_WIFI)) {
		ECHO_TEST_TXD = ECHO_TEST_TXD_WIFI;
		ECHO_TEST_RXD = ECHO_TEST_RXD_WIFI;
		ECHO_TEST_RTS = ECHO_TEST_RTS_WIFI;
		TTL_TXD = TTL_TXD_WIFI;
		TTL_RXD = TTL_RXD_WIFI;
		TTL_RTS = TTL_RTS_WIFI;
	}
	else if (PLATFORM(PLATFORM_DETECTED_2G)) {
		ECHO_TEST_TXD = ECHO_TEST_TXD_2G;
		ECHO_TEST_RXD = ECHO_TEST_RXD_2G;
		ECHO_TEST_RTS = ECHO_TEST_RTS_2G;
		TTL_TXD = TTL_TXD_2G;
		TTL_RXD = TTL_RXD_2G;
		TTL_RTS = TTL_RTS_2G;
	}
	else if (PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT)) {
		ECHO_TEST_TXD = ECHO_TEST_TXD_WROVER;
		ECHO_TEST_RXD = ECHO_TEST_RXD_WROVER;
		ECHO_TEST_RTS = ECHO_TEST_RTS_WROVER;
		TTL_TXD = TTL_TXD_WROVER;
		TTL_RXD = TTL_RXD_WROVER;
		TTL_RTS = TTL_RTS_WROVER;
	}
	else if (PLATFORM(PLATFORM_DETECTED_BCU)) {
		TTL_TXD = TTL_TXD_BCU;
		TTL_RXD = TTL_RXD_BCU;
		TTL_RTS = TTL_RTS_BCU;
	}
}

int Get_TEST_TXD(void){
	return ECHO_TEST_TXD;
}
int Get_TEST_RXD(void){
	return ECHO_TEST_RXD;
}
int Get_TEST_RTS(void){
	return ECHO_TEST_RTS;
}
int Get_TTL_TXD(void){
	return TTL_TXD;
}
int Get_TTL_RXD(void){
	return TTL_RXD;
}
int Get_TTL_RTS(void){
	return TTL_RTS;
}

/**
 * @brief Check_HW_Platform_IS
 *        read the pin that return the HW platform
 * @return platform
 */
C_BYTE Check_HW_Platform_IS(void)
{
	C_INT16 platform = 0;

    #ifdef INCLUDE_PLATFORM_DEPENDENT

    #if defined( __USE_CAREL_BCU_HW)
	platform = PLATFORM_DETECTED_BCU;
	PRINTF_DEBUG("__USE_CAREL_BCU_HW \r\n");
    #elif defined(__USE_ESP_WROVER_KIT)
	platform = PLATFORM_DETECTED_ESP_WROVER_KIT;
	PRINTF_DEBUG("__USE_ESP_WROVER_KIT \r\n");
    #else
	/* pin is pull down on 2G and pull upped on WiFi*/
	if (gpio_get_level(HW_PLATFORM_DETECT_PIN) == 0)
	{
		platform = PLATFORM_DETECTED_2G;
	    PRINTF_DEBUG("__USE_USR_2G_HW \r\n");
	}
	else
	{
		platform = PLATFORM_DETECTED_WIFI;
		PRINTF_DEBUG("__USE_USR_WIFI_HW \r\n");
	}
	#endif
	#endif
  return platform;
}


void Init_IO_IS(void)
{
  #ifdef INCLUDE_PLATFORM_DEPENDENT

  if PLATFORM(PLATFORM_DETECTED_2G)
  {
	  /* GSM Power ON pin */
	  gpio_pad_select_gpio(GSM_POWER_CTRL_PIN);
	  gpio_set_direction(GSM_POWER_CTRL_PIN, GPIO_MODE_OUTPUT);
	  gpio_set_pull_mode(GSM_POWER_CTRL_PIN, GPIO_PULLDOWN_ONLY);

	  gpio_pad_select_gpio(GSM_PWRKEY_PIN);
	  gpio_set_direction(GSM_PWRKEY_PIN, GPIO_MODE_OUTPUT);
	  gpio_set_level(GSM_PWRKEY_PIN, 0);

	  PRINTF_DEBUG("GSM_Module_IO_Init Done! \r\n");


  }





  //TODO BILATO set unused I/O to a well know state



  #endif
}


/**
 * @brief GSM_Module_Pwr_Supply_On_Off
 *        logically power on/off module (see M95 manual)
 *
 * @param param1 PWRKEY_ON / PWRKEY_OFF
 *
 * @return none
 */
void GSM_Module_Pwr_Supply_On_Off(C_BYTE set_status)
{
	switch (set_status)
	{
	   case GSM_POWER_SUPPLY_OFF:
		   gpio_set_level(GSM_POWER_CTRL_PIN, 0);
		   break;

	   case	GSM_POWER_SUPPLY_ON:
		   gpio_set_level(GSM_POWER_CTRL_PIN, 1);
		   break;

	   default:
         //TODO Bilato raise an error
		 break;
	}
}



/**
 * @brief GSM_Module_Pon_Poff
 *        logically power on/off module (see M95 manual)
 *
 * @param param1 PWRKEY_ON / PWRKEY_OFF
 *
 * @return none
 */

void GSM_Module_PwrKey_On_Off(C_BYTE set_status)
{
	C_BYTE gsm_init;


    #ifdef __DEBUG_GSM_MISCELLANEOUS_IS_LEV_2
    PRINTF_DEBUG("GSM_Module_Pon_Poff set_status %X \r\n", set_status);
    #endif

	switch (set_status)
	{
	   case GSM_PWRKEY_ON :
		   /* pulse the pin to enable the module see M95 manual */
           #if 1

		   NVM__ReadU8Value(GSM_INIT, &gsm_init);

		   PRINTF_DEBUG("GSM PON FLAG >>>> %X \r\n",gsm_init);
		   if (gsm_init == 0)
		   {
			 gsm_init = 1;
			 NVM__WriteU8Value(GSM_INIT, gsm_init);
		     Sys__Delay(GSM_PWRKEY_ON_TIME);
		     gpio_set_level(GSM_PWRKEY_PIN, 1);
		     Sys__Delay(GSM_PWRKEY_ON_TIME);
		     gpio_set_level(GSM_PWRKEY_PIN, 0);
		     Sys__Delay(GSM_PWRKEY_ON_TIME);
		     PRINTF_DEBUG("GSM PON DONE ...........\r\n");
		   }
		   else
		   {
	         gsm_init = 0;
			 NVM__WriteU8Value(GSM_INIT, gsm_init);
			 PRINTF_DEBUG("GSM PON NOT DONE ######\r\n");
			 Sys__Delay(GSM_PWRKEY_ON_TIME);
		   }
           #endif
		   break;

	   case GSM_PWRKEY_OFF:
		   /* pulse the pin to enable the module see M95 manual */
		   gpio_set_level(GSM_PWRKEY_PIN, 0);
		   Sys__Delay(GSM_PWRKEY_OFF_TIME);
		   gpio_set_level(GSM_PWRKEY_PIN, 1);
		   break;

	   default:
		   //TODO Bilato raise an error
		   break;
	}
}

void Init_Pins(void){
	Init_Button_Pin();
	Init_TTL_Pins();
}
