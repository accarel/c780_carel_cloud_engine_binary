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

volatile C_BYTE bicolor_led = LED_BOTH_OFF;



/* ========================================================================== */
/*  __USE_USR_2G_HW                                                           */
/* ========================================================================== */
/* used to avoid mistake in the association in the 2G model WYSIWYG */
#define WORK_LED(a) Led_Status_Update_Green_2g(a)
#define LINK_A(a)   Led_Status_Update_Red_2g(a)
#define LINK_B(a)   Led_Status_Update_Blu_2g(a)


/* ========================================================================== */
/*  __USE_CAREL_BCU_HW                                                        */
/* ========================================================================== */



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


/* contain the current status of the led used to restore
 * previous condition if needed */
C_BYTE red_led_current_status;
C_BYTE green_led_current_status;
C_BYTE blu_led_current_status;


#ifdef INCLUDE_PLATFORM_DEPENDENT
#define LED_TASK_TASK_STACK_SIZE   (1024)
//configMINIMAL_STACK_SIZE
#define LED_TASK_TASK_PRIO         tskIDLE_PRIORITY
static xTaskHandle xLedTask;
#endif


/* ------------------------------------------------------------------- */
/* function forward declaration section                                */
/* ------------------------------------------------------------------- */
void Led_Status_Update_Red(Led_Show_Status_t status);
void Led_Status_Update_Green(Led_Show_Status_t status);
void Led_Status_Update_Blu(Led_Show_Status_t status);

void Do_Led_Test_Routine(void);
void Update_Led_Model_Cfg(C_BYTE model_cfg_status);
void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status);
void Update_Led_RS485(C_BYTE rs485_status);
void Update_Led_Fact_Def_A(C_BYTE fact_def_a_status);
void Update_Led_Fact_Def_B(C_BYTE fact_def_b_status);



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


/* ------------------------------------------------------------------- */
/*           BEGIN OF PLATFORM DEPENDENT LED MANAGEMENT                */
/* ------------------------------------------------------------------- */

//#ifdef __USE_CAREL_BCU_HW

/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 *
 * @param none
 * @return none
 */
void Led_init_bcu(void)
{
	/* put you I/O initialization here */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
    gpio_pad_select_gpio(LED_GREEN_BCU);
    gpio_set_direction(LED_GREEN_BCU, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN_BCU, LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red_bcu(Led_Show_Status_t status){
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

    red_led_current_status = status;
}

void Led_Status_Update_Green_bcu(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN_BCU, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
          /* to avoid conflict change immediately like a semaphore*/
		  gpio_set_level(LED_GREEN_BCU, LED_PHY_ON);
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
      		  gpio_set_level(LED_GREEN_BCU, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_GREEN_BCU, LED_PHY_OFF);
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
      		 gpio_set_level(LED_GREEN_BCU, LED_PHY_ON);
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT
  		    gpio_set_level(LED_GREEN_BCU, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}

    green_led_current_status = status;
}

void Led_Status_Update_Blu_bcu(Led_Show_Status_t status){
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

    blu_led_current_status = status;

  }

void Update_Led_Model_Cfg_bcu(C_BYTE model_cfg_status)
{

  if ((model_cfg_status==1) && (led_current_status & LED_STAT_MQTT_CONN))
  {
   	 Led_Status_Update_Green_bcu(LED_ON);
  }

  if ((model_cfg_status==1) && ((led_current_status & LED_STAT_MQTT_CONN)==0))
  {
   	 Led_Status_Update_Green_bcu(LED_BLINK_SLOW);
  }

  if ((model_cfg_status==0) && ((led_current_status & LED_STAT_MQTT_CONN)==0))
  {
   	 Led_Status_Update_Green_bcu(LED_OFF);
  }

}

void Update_Led_MQTT_Conn_bcu(C_BYTE mqtt_conn_status)
{
	  if ((mqtt_conn_status==1) && (led_current_status & LED_STAT_MODEL_CFG))
	  {
	   	 Led_Status_Update_Green_bcu(LED_ON);
	  }

	  if ((mqtt_conn_status==1) && ((led_current_status & LED_STAT_MODEL_CFG)==0))
	  {
	   	 Led_Status_Update_Green_bcu(LED_BLINK_SLOW);
	  }

	  if ((mqtt_conn_status==0) && ((led_current_status & LED_STAT_MODEL_CFG)==0))
	  {
	   	 Led_Status_Update_Green_bcu(LED_OFF);
	  }
}

void Update_Led_RS485_bcu(C_BYTE rs485_status)
{
/*
 right now for the bCU model there isn't any TTL internal
 activity indicator, so do nothing
*/
}

void Update_Led_Fact_Def_A_bcu(C_BYTE fact_def_a_status)
{
//do nothing no led available
}

void Update_Led_Fact_Def_B_bcu(C_BYTE fact_def_a_status)
{
//do nothing no led available
}

void Do_Led_Test_Routine_bcu(void){
	C_BYTE count;

	for (count=0; count < 3; count++)
	{
	Led_Status_Update_Green_bcu(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green_bcu(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}

}

//#endif



//#ifdef __USE_USR_2G_HW

/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 *
 * @param none
 * @return none
 */
void Led_init_2g(void)
{
	/* put you I/O initialization here */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	gpio_pad_select_gpio(LED_RED_2G);
    gpio_set_direction(LED_RED_2G, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_RED_2G, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_GREEN_2G);
    gpio_set_direction(LED_GREEN_2G, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN_2G, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_BLU_2G);
    gpio_set_direction(LED_BLU_2G, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_BLU_2G, LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red_2g(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
    	  gpio_set_level(LED_RED_2G, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_RED_2G, LED_PHY_ON);
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
    		  gpio_set_level(LED_RED_2G, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
		      gpio_set_level(LED_RED_2G, LED_PHY_OFF);
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
    		 gpio_set_level(LED_RED_2G, LED_PHY_ON);
             #endif
             blink_status = LED_ON;
            }
            else
            {
              /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_RED_2G,   LED_PHY_OFF);
			  #endif
              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}

    red_led_current_status = status;
}

void Led_Status_Update_Green_2g(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN_2G, LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
          /* to avoid conflict change immediately like a semaphore*/
		  gpio_set_level(LED_GREEN_2G, LED_PHY_ON);
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
      		  gpio_set_level(LED_GREEN_2G, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_GREEN_2G, LED_PHY_OFF);
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
      		 gpio_set_level(LED_GREEN_2G, LED_PHY_ON);
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT
  		    gpio_set_level(LED_GREEN_2G, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}
    green_led_current_status = status;
}

void Led_Status_Update_Blu_2g(Led_Show_Status_t status){
	  static Led_Show_Status_t blink_status = LED_OFF;
	  static C_INT32 blink_timer = 0;

	    //printf("Status %X\r\n", (uint16_t)status);

	    switch (status)
		{
			case LED_OFF:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
			  gpio_set_level(LED_BLU_2G, LED_PHY_OFF);
			  #endif
			  break;

			case LED_ON:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
	          /* to avoid conflict change immediately like a semaphore*/
			  gpio_set_level(LED_BLU_2G, LED_PHY_ON);
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
	      		  gpio_set_level(LED_BLU_2G, LED_PHY_ON);
	              #endif

	              blink_status = LED_ON;
	            }
	            else
	            {
				  /* put the I/O instruction here */
				  #ifdef INCLUDE_PLATFORM_DEPENDENT
	  		      gpio_set_level(LED_BLU_2G, LED_PHY_OFF);
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
	      		 gpio_set_level(LED_BLU_2G, LED_PHY_ON);
	             #endif

	             blink_status = LED_ON;
	            }
	            else
	            {
				/* put the I/O instruction here */
				#ifdef INCLUDE_PLATFORM_DEPENDENT
	  		    gpio_set_level(LED_BLU_2G, LED_PHY_OFF);
				#endif
	            blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
			  }

	          break;
		}

	    blu_led_current_status  = status;
	}

void Update_Led_Model_Cfg_2g(C_BYTE model_cfg_status)
{
/*
                        RED
Model configured         ON
*/
  if (model_cfg_status==1)
  {
   	 Led_Status_Update_Red_2g(LED_ON);
  }
  else
  {
	 Led_Status_Update_Red_2g(LED_OFF);
  }
}

void Update_Led_MQTT_Conn_2g(C_BYTE mqtt_conn_status)
{
/*
                        GREEN
MQTT connected          ON
*/
	if (mqtt_conn_status == 1)
	{
      Led_Status_Update_Green_2g(LED_ON);
	}
	else
	{
      Led_Status_Update_Green_2g(LED_OFF);
	}

}

void Update_Led_RS485_2g(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
  if (rs485_status)
  {
	  Led_Status_Update_Blu_2g(LED_ON);
  }
  else
  {
	  Led_Status_Update_Blu_2g(LED_OFF);
  }
}

void Update_Led_Fact_Def_A_2g(C_BYTE fact_def_a_status)
{
	if (fact_def_a_status == 1)
	{
		WORK_LED(LED_BLINK_SLOW);
	}
	else
	{
		WORK_LED(green_led_current_status);
	}
}

void Update_Led_Fact_Def_B_2g(C_BYTE fact_def_a_status)
{
	if (fact_def_a_status == 1)
	{
		WORK_LED(LED_BLINK_FAST);
	}
	else
	{
		WORK_LED(green_led_current_status);
	}
}

void Do_Led_Test_Routine_2g(void){

	WORK_LED(LED_ON);
    Sys__Delay(LED_TEST_DELAY);
    LINK_A(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	LINK_B(LED_ON);
	Sys__Delay(LED_TEST_DELAY);

	WORK_LED(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	LINK_A(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	LINK_B(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
}

//
//#endif


//#ifdef __USE_ESP_WROVER_KIT
/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 *
 * @param none
 *  @return none
 */

void Led_init_esp_wrover_kit(void)
{
	/* put you I/O initialization here */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	gpio_pad_select_gpio(LED_RED_ESP_WROVER_KIT );
    gpio_set_direction(LED_RED_ESP_WROVER_KIT , GPIO_MODE_OUTPUT);
    gpio_set_level(LED_RED_ESP_WROVER_KIT , LED_PHY_OFF);

    gpio_pad_select_gpio(LED_GREEN_ESP_WROVER_KIT);
    gpio_set_direction(LED_GREEN_ESP_WROVER_KIT , GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN_ESP_WROVER_KIT , LED_PHY_OFF);

    gpio_pad_select_gpio(LED_BLU_ESP_WROVER_KIT );
    gpio_set_direction(LED_BLU_ESP_WROVER_KIT , GPIO_MODE_OUTPUT);
	gpio_set_level(LED_BLU_ESP_WROVER_KIT , LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red_esp_wrover_kit(Led_Show_Status_t status){
#ifdef __USE_ESP_WROVER_KIT
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
    	  gpio_set_level(LED_RED_ESP_WROVER_KIT ,   LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_RED_ESP_WROVER_KIT ,   LED_PHY_ON);
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
    		  gpio_set_level(LED_RED_ESP_WROVER_KIT , LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
		      gpio_set_level(LED_RED_ESP_WROVER_KIT , LED_PHY_OFF);
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
    		 gpio_set_level(LED_RED_ESP_WROVER_KIT , LED_PHY_ON);
             #endif
             blink_status = LED_ON;
            }
            else
            {
              /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_RED_ESP_WROVER_KIT,   LED_PHY_OFF);
			  #endif
              blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}
    red_led_current_status = status;
#endif
}

void Led_Status_Update_Green_esp_wrover_kit(Led_Show_Status_t status){
#ifdef __USE_ESP_WROVER_KIT
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    if ((led_current_status & LED_STAT_FACT_DEF_A) || (led_current_status & LED_STAT_FACT_DEF_B))
    {
    	//if factory def triggered override the configuration
    	return;
    }

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN_ESP_WROVER_KIT , LED_PHY_OFF);
		  #endif
		  break;

		case LED_ON:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
          /* to avoid conflict change immediately like a semaphore*/
		  gpio_set_level(LED_GREEN_ESP_WROVER_KIT, LED_PHY_ON);
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
      		  gpio_set_level(LED_GREEN_ESP_WROVER_KIT, LED_PHY_ON);
              #endif

              blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
  		      gpio_set_level(LED_GREEN_ESP_WROVER_KIT, LED_PHY_OFF);
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
      		 gpio_set_level(LED_GREEN_ESP_WROVER_KIT, LED_PHY_ON);
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT
  		    gpio_set_level(LED_GREEN_ESP_WROVER_KIT, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}

    green_led_current_status = status;
#endif
}

void Led_Status_Update_Blu_esp_wrover_kit(Led_Show_Status_t status){

#ifdef __USE_ESP_WROVER_KIT

	  static Led_Show_Status_t blink_status = LED_OFF;
	  static C_INT32 blink_timer = 0;

	    //printf("Status %X\r\n", (uint16_t)status);

	    switch (status)
		{
			case LED_OFF:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
			  gpio_set_level(LED_BLU_ESP_WROVER_KIT, LED_PHY_OFF);
			  #endif
			  break;

			case LED_ON:
			  /* put the I/O instruction here */
			  #ifdef INCLUDE_PLATFORM_DEPENDENT
	          /* to avoid conflict change immediately like a semaphore*/
			  gpio_set_level(LED_BLU_ESP_WROVER_KIT, LED_PHY_ON);
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
	      		  gpio_set_level(LED_BLU_ESP_WROVER_KIT, LED_PHY_ON);
	              #endif

	              blink_status = LED_ON;
	            }
	            else
	            {
				  /* put the I/O instruction here */
				  #ifdef INCLUDE_PLATFORM_DEPENDENT
	  		      gpio_set_level(LED_BLU_ESP_WROVER_KIT, LED_PHY_OFF);
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
	      		 gpio_set_level(LED_BLU_ESP_WROVER_KIT, LED_PHY_ON);
	             #endif

	             blink_status = LED_ON;
	            }
	            else
	            {
				/* put the I/O instruction here */
				#ifdef INCLUDE_PLATFORM_DEPENDENT
	  		    gpio_set_level(LED_BLU_ESP_WROVER_KIT, LED_PHY_OFF);
				#endif
	            blink_status = LED_OFF;
	            }
				blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
			  }

	          break;
		}

	    blu_led_current_status = status;
#endif
	}

void Update_Led_Model_Cfg_esp_wrover_kit(C_BYTE model_cfg_status)
{
#ifdef __USE_ESP_WROVER_KIT
/*
                         RED
Model configured         ON
*/

  if (model_cfg_status==1)
  {
   	 Led_Status_Update_Red_esp_wrover_kit(LED_ON);
  }
  else
  {
	 Led_Status_Update_Red_esp_wrover_kit(LED_OFF);
  }
#endif
}

void Update_Led_MQTT_Conn_esp_wrover_kit(C_BYTE mqtt_conn_status)
{
#ifdef __USE_ESP_WROVER_KIT
/*
                        GREEN
MQTT connected          ON
*/
	if (mqtt_conn_status == 1)
	{
      Led_Status_Update_Green_esp_wrover_kit(LED_ON);
	}
	else
	{
      Led_Status_Update_Green_esp_wrover_kit(LED_OFF);
	}
#endif
}

void Update_Led_RS485_esp_wrover_kit(C_BYTE rs485_status)
{
#ifdef __USE_ESP_WROVER_KIT
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
  if (rs485_status)
  {
	  Led_Status_Update_Blu_esp_wrover_kit(LED_ON);
  }
  else
  {
	  Led_Status_Update_Blu_esp_wrover_kit(LED_OFF);
  }
#endif
}

void Update_Led_Fact_Def_A_esp_wrover_kit(C_BYTE fact_def_a_status)
{
#ifdef __USE_ESP_WROVER_KIT
	if (fact_def_a_status == 1)
	{
		Led_Status_Update_Red_esp_wrover_kit(LED_BLINK_SLOW);
	}
	else
	{
		Led_Status_Update_Red_esp_wrover_kit(red_led_current_status);
	}
#endif
}

void Update_Led_Fact_Def_B_esp_wrover_kit(C_BYTE fact_def_b_status)
{
#ifdef __USE_ESP_WROVER_KIT
	if (fact_def_b_status == 1)
	{
		Led_Status_Update_Red_esp_wrover_kit(LED_BLINK_FAST);
	}
	else
	{
		Led_Status_Update_Red_esp_wrover_kit(red_led_current_status);
	}
#endif
}

void Do_Led_Test_Routine_esp_wrover_kit(void){
#ifdef 	__USE_ESP_WROVER_KIT
	C_BYTE count;

	for (count=0; count < 3; count++)
	{
	Led_Status_Update_Red_esp_wrover_kit(LED_ON);
    Sys__Delay(LED_TEST_DELAY);
    Led_Status_Update_Red_esp_wrover_kit(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Green_esp_wrover_kit(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green_esp_wrover_kit(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Blu_esp_wrover_kit(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Blu_esp_wrover_kit(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}
#endif
}


//#endif





//#ifdef __USE_USR_WIFI_HW
/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds for a specific platform
 * 
 * @param none
 * @return none
 */
void Led_init_wifi(void)
{
	/* put you I/O initialization here */	
	#ifdef INCLUDE_PLATFORM_DEPENDENT	
	gpio_pad_select_gpio(LED_RED_WIFI);
    gpio_set_direction(LED_RED_WIFI, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_RED_WIFI, LED_PHY_OFF);

    gpio_pad_select_gpio(LED_GREEN_WIFI);
    gpio_set_direction(LED_GREEN_WIFI, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
	#endif
}

void Led_Status_Update_Red_wifi(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;

    //printf("Status %X\r\n", (uint16_t)status);

    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
    	  gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);

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

		    gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
		    gpio_set_level(LED_RED_WIFI,   LED_PHY_ON);
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

      		    gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
      		    gpio_set_level(LED_RED_WIFI,   LED_PHY_ON);
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

  		      gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
  		      gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
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

      		    gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
      		    gpio_set_level(LED_RED_WIFI,   LED_PHY_ON);
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

  		      gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
  		      gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
  		    }

			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }

          break;
	}

    red_led_current_status = status;
}

void Led_Status_Update_Green_wifi(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  
    //printf("Status %X\r\n", (uint16_t)status);
 
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT
		  gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
    	  gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);

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

		    gpio_set_level(LED_GREEN_WIFI, LED_PHY_ON);
		    gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
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

      		    gpio_set_level(LED_GREEN_WIFI, LED_PHY_ON);
      		    gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
      		  }
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */	
			#ifdef INCLUDE_PLATFORM_DEPENDENT		

  		    if (bicolor_led == LED_GREEN_ON)
  		    {
  			  /* to avoid conflict change immediately like a semaphore*/
  		      bicolor_led = LED_BOTH_OFF;

  		      gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
  		      gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
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

      		    gpio_set_level(LED_GREEN_WIFI, LED_PHY_ON);
      		    gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
      		  }
             #endif

             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */
			#ifdef INCLUDE_PLATFORM_DEPENDENT

  		    if (bicolor_led == LED_GREEN_ON)
  		    {
  			  /* to avoid conflict change immediately like a semaphore*/
  		      bicolor_led = LED_BOTH_OFF;

  		      gpio_set_level(LED_GREEN_WIFI, LED_PHY_OFF);
  		      gpio_set_level(LED_RED_WIFI,   LED_PHY_OFF);
  		    }

			#endif
            blink_status = LED_OFF;
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_FAST_INTERVAL;
		  }
		    		  		
          break;				

	}

    green_led_current_status = status;

}

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
   	 Led_Status_Update_Red_wifi(LED_ON);
  }		
  else
  {
	 Led_Status_Update_Red_wifi(LED_OFF);
  }		
}

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
		   Led_Status_Update_Green_wifi(LED_ON);
	   }		
	}
	else
	{
	       if (led_current_status & LED_STAT_MODEL_CFG)
		   {
			   Led_Status_Update_Green_wifi(LED_BLINK_SLOW);
		   }
	       else
	       {
		     Led_Status_Update_Green_wifi(LED_OFF);
	       }
	}
	
}

void Update_Led_RS485_wifi(C_BYTE rs485_status)
{
/*
 right now for the WiFi model there isn't any RS485
 activity indicator, so do nothing
*/
}

void Update_Led_Fact_Def_A_wifi(C_BYTE fact_def_a_status)
{

	if (fact_def_a_status == 1)
	{
  	 Led_Status_Update_Red_wifi(LED_BLINK_SLOW);
	}
	else
	{
	  Led_Status_Update_Red_wifi(red_led_current_status);
	}

}

void Update_Led_Fact_Def_B_wifi(C_BYTE fact_def_b_status)
{
	if (fact_def_b_status == 1)
	{
  	  Led_Status_Update_Red_wifi(LED_BLINK_FAST);
	}
	else
	{
	  Led_Status_Update_Red_wifi(red_led_current_status);
	}

}

void Do_Led_Test_Routine_wifi(void){
	C_BYTE count;

	for (count=0; count < 3; count++)
	{
	Led_Status_Update_Red_wifi(LED_ON);
    Sys__Delay(LED_TEST_DELAY);
    Led_Status_Update_Red_wifi(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);

	Led_Status_Update_Green_wifi(LED_ON);
	Sys__Delay(LED_TEST_DELAY);
	Led_Status_Update_Green_wifi(LED_OFF);
	Sys__Delay(LED_TEST_DELAY);
	}

}

//#endif


/* ------------------------------------------------------------------- */
/*                          TASK MANAGER                               */
/* ------------------------------------------------------------------- */


void Led_init(void)
{
    #ifdef INCLUDE_PLATFORM_DEPENDENT

    #ifdef  __USE_CAREL_BCU_HW
	if PLATFORM(PLATFORM_DETECTED_BCU) Led_init_bcu();
    #else

	#ifdef _ESP_WROVER_KIT
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Led_init_esp_wrover_kit();
    #endif

	/* pin is pull down on 2G and pull upped on WiFi*/
	if PLATFORM(PLATFORM_DETECTED_2G) Led_init_2g();
	if PLATFORM(PLATFORM_DETECTED_WIFI) Led_init_wifi();

    #endif

    #endif

}

void Do_Led_Test_Routine(void){
#ifdef INCLUDE_PLATFORM_DEPENDENT

   #ifdef  __USE_CAREL_BCU_HW
	if PLATFORM(PLATFORM_DETECTED_BCU) Do_Led_Test_Routine_bcu();
   #else

    #ifdef _ESP_WROVER_KIT
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Do_Led_Test_Routine_esp_wrover_kit();
    #endif

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Do_Led_Test_Routine_2g();
    if PLATFORM(PLATFORM_DETECTED_WIFI) Do_Led_Test_Routine_wifi();
   #endif

#endif

}


void Update_Led_Model_Cfg(C_BYTE model_cfg_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT

  #ifdef  __USE_CAREL_BCU_HW
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_Model_Cfg_bcu(model_cfg_status);
  #else

    #ifdef _ESP_WROVER_KIT
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_Model_Cfg_esp_wrover_kit(model_cfg_status);
    #endif

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_Model_Cfg_2g(model_cfg_status);
    if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_Model_Cfg_wifi(model_cfg_status);
#endif

#endif
}

void Update_Led_MQTT_Conn(C_BYTE mqtt_conn_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT

  #ifdef  __USE_CAREL_BCU_HW
  if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_MQTT_Conn_bcu(mqtt_conn_status);
  #else __USE_ESP_WROVER_KIT

  #ifdef _ESP_WROVER_KIT
  if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_MQTT_Conn_esp_wrover_kit(mqtt_conn_status);
  #endif

  /* pin is pull down on 2G and pull upped on WiFi*/
  if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_MQTT_Conn_2g(mqtt_conn_status);
  if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_MQTT_Conn_wifi(mqtt_conn_status);
  #endif

#endif
}

void Update_Led_RS485(C_BYTE rs485_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT

    #ifdef  __USE_CAREL_BCU_HW
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_RS485_bcu(rs485_status);

	#else

	#ifdef _ESP_WROVER_KIT
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT)Update_Led_RS485_esp_wrover_kit(rs485_status);
    #endif

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_RS485_2g(rs485_status);
    if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_RS485_wifi(rs485_status);

    #endif

#endif
}



void Update_Led_Fact_Def_A(C_BYTE rs485_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT

    #ifdef  __USE_CAREL_BCU_HW
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_Fact_Def_A_bcu(rs485_status);

	#else

	#ifdef _ESP_WROVER_KIT
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_Fact_Def_A_esp_wrover_kit(rs485_status);
    #endif

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_Fact_Def_A_2g(rs485_status);
    if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_Fact_Def_A_wifi(rs485_status);

    #endif

#endif
}

void Update_Led_Fact_Def_B(C_BYTE rs485_status){
#ifdef INCLUDE_PLATFORM_DEPENDENT

    #ifdef  __USE_CAREL_BCU_HW
	if PLATFORM(PLATFORM_DETECTED_BCU) Update_Led_Fact_Def_B_bcu(rs485_status);

	#else

	#ifdef _ESP_WROVER_KIT
	if PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) Update_Led_Fact_Def_B_esp_wrover_kit(rs485_status);
    #endif

    /* pin is pull down on 2G and pull upped on WiFi*/
    if PLATFORM(PLATFORM_DETECTED_2G) Update_Led_Fact_Def_B_2g(rs485_status);
    if PLATFORM(PLATFORM_DETECTED_WIFI) Update_Led_Fact_Def_B_wifi(rs485_status);

    #endif

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
 * @param none
 * @return none
 */
void Led_Task_Start(void)
{
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  xTaskCreate(Led_task, "Led_task", LED_TASK_TASK_STACK_SIZE, NULL, LED_TASK_TASK_PRIO, &xLedTask);
  #endif
}


