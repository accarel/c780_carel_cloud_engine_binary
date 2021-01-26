/**
 * @file   main_IS.c
 * @author carel
 * @date 7 january 2020
 * @brief  Implementation specific function implementations of main application engine
 *
 */

#include "main_IS.h"
#include "main_CAREL.h"
#include "data_types_CAREL.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


/**
 * @brief Carel_Main_Task_Start
 *		  Call the Carel_Main_Task in main_CAREL.c
 *
 * @param  none
 * @return none
 */
void Carel_Main_Task_Start(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
  xTaskCreate(Carel_Main_Task, "Carel_Task", 3*(CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE+512), NULL, tskIDLE_PRIORITY, NULL );
#endif
}

/**
 * @brief GME_Reboot_IS
 *		  Call the ESP32 specific function to restart the module
 * @param  none
 * @return none
 */
void GME_Reboot_IS(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	esp_restart();
#endif
}
