/**
 * @file Led_Manager_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the LEDs
 *
*/


#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#endif

#include "RTC_IS.h"
#include "sys_IS.h"
#include "IO_Port_IS.h"

#include "Led_Manager_IS.h"


/**
 * @brief LED_STATUS_IO
 *        is platform dependent redefine it to match your I/O schema
 */

/*
if we need specific polarity for different platform move this define
duplicate & copy inside each platform
*/
#define LED_PHY_ON   1
#define LED_PHY_OFF  0

int LED_GREEN = -1;
int LED_RED = -1;
int LED_BLU = -1;

/* ========================================================================== */
/* development platform                                                       */
/* ========================================================================== */


/* ========================================================================== */
/*  OFFICIAL HW PLATFORM                                                      */
/* ========================================================================== */

/* ========================================================================== */
/*  __USE_USR_WIFI_HW                                                         */
/* ========================================================================== */
/*
The WiFi model use a two wire bicolor led the control of it
follow this table

LED           |IO27|IO5 |
______________|____|____|
BOTH OFF      | 0  | 0  |
LED RED    ON | 0  | 1  |
LED GREEN  ON | 1  | 0  |
FORBIDDEN     | 1  | 1  |
*/
typedef enum{
	LED_BOTH_OFF  = 0,
	LED_RED_ON    = 1,
	LED_GREEN_ON  = 2
}Led_Bicolor_Status_t;

/* ========================================================================== */
/*  COMMON PART                                                               */
/* ========================================================================== */
typedef enum{
	LED_OFF        = 0,
	LED_ON         = 1,
	LED_BLINK_SLOW = 2,
	LED_BLINK_FAST = 3 
}Led_Show_Status_t;


/**
 * @brief led_current_status
 *        contain the current status of the leds, this is platform indepented
 *        each platform could be used different led or indication 
 */
volatile C_UINT16 led_current_status = 0;

#ifdef INCLUDE_PLATFORM_DEPENDENT
#define LED_TASK_TASK_STACK_SIZE   (1024)
//configMINIMAL_STACK_SIZE
#define LED_TASK_TASK_PRIO         tskIDLE_PRIORITY
static xTaskHandle xLedTask;
#endif

/* ------------------------------------------------------------------- */
/*           BEGIN OF PLATFORM INDEPENDENT LED MANAGEMENT              */
/* ------------------------------------------------------------------- */

/**
 * @brief Update_Led_Status this routine set the leds of the device to a 
 * specific configuration select from a combination of the following
 * not all combination are valid, but are intuitive LED_STAT_TEST
 * must be use exclusively 
 LED_STAT_ALL_OFF    
 LED_STAT_MODEL_CFG  
 LED_STAT_MQTT_CONN  
 LED_STAT_RS485      
 
 LED_STAT_TEST        
 LED_STAT_FACT_DEF_A 
 LED_STAT_FACT_DEF_B 
 
 * @param none
 * @return none
 */
void Update_Led_Status(C_UINT16 set_status, C_BYTE status)
{

	if (status == LED_STAT_ON)
	{
		led_current_status = (led_current_status | set_status);
	}
	else
	{
		led_current_status = (led_current_status & (~set_status));
	}

}

/**
 * @brief Task_Led_Status
 *        TAsk that manage the led status
 *
 * @param  none
 * @return none
 */
void Task_Led_Status(void)
{

    if (led_current_status & LED_STAT_TEST)
	{
		Do_Led_Test_Routine();
		led_current_status &= ~LED_STAT_TEST;
	}


    if (led_current_status & LED_STAT_MODEL_CFG)
	{
		Update_Led_Model_Cfg(1);		
	}
	else
	{
		Update_Led_Model_Cfg(0);
	}
	

    if (led_current_status & LED_STAT_MQTT_CONN)
	{
		Update_Led_MQTT_Conn(1);		
	}
	else
	{
		Update_Led_MQTT_Conn(0);
	}
	
	
    if (led_current_status & LED_STAT_RS485)
	{
		Update_Led_RS485(1);		
	}	
	else
	{
		Update_Led_RS485(0);
	}


    if (led_current_status & LED_STAT_FACT_DEF_A)
	{
		Update_Led_Fact_Def_A(1);
	}
	else
	{
		Update_Led_Fact_Def_A(0);
	}

    if (led_current_status & LED_STAT_FACT_DEF_B)
	{
		Update_Led_Fact_Def_B(1);
	}
	else
	{
		Update_Led_Fact_Def_B(0);
	}
}

/**
 * @brief Led_Status_Update
 *        This function is usefull to change the status led
 *
 * @param  Led_Show_Status_t status
 *         int led
 * @return none
 */
void Led_Status_Update(Led_Show_Status_t status, int led){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  static Led_Show_Status_t blink_status_red = LED_OFF;
  static Led_Show_Status_t blink_status_green = LED_OFF;
  static Led_Show_Status_t blink_status_blu = LED_OFF;
  static C_INT32 blink_timer_red = 0;
  static C_INT32 blink_timer_green = 0;
  static C_INT32 blink_timer_blu = 0;

    //printf("Status %X\r\n", (uint16_t)status);
  if(led < 0)
	  return;

  if (led == LED_RED){
	  blink_status = blink_status_red;
	  blink_timer = blink_timer_red;
  }
  else if (led == LED_GREEN){
	  blink_status = blink_status_green;
	  blink_timer = blink_timer_green;
  }
  else if (led == LED_BLU){
  	  blink_status = blink_status_blu;
  	  blink_timer = blink_timer_blu;
  }

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
    	  gpio_set_level(led, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(led, LED_PHY_ON);
		  #endif
		  break;

		case LED_BLINK_SLOW:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		      /* put the I/O instruction here */
		      #ifdef INCLUDE_PLATFORM_DEPENDENT
					gpio_set_level(led, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
            	if PLATFORM(PLATFORM_DETECTED_WIFI) {
					  gpio_set_level(LED_RED, LED_PHY_OFF);
					  gpio_set_level(LED_GREEN, LED_PHY_OFF);
           		}
       			else
       				gpio_set_level(led, LED_PHY_OFF);
			  #endif

              blink_status = LED_OFF;
            }

			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT
          		  gpio_set_level(led, LED_PHY_ON);
             #endif
             blink_status = LED_ON;
            }
            else
            {
              /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
            	if PLATFORM(PLATFORM_DETECTED_WIFI) {
				  gpio_set_level(LED_RED, LED_PHY_OFF);
				  gpio_set_level(LED_GREEN, LED_PHY_OFF);
            	}
            	else
            		gpio_set_level(led, LED_PHY_OFF);
			  #endif
              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}

    if (led == LED_RED){
  	  blink_status_red = blink_status;
  	  blink_timer_red = blink_timer;
    }
    else if (led == LED_GREEN){
  	  blink_status_green = blink_status;
  	  blink_timer_green = blink_timer;
    }
    else if (led == LED_BLU){
      blink_status_blu = blink_status;
      blink_timer_blu = blink_timer;
    }
}

/* ------------------------------------------------------------------- */
/*           BEGIN OF PLATFORM DEPENDENT LED MANAGEMENT                */
/* ------------------------------------------------------------------- */

//#ifdef __USE_CAREL_BCU_HW

/**
 * @brief Update_Led_Model_Cfg_bcu
 *
 * @param  C_BYTE model_cfg_status
 * @return none
 */
void Update_Led_Model_Cfg_bcu(C_BYTE model_cfg_status)
{

  if ((model_cfg_status==1) && (led_current_status & LED_STAT_MQTT_CONN))
  {
   	 Led_Status_Update(LED_ON, LED_GREEN);
  }

  if ((model_cfg_status==1) && ((led_current_status & LED_STAT_MQTT_CONN)==0))
  {
   	 Led_Status_Update(LED_BLINK_SLOW, LED_GREEN);
  }

  if ((model_cfg_status==0) && ((led_current_status & LED_STAT_MQTT_CONN)==0))
  {
   	 Led_Status_Update(LED_OFF, LED_GREEN);
  }

}

/**
 * @brief Update_Led_MQTT_Conn_bcu
 *
 * @param  C_BYTE mqtt_conn_status
 * @return none
 */
void Update_Led_MQTT_Conn_bcu(C_BYTE mqtt_conn_status)
{
	  if ((mqtt_conn_status==1) && (led_current_status & LED_STAT_MODEL_CFG))
	  {
	   	 Led_Status_Update(LED_ON, LED_GREEN);
	  }

	  if ((mqtt_conn_status==1) && ((led_current_status & LED_STAT_MODEL_CFG)==0))
	  {
	   	 Led_Status_Update(LED_BLINK_SLOW, LED_GREEN);
	  }

	  if ((mqtt_conn_status==0) && ((led_current_status & LED_STAT_MODEL_CFG)==0))
	  {
	   	 Led_Status_Update(LED_OFF, LED_GREEN);
	  }
}

/**
 * @brief Update_Led_RS485_bcu
 *
 * @param  C_BYTE rs485_status
 * @return none
 */
void Update_Led_RS485_bcu(C_BYTE rs485_status)
{
/*
 right now for the bCU model there isn't any TTL internal
 activity indicator, so do nothing
*/
}

/**
 * @brief Update_Led_Fact_Def_A_bcu
 *
 * @param  C_BYTE fact_def_a_status
 * @return none
 */
void Update_Led_Fact_Def_A_bcu(C_BYTE fact_def_a_status)
{
//do nothing no led available
}

/**
 * @brief Update_Led_Fact_Def_B_bcu
 *
 * @param  C_BYTE fact_def_a_status
 * @return none
 */
void Update_Led_Fact_Def_B_bcu(C_BYTE fact_def_a_status)
{
//do nothing no led available
}


//#endif



/**
 * @brief Update_Led_Model_Cfg_2g
 *
 * @param  C_BYTE model_cfg_status
 * @return none
 */
void Update_Led_Model_Cfg_2g(C_BYTE model_cfg_status)
{
/*
                        RED
Model configured         ON
*/
  if (model_cfg_status==1)
  {
   	 Led_Status_Update(LED_ON, LED_RED);
  }
  else
  {
	 Led_Status_Update(LED_OFF, LED_RED);
  }
}

/**
 * @brief Update_Led_MQTT_Conn_2g
 *
 * @param  C_BYTE mqtt_conn_status
 * @return none
 */
void Update_Led_MQTT_Conn_2g(C_BYTE mqtt_conn_status)
{
/*
                        GREEN
MQTT connected          ON
*/
	if (mqtt_conn_status == 1)
	{
      Led_Status_Update(LED_ON, LED_GREEN);
	}
	else
	{
      Led_Status_Update(LED_OFF, LED_GREEN);
	}

}

/**
 * @brief Update_Led_RS485_2g
 *
 * @param  C_BYTE rs485_status
 * @return none
 */
void Update_Led_RS485_2g(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
  if (rs485_status)
  {
	  Led_Status_Update(LED_ON, LED_BLU);
  }
  else
  {
	  Led_Status_Update(LED_OFF, LED_BLU);
  }
}

/**
 * @brief Update_Led_Fact_Def_A_2g
 *
 * @param  C_BYTE fact_def_a_status
 * @return none
 */
void Update_Led_Fact_Def_A_2g(C_BYTE fact_def_a_status)
{
	if (fact_def_a_status == 1)
	{
		Led_Status_Update(LED_BLINK_SLOW, LED_GREEN);
	}
}

/**
 * @brief Update_Led_Fact_Def_B_2g
 *
 * @param  C_BYTE fact_def_a_status
 * @return none
 */
void Update_Led_Fact_Def_B_2g(C_BYTE fact_def_a_status)
{
	if (fact_def_a_status == 1)
	{
		Led_Status_Update(LED_BLINK_FAST, LED_GREEN);
	}
}

//
//#endif


//#ifdef __USE_ESP_WROVER_KIT

/**
 * @brief Update_Led_Model_Cfg_esp_wrover_kit
 *
 * @param  C_BYTE model_cfg_status
 * @return none
 */
void Update_Led_Model_Cfg_esp_wrover_kit(C_BYTE model_cfg_status)
{
/*
                         RED
Model configured         ON
*/

  if (model_cfg_status==1)
  {
   	 Led_Status_Update(LED_ON, LED_RED);
  }
  else
  {
	 Led_Status_Update(LED_OFF, LED_RED);
  }
}

/**
 * @brief Update_Led_MQTT_Conn_esp_wrover_kit
 *
 * @param  C_BYTE mqtt_conn_status
 * @return none
 */
void Update_Led_MQTT_Conn_esp_wrover_kit(C_BYTE mqtt_conn_status)
{
/*
                        GREEN
MQTT connected          ON
*/
	if (mqtt_conn_status == 1)
	{
      Led_Status_Update(LED_ON, LED_GREEN);
	}
	else
	{
      Led_Status_Update(LED_OFF, LED_GREEN);
	}
}

/**
 * @brief Update_Led_RS485_esp_wrover_kit
 *
 * @param  C_BYTE rs485_status
 * @return none
 */
void Update_Led_RS485_esp_wrover_kit(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
  if (rs485_status)
  {
	  Led_Status_Update(LED_ON, LED_BLU);
  }
  else
  {
	  Led_Status_Update(LED_OFF, LED_BLU);
  }
}

/**
 * @brief Update_Led_Fact_Def_A_esp_wrover_kit
 *
 * @param  C_BYTE fact_def_a_status
 * @return none
 */
void Update_Led_Fact_Def_A_esp_wrover_kit(C_BYTE fact_def_a_status)
{
	if (fact_def_a_status == 1)
	{
		Led_Status_Update(LED_BLINK_SLOW, LED_RED);
	}
}

/**
 * @brief Update_Led_Fact_Def_B_esp_wrover_kit
 *
 * @param  C_BYTE fact_def_b_status
 * @return none
 */
void Update_Led_Fact_Def_B_esp_wrover_kit(C_BYTE fact_def_b_status)
{
	if (fact_def_b_status == 1)
	{
		Led_Status_Update(LED_BLINK_FAST, LED_RED);
	}
}

//#endif



/**
 * @brief Update_Led_Model_Cfg_wifi
 *        This function used to see the led configuration of Wifi model
 *
 * @param  C_BYTE model_cfg_status
 * @return none
 */
void Update_Led_Model_Cfg_wifi(C_BYTE model_cfg_status)
{
/*
                        RED         GREEN
Model configured        ---         BLINK
MQTT connected           ON          ON
*/
  if ((led_current_status & LED_STAT_FACT_DEF_A) || (led_current_status & LED_STAT_FACT_DEF_B))
  {
  	//if factory def triggered override the configuration
  	return;
   }

  if ((led_current_status & LED_STAT_MQTT_CONN) && (model_cfg_status==0))
  {
	 Led_Status_Update(LED_ON, LED_RED);
  }		
  else
  {
	 Led_Status_Update(LED_OFF, LED_RED);
  }		
}

/**
 * @brief Update_Led_MQTT_Conn_wifi
 *        This function used to see the led connection of Wifi model
 *
 * @param  C_BYTE mqtt_conn_status
 * @return none
 */
void Update_Led_MQTT_Conn_wifi(C_BYTE mqtt_conn_status)
{
/*
                        RED         GREEN
Model configured        ---         BLINK
MQTT connected           ON          ON
*/
	if (mqtt_conn_status == 1)
	{	
       if (led_current_status & LED_STAT_MODEL_CFG)
	   {
    	   Led_Status_Update(LED_ON, LED_GREEN);
	   }
       else
       {
    	   Led_Status_Update(LED_OFF, LED_GREEN);
       }
	}
	else
	    Led_Status_Update(LED_OFF, LED_GREEN);
}

/**
 * @brief Update_Led_RS485_wifi
 *        right now for the WiFi model there isn't any RS485
 *	 	  activity indicator, so do nothing
 *
 * @param  C_BYTE rs485_status
 * @return none
 */
void Update_Led_RS485_wifi(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
}

/**
 * @brief Update_Led_Fact_Def_A_wifi
 *        This function used to see the led factory default of Wifi model
 *
 * @param  C_BYTE fact_def_a_status
 * @return none
 */
void Update_Led_Fact_Def_A_wifi(C_BYTE fact_def_a_status)
{

	if (fact_def_a_status == 1)
	{
		Led_Status_Update(LED_OFF, LED_RED);
		Led_Status_Update(LED_BLINK_FAST, LED_GREEN);
	}
}

/**
 * @brief Update_Led_Fact_Def_B_wifi
 *        This function used to see the led factory default of Wifi model
 *
 * @param  C_BYTE fact_def_b_status
 * @return none
 */
void Update_Led_Fact_Def_B_wifi(C_BYTE fact_def_b_status)
{
	if (fact_def_b_status == 1)
	{
		Led_Status_Update(LED_OFF, LED_GREEN);
		Led_Status_Update(LED_BLINK_FAST, LED_RED);
	}
}

//#endif


/* ------------------------------------------------------------------- */
/*                          TASK MANAGER                               */
/* ------------------------------------------------------------------- */

/**
 * @brief Led_Pin_Init
 *        This function used to initialize the led pin architecture
 *        depending on the hardware
 *
 * @param  none
 * @return none
 */
void Led_Pin_Init(void){

	if PLATFORM(PLATFORM_DETECTED_BCU){
		LED_GREEN = LED_GREEN_BCU;
	}
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT){
		LED_RED = LED_RED_ESP_WROVER_KIT;
		LED_GREEN = LED_GREEN_ESP_WROVER_KIT;
		LED_BLU = LED_BLU_ESP_WROVER_KIT;
	}
	/* pin is pull down on 2G and pull up on WiFi*/
	if PLATFORM(PLATFORM_DETECTED_2G){
		LED_RED = LED_RED_2G;
		LED_GREEN = LED_GREEN_2G;
		LED_BLU = LED_BLU_2G;
	}
	if PLATFORM(PLATFORM_DETECTED_WIFI){
		LED_RED = LED_RED_WIFI;
		LED_GREEN = LED_GREEN_WIFI;
	}
}

/**
 * @brief Led_init
 *        Main function used to initialize the led pin
 *
 * @param  none
 * @return none
 */
void Led_init(void)
{
    #ifdef INCLUDE_PLATFORM_DEPENDENT

	Led_Pin_Init();

	/* put you I/O initialization here */
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if (LED_RED >= 0){
		gpio_pad_select_gpio(LED_RED);
		gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
		gpio_set_level(LED_RED, LED_PHY_OFF);
	}
	if (LED_GREEN >= 0){
		gpio_pad_select_gpio(LED_GREEN);
		gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
		gpio_set_level(LED_GREEN, LED_PHY_OFF);
	}
	if (LED_BLU >= 0){
		gpio_pad_select_gpio(LED_BLU);
		gpio_set_direction(LED_BLU, GPIO_MODE_OUTPUT);
		gpio_set_level(LED_BLU, LED_PHY_OFF);
	}
#endif

    #endif

}

/**
 * @brief Do_Led_Test_Routine
 *        Led test routine
 *
 * @param  none
 * @return none
 */
void Do_Led_Test_Routine(void){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	C_BYTE count;
	C_BYTE led_test = 3;
	C_UINT16 led_delay = LED_TEST_DELAY;
	if PLATFORM(PLATFORM_DETECTED_2G) {
		led_test = 1;
		led_delay = LED_TEST_DELAY_2G;
	}

	for (count=0; count < led_test; count++) {
		Led_Status_Update(LED_ON, LED_RED);
		Sys__Delay(led_delay);
		Led_Status_Update(LED_OFF, LED_RED);
		Sys__Delay(led_delay);

		Led_Status_Update(LED_ON, LED_GREEN);
		Sys__Delay(led_delay);
		Led_Status_Update(LED_OFF, LED_GREEN);
		Sys__Delay(led_delay);

		Led_Status_Update(LED_ON, LED_BLU);
		Sys__Delay(led_delay);
		Led_Status_Update(LED_OFF, LED_BLU);
		Sys__Delay(led_delay);
	}
#endif

}


/**
 * @brief Update_Led_Model_Cfg
 *        Used inside the task to turn on or off the led cfg
 *
 * @param  C_BYTE model_cfg_status
 *
 * @return none
 */
void Update_Led_Model_Cfg(C_BYTE model_cfg_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_Model_Cfg_bcu(model_cfg_status);
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_Model_Cfg_esp_wrover_kit(model_cfg_status);

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_Model_Cfg_2g(model_cfg_status);

    if PLATFORM(PLATFORM_DETECTED_WIFI)	Update_Led_Model_Cfg_wifi(model_cfg_status);

#endif
}

/**
 * @brief Update_Led_MQTT_Conn
 *        Used inside the task to turn on or off the led MQTT_Conn
 *
 * @param  C_BYTE mqtt_conn_status
 *
 * @return none
 */
void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT
  if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_MQTT_Conn_bcu(mqtt_conn_status);
  if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_MQTT_Conn_esp_wrover_kit(mqtt_conn_status);

  /* pin is pull down on 2G and pull upped on WiFi*/
  if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_MQTT_Conn_2g(mqtt_conn_status);

  if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_MQTT_Conn_wifi(mqtt_conn_status);

#endif
}

/**
 * @brief Update_Led_RS485
 *        Used inside the task to turn on or off the led RS485
 *
 * @param  C_BYTE rs485_status
 *
 * @return none
 */
void Update_Led_RS485(C_BYTE rs485_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_RS485_bcu(rs485_status);
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT)Update_Led_RS485_esp_wrover_kit(rs485_status);

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_RS485_2g(rs485_status);

    if PLATFORM(PLATFORM_DETECTED_WIFI)	Update_Led_RS485_wifi(rs485_status);

#endif
}


/**
 * @brief Update_Led_Fact_Def_A
 *        Used inside the task to turn on or off the led Fact_Def
 *
 * @param  C_BYTE rs485_status
 *
 * @return none
 */
void Update_Led_Fact_Def_A(C_BYTE rs485_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_Fact_Def_A_bcu(rs485_status);
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_Fact_Def_A_esp_wrover_kit(rs485_status);

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_Fact_Def_A_2g(rs485_status);
    if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_Fact_Def_A_wifi(rs485_status);

#endif
}

void Update_Led_Fact_Def_B(C_BYTE rs485_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_Fact_Def_B_bcu(rs485_status);
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_Fact_Def_B_esp_wrover_kit(rs485_status);

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_Fact_Def_B_2g(rs485_status);
    if PLATFORM(PLATFORM_DETECTED_WIFI)	Update_Led_Fact_Def_B_wifi(rs485_status);

#endif
}



/**
 * @brief Led_task  this routine need to be associated to an OS task to update
 * cyclically the leds, or in a megaloop system put in the main loop with some 
 * modifications, take care
 * in this case that the blink rate could be vary.
 * Define this as a low priority task, it only update the led status
 *       
 * @param none
 * @return none
 */
void Led_task(void)
{
	
    Led_init();

    Do_Led_Test_Routine();

    while(1)
    {
		Task_Led_Status();
		/* include a task delay here to leave time for other task */		
        Sys__Delay(500);
    }
}


/**
 * @brief Led_Task_Start this routine launch an OS task to update
 * cyclically the leds, or in a megaloop system this routine is not required 
 * put in the main loop the contente of Led_task
 *
 * @param none
 * @return none
 */
void Led_Task_Start(void)
{
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  xTaskCreate(Led_task, "Led_task", LED_TASK_TASK_STACK_SIZE, NULL, LED_TASK_TASK_PRIO, &xLedTask);
  #endif
}


