/**
 * @file   polling_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         polling engine
 *
 *
 */

#include "MQTT_Interface_CAREL.h"

#include "polling_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
	#include "nvm.h"
	#include "esp_log.h"
	#include "driver/gpio.h"
	#include "mb_m.h"
#endif

#include "polling_IS.h"
#include "utilities_CAREL.h"
#include "sys_IS.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
static xTaskHandle xPollingEngine;
#endif

/**
 * @brief Polling_Engine_Init_IS
 *        init function of the polling engine machine
 *
 * @param  none
 * @return none
 */
void Polling_Engine_Init_IS(void)
{
	//Create Values Buffer
	create_values_buffers();


	req_set_gw_config_t * polling_times = Utilities__GetGWConfigData();
	ForceSending();
	if (BinaryModel_CheckCrc() == C_SUCCESS)
		PollEngine_StartEngine_CAREL();	//if GME is properly configured, then start polling

	while(1)
	{
		// don't remove this line...main function for polling
		DoPolling_CAREL(polling_times);

        Sys__Delay(10);
	}
}



/**
 * @brief CarelEngineMB_Init
 *        task to run Polling_Engine_Init
 *        TO DO...depends on the operating system in use
 *
 * @param  none
 * @return none
 */
void PollEngine_MBStart_IS(void){
   #ifdef INCLUDE_PLATFORM_DEPENDENT

   xTaskCreate(&Polling_Engine_Init_IS, "Poll_engine_init",SENSE_TRIGGER_TASK_STACK_SIZE, NULL, SENSE_TRIGGER_TASK_PRIO, &xPollingEngine);

   #endif
}

void PollEngine_MBResume_IS(void){
  #ifdef INCLUDE_PLATFORM_DEPENDENT

  PRINTF_DEBUG("PE RESUMED\n");
  vTaskResume(xPollingEngine);

  #endif
}

void PollEngine_MBSuspend_IS(void){
#ifdef INCLUDE_PLATFORM_DEPENDENT

	//mbc_master_destroy();
	PRINTF_DEBUG("PE SUSPENDED, free heap : %d\n",esp_get_free_heap_size());
	vTaskSuspend(xPollingEngine);

#endif
}
