/**
 * @file   sys_IS.c
 * @author carel
 * @date 20 Nov 2019
 * @brief ... TODO
 *        
 */ 
 

#include "sys_IS.h"
#include "File_System_IS.h"
#include "https_client_CAREL.h"

#include "binary_model.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
  #include "esp_partition.h"
  #include "nvm.h"
  #include "wifi.h"
  #include "common.h"
  #include "cmd_custom.h"
  #include "driver/gpio.h"
  #include "http_server.h"
  #include "esp_ota_ops.h"
#endif
 
 
static uint32_t factory_reset_debounce_counter = 0;
static uint32_t config_reset_debounce_counter = 0;

 
 /**
 * @brief Sys__SetFactoryBootPartition
 *     Change boot partition to factory
 *
 *
 * @param  none
 * @return C_RES
 */
C_RES Sys__SetFactoryBootPartition(void){
	
	C_RES err;
	
#ifdef INCLUDE_PLATFORM_DEPENDENT
	const esp_partition_t *factory = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);

	err = esp_ota_set_boot_partition(factory);

	if (err != C_SUCCESS) {
		printf("SetFactoryBootPartition failed!  err=0x%d", err);
		return err;
	}else{
		 printf("SetFactoryBootPartition Succeeded!");
		return C_SUCCESS;
	}
#endif

	return C_FAIL;
}
 
 
 
 /**
 * @brief Sys_ResetCheck
 *      test the button for system reset   
 *
 *
 * @param  none
 * @return C_RES
 */ 
C_BOOL Sys__ResetCheck(void){
	return C_FALSE;
	// Debounce check using up/down counter every 10ms
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if (gpio_get_level(CONFIG_RESET_BUTTON) == 0) {
		config_reset_debounce_counter++;		 
		Sys__Delay(100);
	}else if(gpio_get_level(CONFIG_RESET_BUTTON) == 1 && config_reset_debounce_counter > 0){
		config_reset_debounce_counter--;
		Sys__Delay(100);
	}
	if((CONFIG_RESET_SEC * 10) == config_reset_debounce_counter){
		//Erase configuration flag form NVM
		WiFi__ErasingConfig();
		unlink(MODEL_FILE);
		PRINTF_DEBUG("CONFIG RESET CHECK DONE = %d\n",config_reset_debounce_counter);
		return C_TRUE;
	}
#endif
	return C_FALSE;
}


/**
 * @brief Sys_FirmwareFactoryReset
 *      TODO   
 *
 *
 * @param  none
 * @return C_RES
 */
C_BOOL Sys__FirmwareFactoryReset(void){
	// Debounce check using up/down counter every 10ms
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if (gpio_get_level(FACTORY_RESET_BUTTON) == 0) {
		factory_reset_debounce_counter++;
		 
		Sys__Delay(100);
	}else if(gpio_get_level(FACTORY_RESET_BUTTON) == 1 && factory_reset_debounce_counter > 0){
		factory_reset_debounce_counter--;
		 
		Sys__Delay(100);
	}
	if((FACTORY_RESET_SEC * 10) == factory_reset_debounce_counter){
		//Point boot partition on /factory part.
		Sys__SetFactoryBootPartition();
		printf("GME Configuration has bees reseted\n");
		PRINTF_DEBUG("FIRMWARE RESET CHECK DONE = %d\n",factory_reset_debounce_counter);
		return C_TRUE;
	}
#endif
	return C_FALSE;
}

C_UINT32 Sys__GetFreeHeapSize(void){
	C_UINT32 freemem = 0;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	freemem = esp_get_free_heap_size();
#endif
	return freemem;
}

void Sys__Delay(C_UINT32 delay){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	vTaskDelay(delay / portTICK_PERIOD_MS);
#endif
}
