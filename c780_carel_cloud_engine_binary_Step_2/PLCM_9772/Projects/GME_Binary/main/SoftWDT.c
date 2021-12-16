/**
 * @file   SoftWDT.c
 * @author carel
 * @date   9 Sep 2019
 * @brief  a softwd implementation to monitor the system
 *
 */

/* decomment to enable output debug */
//#define __SWWDT_ENABLE_DEBUG_PRINT


#include "types.h"

#include "SoftWDT.h"
#include "data_types_CAREL.h"

static C_INT32 step_count = 0;
volatile softwdt_struct softwdt_items[MAX_SOFTWDT];


/**
 * @brief SoftWDT_Init
 *        configure the one of the available softwdt
 *
 * @param uint8_t  which_one
 * @param uint16_t value
 *
 * @return none
 */
void SoftWDT_Init(uint8_t which_one, uint16_t value){  
  
  if (which_one >= MAX_SOFTWDT) {
    return; //it trigger some other error
  }
      
  softwdt_items[which_one].wdt_counter_preset = value;
  softwdt_items[which_one].wdt_counter_value = softwdt_items[which_one].wdt_counter_preset;
  softwdt_items[which_one].is_enabled = 1;
}


/**
 * @brief SoftWDT_DeInit
 *        remove one of the available softwdt
 *
 * @param uint8_t  which_one
 *
 * @return none
 */
void SoftWDT_DeInit(uint8_t which_one){  
  
  if (which_one >= MAX_SOFTWDT) {
    return; //it trigger some other error
  }
      
  softwdt_items[which_one].is_enabled = 0;
}


/**
 * @brief SoftWDT_Reset
 *        reset one of the available softwdt
 *
 * @param uint8_t  which_one
 *
 * @return none
 */
void SoftWDT_Reset(uint8_t which_one){
  if (which_one >= MAX_SOFTWDT) 
  {
	P_COV_LN;
    return; //it trigger some other error
  }
  
  if (softwdt_items[which_one].is_enabled == 1)
    softwdt_items[which_one].wdt_counter_value = softwdt_items[which_one].wdt_counter_preset;
}

/**
 * @brief SoftWDT_Manager
 *
 * @param none
 *
 * @return none
 */
void SoftWDT_Manager(void){
    uint8_t count;
    uint8_t softwdt_fired;
    
    softwdt_fired = 0;

#ifdef __SWWDT_ENABLE_DEBUG_PRINT
    printf("SoftWDT Running ( %d )\r\n", step_count++);
#endif

    for (count=0; count < MAX_SOFTWDT; count++)
    {       
      if (softwdt_items[count].wdt_counter_value > 0)
        softwdt_items[count].wdt_counter_value--;  
      
      if ((softwdt_items[count].is_enabled) && (softwdt_items[count].wdt_counter_value == 0)){
        softwdt_fired = 1; 

        #ifdef __SWWDT_ENABLE_DEBUG_PRINT
           printf("SoftWDT Fired!= %d\r\n",count);
        #endif

      }       
    }  
    
    if (softwdt_fired == 1){
      //TODO something like fire wd or what else?      
      softwdt_fired = 0;      
      P_COV_LN;
      // Reset del sistema in base alla piattaforma su cui si sta lavorando
    }
   
  }


/**
 * @brief esp_task_wdt_isr_user_handler
 *        called by task_wdt_isr function (ISR for when TWDT times out)
 * @param none
 * @return none
 */
void esp_task_wdt_isr_user_handler(void)
{
	/* restart firmware */
	esp_restart();
}

