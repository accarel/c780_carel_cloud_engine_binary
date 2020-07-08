/**
 * @file   sys_IS.c
 * @author carel
 * @date 20 Nov 2019
 * @brief  Contain a system util function
 *         related to the type of operating system in use and
 *         related to the chip.
 *        
 */ 
#include "CAREL_GLOBAL_DEF.h"
#include "gme_config.h"
#include "data_types_CAREL.h"

#include "sys_IS.h"
#include "File_System_IS.h"
#include "https_client_CAREL.h"

#include "binary_model.h"
#include "IO_Port_IS.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
  #include "esp_partition.h"
  #include "nvm_CAREL.h"
  #include "wifi.h"
  #include "common.h"

  #include "driver/gpio.h"
  #include "http_server.h"
  #include "esp_ota_ops.h"
#endif
#include "Led_Manager_IS.h"

 /**
 * @brief Sys_ResetCheck
 *      test the button for system reset   
 *
 *
 * @param  none
 * @return C_RES
 */ 
void Sys__ResetCheck(void){

    #ifdef INCLUDE_PLATFORM_DEPENDENT

	static C_TIME TimerForButton = 0;
	C_TIME CurrentTime = 0;
	static int button_state = BUTTON_WAIT;

	switch(button_state){
		case BUTTON_WAIT:
			if (gpio_get_level(Get_Button_Pin()) == 0){	//pressed
				TimerForButton = RTC_Get_UTC_Current_Time();
				PRINTF_DEBUG_SYS("start button pressure at %d\n", RTC_Get_UTC_Current_Time());
				button_state = BUTTON_PRESSED;
				Sys__Delay(100);
			}
		break;

		case BUTTON_PRESSED:
			CurrentTime = RTC_Get_UTC_Current_Time();
			if (gpio_get_level(Get_Button_Pin()) == 1){  //released
				if ( (CurrentTime - TimerForButton) >= 0 && (CurrentTime - TimerForButton) <= 5) {
					PRINTF_DEBUG_SYS("button released after %d, before 5 seconds\n", CurrentTime - TimerForButton);
					TimerForButton = 0;
					button_state = BUTTON_WAIT;
					return;
				}
				else if ( (CurrentTime - TimerForButton) > 5 && (CurrentTime - TimerForButton) <= 10) {
					// do a reboot
					PRINTF_DEBUG_SYS("button released after %d, between 5 and 10 seconds\n", CurrentTime - TimerForButton);
					GME__Reboot();
				}
			}
			if ( (CurrentTime - TimerForButton) > 10) {
				PRINTF_DEBUG_SYS("button not released after %d, 10 seconds\n", CurrentTime - TimerForButton);
				button_state = BUTTON_SLOWBLINK;
			}
		break;

		case BUTTON_SLOWBLINK:
			//led status blink slow
			PRINTF_DEBUG_SYS("start led blinking slowly\n");
			Update_Led_Status(LED_STAT_FACT_DEF_A, LED_STAT_ON);
			TimerForButton = RTC_Get_UTC_Current_Time();
			button_state = BUTTON_WAITRELEASE;
		break;

		case BUTTON_WAITRELEASE:
			CurrentTime = RTC_Get_UTC_Current_Time();
			if ( (CurrentTime - TimerForButton) > 10 ) {
				if (gpio_get_level(Get_Button_Pin()) == 1) { //released
					PRINTF_DEBUG_SYS("button released after %d, between 0 and 5 seconds\n", CurrentTime - TimerForButton);
					TimerForButton = CurrentTime;
					button_state = BUTTON_FASTBLINK;
				}
				else {
					PRINTF_DEBUG_SYS("factory setting procedure aborted\n");
					Update_Led_Status(LED_STAT_FACT_DEF_A, LED_STAT_OFF);
					button_state = BUTTON_WAIT;
				}
			}
		break;

		case BUTTON_FASTBLINK:
			//led status blink fast
			PRINTF_DEBUG_SYS("start led blinking fast\n");
			Update_Led_Status(LED_STAT_FACT_DEF_A, LED_STAT_OFF);
			Update_Led_Status(LED_STAT_FACT_DEF_B, LED_STAT_ON);
			TimerForButton = RTC_Get_UTC_Current_Time();
			button_state = BUTTON_WAITFACTORY;
		break;

		case BUTTON_WAITFACTORY:
			CurrentTime = RTC_Get_UTC_Current_Time();
			if ( (CurrentTime - TimerForButton) >= 0 && (CurrentTime - TimerForButton) <= 5) {
				if (gpio_get_level(Get_Button_Pin()) == 0){  //pressed
					PRINTF_DEBUG_SYS("button pressed again after %d, between 0 and 5 seconds: go FACTORY\n", CurrentTime - TimerForButton);

					// do fact setting
					NVM__EraseAll();
					unlink(MODEL_FILE);
					PRINTF_DEBUG_SYS("Rebooting after factory setting\n");
					GME__Reboot();
				}
			}
			else {
				PRINTF_DEBUG_SYS("factory setting procedure aborted\n");
				button_state = BUTTON_WAIT;
				Update_Led_Status(LED_STAT_FACT_DEF_B, LED_STAT_OFF);
			}
			break;
		}
	#endif

	return;
}

/**
* @brief Sys__GetFreeHeapSize
*      this function call  esp_get_free_heap_size(),
*      refer to esp-idf for more details.
*
*
* @param  none
* @return C_UINT32
*/
C_UINT32 Sys__GetFreeHeapSize(void){
	C_UINT32 freemem = 0;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	freemem = esp_get_free_heap_size();
#endif
	return freemem;
}


/**
* @brief Sys__Delay
*      this function call a delay (in millisecond)
*	   it depends on the operating system in use.
*
* @param  C_UINT32 delay
* @return none
*/
void Sys__Delay(C_UINT32 delay){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	vTaskDelay(delay / portTICK_PERIOD_MS); //portTICK_PERIOD_MS 10
#endif
}
