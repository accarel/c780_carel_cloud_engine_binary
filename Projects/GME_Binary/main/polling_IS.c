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
	#include "nvm_CAREL.h"
	#include "esp_log.h"
	#include "driver/gpio.h"
	#include "mb_m.h"
#endif

#include "polling_IS.h"
#include "utilities_CAREL.h"
#include "sys_IS.h"

#include "SoftWDT.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
static xTaskHandle xPollingEngine;
#endif

/**
 * @brief Polling_Engine_Init_IS
 *        Init function of the polling engine machine
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
	//if GME is properly configured...
	if (BinaryModel_CheckCrc() == C_SUCCESS) {
		uint8_t pe_status;
		if (C_SUCCESS != NVM__ReadU8Value(PE_STATUS_NVM, &pe_status)) {
			PollEngine_StartEngine_CAREL();	// first time here, then start polling
			NVM__WriteU8Value(PE_STATUS_NVM, RUNNING); // and save current status to nvm
		}
		else {
			if (pe_status == RUNNING)
				PollEngine_StartEngine_CAREL();	// start polling
			else
				PollEngine_StopEngine_CAREL();	// don't start polling
		}
	}

	SoftWDT_Init(SWWDT_POLLING, SWWDT_DEFAULT_TIME);

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
 *        NB: depends on the operating system in use
 *
 * @param  none
 * @return none
 */
void PollEngine_MBStart_IS(void){
   #ifdef INCLUDE_PLATFORM_DEPENDENT

   xTaskCreate(&Polling_Engine_Init_IS, "Poll_engine_init",SENSE_TRIGGER_TASK_STACK_SIZE, NULL, SENSE_TRIGGER_TASK_PRIO, &xPollingEngine);

   #endif
}

/**
 * @brief PollEngine_MBResume_IS
 *        resume the task  Polling_Engine
 *
 * @param  none
 * @return none
 */
void PollEngine_MBResume_IS(void){
  #ifdef INCLUDE_PLATFORM_DEPENDENT

  PRINTF_DEBUG("PE RESUMED\n");
  vTaskResume(xPollingEngine);

  #endif
}


/**
 * @brief PollEngine_MBSuspend_IS
 *        suspend the task  Polling_Engine
 *
 * @param  none
 * @return none
 */
void PollEngine_MBSuspend_IS(void){
#ifdef INCLUDE_PLATFORM_DEPENDENT

	//mbc_master_destroy();
	PRINTF_DEBUG("PE SUSPENDED, free heap : %d\n",esp_get_free_heap_size());
	vTaskSuspend(xPollingEngine);

#endif
}
