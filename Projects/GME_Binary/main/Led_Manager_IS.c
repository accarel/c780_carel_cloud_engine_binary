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
#include "sdkconfig.h"
#include "esp_log.h"
#endif

#include "RTC_IS.h"
#include "Led_Manager_IS.h"
#include "sys_IS.h"

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

/* ========================================================================== */
/* development platform                                                       */
/* ========================================================================== */
#ifdef __USE_ESP_WROVER_KIT
#define LED_GREEN       GPIO_NUM_2
#define LED_RED    	    GPIO_NUM_0
#define LED_BLU         GPIO_NUM_4
#endif

/* ========================================================================== */
/*  OFFICIAL HW PLATFORM                                                      */
/* ========================================================================== */

/* ========================================================================== */
/*  __USE_USR_WIFI_HW                                                         */
/* ========================================================================== */
#ifdef __USE_USR_WIFI_HW
#define LED_GREEN     GPIO_NUM_27
#define LED_RED       GPIO_NUM_5

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

volatile C_BYTE bicolor_led = LED_BOTH_OFF;
#endif


/* ========================================================================== */
/*  __USE_USR_2G_HW                                                           */
/* ========================================================================== */
#ifdef __USE_USR_2G_HW
#define LED_GREEN     GPIO_NUM_27
#define LED_RED       GPIO_NUM_5
#define LED_BLU       GPIO_NUM_33
#endif

/* ========================================================================== */
/*  __USE_CAREL_BCU_HW                                                        */
/* ========================================================================== */
#ifdef __USE_CAREL_BCU_HW
#define LED_GREEN     GPIO_NUM_27
#endif


/* ========================================================================== */
/*  COMMON PART                                                               */
/* ========================================================================== */


typedef enum{
	LED_OFF        = 0,
	LED_ON         = 1,
	LED_BLINK_SLOW = 2,
	LED_BLINK_FAST = 3 
}Led_Show_Status_t;


//C_BYTE led_configured;
//C_BYTE led_cloud;
//C_BYTE led_rs_485;


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
/* function forward declaration section                                */
/* ------------------------------------------------------------------- */
void Update_Led_Model_Cfg(C_BYTE model_cfg_status);

void Led_Status_Update_Red(Led_Show_Status_t status);
void Led_Status_Update_Green(Led_Show_Status_t status);

void Do_Led_Test_Routine(void);

void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status);
void Update_Led_RS485(C_BYTE rs485_status);


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


void Task_Led_Status(void)
{

    if (led_current_status & LED_STAT_TEST)
	{
		Do_Led_Test_Routine();
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
	
}


/* ------------------------------------------------------------------- */
/*           BEGIN OF PLATFORM DEPENDENT LED MANAGEMENT                */
/* ------------------------------------------------------------------- */

#ifdef __USE_CAREL_BCU_HW

/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 *
 * @param none
 * @return none
 */
void Led_init(void)
{
	/* put you I/O initialization here */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
    gpio_pad_select_gpio(LED_GREEN);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN, LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  break;

		case LED_BLINK_SLOW:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		      /* put the I/O instruction here */
              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
              blink_status = LED_OFF;
            }

			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

         //TODO BILATO refactor to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
             blink_status = LED_ON;
            }
            else
            {
              /* put the I/O instruction here */
              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}


}

void Led_Status_Update_Green(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
          /* to avoid conflict change immediately like a semaphore*/
		  gpio_set_level(LED_GREEN, LED_PHY_ON);
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
      		  gpio_set_level(LED_GREEN, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
			  #endif

              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

        //todo BILATO refactoring to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
		     #ifdef INCLUDE_PLATFORM_DEPENDENT
      		 gpio_set_level(LED_GREEN, LED_PHY_ON);
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT
  		    gpio_set_level(LED_GREEN, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}
}

void Led_Status_Update_Blu(Led_Show_Status_t status){
	  static Led_Show_Status_t blink_status = LED_OFF;
	  static C_INT32 blink_timer = 0;

	    //printf("Status %X\r\n", (uint16_t)status);

	    switch (status)
		{
			case LED_OFF:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
			  #endif
			  break;

			case LED_ON:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
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
	              #endif

	              blink_status = LED_ON;
	            }
	            else
	            {
				  /* put the I/O instruction here */
				  #ifdef INCLUDE_PLATFORM_DEPENDENT
				  #endif

	              blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
			  }

	          break;

	        //todo BILATO refactoring to make code compact
			case LED_BLINK_FAST:
			  /* put the I/O instruction here */

			  if (RTC_Get_UTC_Current_Time() >= blink_timer)
			  {
	            if (blink_status == LED_OFF)
	            {
			     /* put the I/O instruction here */
			     #ifdef INCLUDE_PLATFORM_DEPENDENT
	             #endif

	             blink_status = LED_ON;
	            }
	            else
	            {
				/* put the I/O instruction here */
				#ifdef INCLUDE_PLATFORM_DEPENDENT
				#endif
	            blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
			  }

	          break;
		}
	}


void Update_Led_Model_Cfg(C_BYTE model_cfg_status)
{

  if ((model_cfg_status==1) && (led_current_status & LED_STAT_MQTT_CONN))
  {
   	 Led_Status_Update_Green(LED_ON);
  }

  if ((model_cfg_status==1) && ((led_current_status & LED_STAT_MQTT_CONN)==0))
  {
   	 Led_Status_Update_Green(LED_BLINK_SLOW);
  }

  if ((model_cfg_status==0) && ((led_current_status & LED_STAT_MQTT_CONN)==0))
  {
   	 Led_Status_Update_Green(LED_OFF);
  }

}

void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status)
{
	  if ((mqtt_conn_status==1) && (led_current_status & LED_STAT_MODEL_CFG))
	  {
	   	 Led_Status_Update_Green(LED_ON);
	  }

	  if ((mqtt_conn_status==1) && ((led_current_status & LED_STAT_MODEL_CFG)==0))
	  {
	   	 Led_Status_Update_Green(LED_BLINK_SLOW);
	  }

	  if ((mqtt_conn_status==0) && ((led_current_status & LED_STAT_MODEL_CFG)==0))
	  {
	   	 Led_Status_Update_Green(LED_OFF);
	  }
}

void Update_Led_RS485(C_BYTE rs485_status)
{
/*
 right now for the bCU model there isn't any TTL internal
 activity indicator, so do nothing
*/
}


void Do_Led_Test_Routine(void){
	C_BYTE count;

	for (count=0; count < 3; count++)
	{
	Led_Status_Update_Green(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}

}
#endif



#ifdef __USE_USR_2G_HW

/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 *
 * @param none
 * @return none
 */
void Led_init(void)
{
	/* put you I/O initialization here */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	gpio_pad_select_gpio(LED_RED);
    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_RED, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_GREEN);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_BLU);
    gpio_set_direction(LED_BLU, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_BLU, LED_PHY_OFF);

	#endif
}

void Led_Status_Update_Red(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
    	  gpio_set_level(LED_RED,   LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_RED,   LED_PHY_ON);
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
    		  gpio_set_level(LED_RED, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
		      gpio_set_level(LED_RED, LED_PHY_OFF);
			  #endif

              blink_status = LED_OFF;
            }

			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

         //TODO BILATO refactor to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT
    		 gpio_set_level(LED_RED, LED_PHY_ON);
             #endif
             blink_status = LED_ON;
            }
            else
            {
              /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_RED,   LED_PHY_OFF);
			  #endif
              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}


}

void Led_Status_Update_Green(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
          /* to avoid conflict change immediately like a semaphore*/
		  gpio_set_level(LED_GREEN, LED_PHY_ON);
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
      		  gpio_set_level(LED_GREEN, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
			  #endif

              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

        //todo BILATO refactoring to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
		     #ifdef INCLUDE_PLATFORM_DEPENDENT
      		 gpio_set_level(LED_GREEN, LED_PHY_ON);
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT
  		    gpio_set_level(LED_GREEN, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}
}

void Led_Status_Update_Blu(Led_Show_Status_t status){
	  static Led_Show_Status_t blink_status = LED_OFF;
	  static C_INT32 blink_timer = 0;

	    //printf("Status %X\r\n", (uint16_t)status);

	    switch (status)
		{
			case LED_OFF:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
			  gpio_set_level(LED_BLU, LED_PHY_OFF);
			  #endif
			  break;

			case LED_ON:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
	          /* to avoid conflict change immediately like a semaphore*/
			  gpio_set_level(LED_BLU, LED_PHY_ON);
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
	      		  gpio_set_level(LED_BLU, LED_PHY_ON);
	              #endif

	              blink_status = LED_ON;
	            }
	            else
	            {
				  /* put the I/O instruction here */
				  #ifdef INCLUDE_PLATFORM_DEPENDENT
	  		      gpio_set_level(LED_BLU, LED_PHY_OFF);
				  #endif

	              blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
			  }

	          break;

	        //todo BILATO refactoring to make code compact
			case LED_BLINK_FAST:
			  /* put the I/O instruction here */

			  if (RTC_Get_UTC_Current_Time() >= blink_timer)
			  {
	            if (blink_status == LED_OFF)
	            {
			     /* put the I/O instruction here */
			     #ifdef INCLUDE_PLATFORM_DEPENDENT
	      		 gpio_set_level(LED_BLU, LED_PHY_ON);
	             #endif

	             blink_status = LED_ON;
	            }
	            else
	            {
				/* put the I/O instruction here */
				#ifdef INCLUDE_PLATFORM_DEPENDENT
	  		    gpio_set_level(LED_BLU, LED_PHY_OFF);
				#endif
	            blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
			  }

	          break;
		}
	}

void Update_Led_Model_Cfg(C_BYTE model_cfg_status)
{
/*
                        RED
Model configured         ON
*/
  if (model_cfg_status==1)
  {
   	 Led_Status_Update_Red(LED_ON);
  }
  else
  {
	 Led_Status_Update_Red(LED_OFF);
  }
}

void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status)
{
/*
                        GREEN
MQTT connected          ON
*/
	if (mqtt_conn_status == 1)
	{
      Led_Status_Update_Green(LED_ON);
	}
	else
	{
      Led_Status_Update_Green(LED_OFF);
	}

}

void Update_Led_RS485(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
  if (rs485_status)
  {
	  Led_Status_Update_Blu(LED_ON);
  }
  else
  {
	  Led_Status_Update_Blu(LED_OFF);
  }
}


void Do_Led_Test_Routine(void){
	C_BYTE count;

	for (count=0; count < 3; count++)
	{

	//TODO Bilato metterli in fila e accenderli uno dietro l'altro quando avremo HW
	Led_Status_Update_Red(LED_ON);
    Sys__Delay(LED_TEST_DELAY);
    Led_Status_Update_Red(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Green(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Blu(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Blu(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}

}
#endif

#ifdef __USE_ESP_WROVER_KIT
/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 *
 * @param none
 *  @return none
 */

void Led_init(void)
{
	/* put you I/O initialization here */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	gpio_pad_select_gpio(LED_RED);
    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_RED, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_GREEN);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_BLU);
    gpio_set_direction(LED_BLU, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_BLU, LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
    	  gpio_set_level(LED_RED,   LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_RED,   LED_PHY_ON);
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
    		  gpio_set_level(LED_RED, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
		      gpio_set_level(LED_RED, LED_PHY_OFF);
			  #endif

              blink_status = LED_OFF;
            }

			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

         //TODO BILATO refactor to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT
    		 gpio_set_level(LED_RED, LED_PHY_ON);
             #endif
             blink_status = LED_ON;
            }
            else
            {
              /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_RED,   LED_PHY_OFF);
			  #endif
              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}


}

void Led_Status_Update_Green(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
          /* to avoid conflict change immediately like a semaphore*/
		  gpio_set_level(LED_GREEN, LED_PHY_ON);
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
      		  gpio_set_level(LED_GREEN, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
			  #endif

              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

        //todo BILATO refactoring to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
		     #ifdef INCLUDE_PLATFORM_DEPENDENT
      		 gpio_set_level(LED_GREEN, LED_PHY_ON);
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT
  		    gpio_set_level(LED_GREEN, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}
}

void Led_Status_Update_Blu(Led_Show_Status_t status){
	  static Led_Show_Status_t blink_status = LED_OFF;
	  static C_INT32 blink_timer = 0;

	    //printf("Status %X\r\n", (uint16_t)status);

	    switch (status)
		{
			case LED_OFF:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
			  gpio_set_level(LED_BLU, LED_PHY_OFF);
			  #endif
			  break;

			case LED_ON:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
	          /* to avoid conflict change immediately like a semaphore*/
			  gpio_set_level(LED_BLU, LED_PHY_ON);
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
	      		  gpio_set_level(LED_BLU, LED_PHY_ON);
	              #endif

	              blink_status = LED_ON;
	            }
	            else
	            {
				  /* put the I/O instruction here */
				  #ifdef INCLUDE_PLATFORM_DEPENDENT
	  		      gpio_set_level(LED_BLU, LED_PHY_OFF);
				  #endif

	              blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
			  }

	          break;

	        //todo BILATO refactoring to make code compact
			case LED_BLINK_FAST:
			  /* put the I/O instruction here */

			  if (RTC_Get_UTC_Current_Time() >= blink_timer)
			  {
	            if (blink_status == LED_OFF)
	            {
			     /* put the I/O instruction here */
			     #ifdef INCLUDE_PLATFORM_DEPENDENT
	      		 gpio_set_level(LED_BLU, LED_PHY_ON);
	             #endif

	             blink_status = LED_ON;
	            }
	            else
	            {
				/* put the I/O instruction here */
				#ifdef INCLUDE_PLATFORM_DEPENDENT
	  		    gpio_set_level(LED_BLU, LED_PHY_OFF);
				#endif
	            blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
			  }

	          break;
		}
	}

void Update_Led_Model_Cfg(C_BYTE model_cfg_status)
{
/*
                        RED
Model configured         ON
*/
  if (model_cfg_status==1)
  {
   	 Led_Status_Update_Red(LED_ON);
  }
  else
  {
	 Led_Status_Update_Red(LED_OFF);
  }
}

void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status)
{
/*
                        GREEN
MQTT connected          ON
*/
	if (mqtt_conn_status == 1)
	{
      Led_Status_Update_Green(LED_ON);
	}
	else
	{
      Led_Status_Update_Green(LED_OFF);
	}

}

void Update_Led_RS485(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
  if (rs485_status)
  {
	  Led_Status_Update_Blu(LED_ON);
  }
  else
  {
	  Led_Status_Update_Blu(LED_OFF);
  }
}

void Do_Led_Test_Routine(void){
	C_BYTE count;

	for (count=0; count < 3; count++)
	{
	Led_Status_Update_Red(LED_ON);
    Sys__Delay(LED_TEST_DELAY);
    Led_Status_Update_Red(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Green(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Blu(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Blu(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}

}
#endif





#ifdef __USE_USR_WIFI_HW
/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 * 
 * @param none
 * @return none
 */
void Led_init(void)
{
	/* put you I/O initialization here */	
	#ifdef INCLUDE_PLATFORM_DEPENDENT	
	gpio_pad_select_gpio(LED_RED);
    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_RED, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_GREEN);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN, LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN, LED_PHY_OFF);
    	  gpio_set_level(LED_RED,   LED_PHY_OFF);

    	  /* to avoid conflict change at the end*/
    	  bicolor_led = LED_BOTH_OFF;
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT

		  if (bicolor_led == LED_BOTH_OFF)
		  {
			/* to avoid conflict change immediately like a semaphore*/
		    bicolor_led = LED_RED_ON;

		    gpio_set_level(LED_GREEN, LED_PHY_OFF);
		    gpio_set_level(LED_RED,   LED_PHY_ON);
		  }
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
      		 if (bicolor_led == LED_BOTH_OFF)
      		 {
      			/* to avoid conflict change immediately like a semaphore*/
      		    bicolor_led = LED_RED_ON;

      		    gpio_set_level(LED_GREEN, LED_PHY_OFF);
      		    gpio_set_level(LED_RED,   LED_PHY_ON);
      		  }
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT

  		    if (bicolor_led == LED_RED_ON)
  		    {
  			  /* to avoid conflict change immediately like a semaphore*/
  		      bicolor_led = LED_BOTH_OFF;

  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
  		      gpio_set_level(LED_RED,   LED_PHY_OFF);
  		    }

			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

         //TODO BILATO refactor to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
		     #ifdef INCLUDE_PLATFORM_DEPENDENT
      		 if (bicolor_led == LED_BOTH_OFF)
      		 {
      			/* to avoid conflict change immediately like a semaphore*/
      		    bicolor_led = LED_RED_ON;

      		    gpio_set_level(LED_GREEN, LED_PHY_OFF);
      		    gpio_set_level(LED_RED,   LED_PHY_ON);
      		  }
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT

  		    if (bicolor_led == LED_RED_ON)
  		    {
  			  /* to avoid conflict change immediately like a semaphore*/
  		      bicolor_led = LED_BOTH_OFF;

  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
  		      gpio_set_level(LED_RED,   LED_PHY_OFF);
  		    }

			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}


}

void Led_Status_Update_Green(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  
    //printf("Status %X\r\n", (uint16_t)status);
 
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN, LED_PHY_OFF);
    	  gpio_set_level(LED_RED,   LED_PHY_OFF);

    	  /* to avoid conflict change at the end*/
    	  bicolor_led = LED_BOTH_OFF;
		  #endif
		  break;
		  
		case LED_ON:  
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  

		  if (bicolor_led == LED_BOTH_OFF)
		  {
			/* to avoid conflict change immediately like a semaphore*/
		    bicolor_led = LED_GREEN_ON;

		    gpio_set_level(LED_GREEN, LED_PHY_ON);
		    gpio_set_level(LED_RED,   LED_PHY_OFF);
		  }
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
      		 if (bicolor_led == LED_BOTH_OFF)
      		 {
      			/* to avoid conflict change immediately like a semaphore*/
      		    bicolor_led = LED_GREEN_ON;

      		    gpio_set_level(LED_GREEN, LED_PHY_ON);
      		    gpio_set_level(LED_RED,   LED_PHY_OFF);
      		  }
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */	
			#ifdef INCLUDE_PLATFORM_DEPENDENT		

  		    if (bicolor_led == LED_GREEN)
  		    {
  			  /* to avoid conflict change immediately like a semaphore*/
  		      bicolor_led = LED_BOTH_OFF;

  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
  		      gpio_set_level(LED_RED,   LED_PHY_OFF);
  		    }

			#endif
            blink_status = LED_OFF;			
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_SLOW_INTERVAL;
		  }

          break;

        //todo BILATO refactoring to make code compact
		case LED_BLINK_FAST:
		  /* put the I/O instruction here */

		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {
		     /* put the I/O instruction here */
		     #ifdef INCLUDE_PLATFORM_DEPENDENT
      		 if (bicolor_led == LED_BOTH_OFF)
      		 {
      			/* to avoid conflict change immediately like a semaphore*/
      		    bicolor_led = LED_GREEN_ON;

      		    gpio_set_level(LED_GREEN, LED_PHY_ON);
      		    gpio_set_level(LED_RED,   LED_PHY_OFF);
      		  }
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT

  		    if (bicolor_led == LED_GREEN)
  		    {
  			  /* to avoid conflict change immediately like a semaphore*/
  		      bicolor_led = LED_BOTH_OFF;

  		      gpio_set_level(LED_GREEN, LED_PHY_OFF);
  		      gpio_set_level(LED_RED,   LED_PHY_OFF);
  		    }

			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }
		    		  		
          break;				

	}

}

void Update_Led_Model_Cfg(C_BYTE model_cfg_status)
{
/*
                        RED         GREEN
Model configured        ---         BLINK
MQTT connected           ON          ON
*/
  if ((led_current_status & LED_STAT_MQTT_CONN) && (model_cfg_status==0))
  {
   	 Led_Status_Update_Red(LED_ON);
  }		
  else
  {
	 Led_Status_Update_Red(LED_OFF);
  }		
}

void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status)
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
		   Led_Status_Update_Green(LED_ON);
	   }		
	}
	else
	{
	       if (led_current_status & LED_STAT_MODEL_CFG)
		   {
			   Led_Status_Update_Green(LED_BLINK_SLOW);
		   }
	       else
	       {
		     Led_Status_Update_Green(LED_OFF);
	       }
	}
	
}

void Update_Led_RS485(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
}

void Do_Led_Test_Routine(void){
	C_BYTE count;

	for (count=0; count < 3; count++)
	{
	Led_Status_Update_Red(LED_ON);
    Sys__Delay(LED_TEST_DELAY);
    Led_Status_Update_Red(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Green(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}

}

#endif






/* ------------------------------------------------------------------- */
/*                          TASK MANAGER                               */
/* ------------------------------------------------------------------- */

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
 * @param none
 * @return none
 */
void Led_Task_Start(void)
{
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  xTaskCreate(Led_task, "Led_task", LED_TASK_TASK_STACK_SIZE, NULL, LED_TASK_TASK_PRIO, &xLedTask);
  #endif
}


