/**
 * @file Led_Manager_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the LEDs
 *
 * Led function
 *
 * WiFi Model
 * there is only one Led_Status : 
 * - is off if not connected to an AP. 
 * - blinks 1 time/s if connected to an AP but not to the CAREL MQTT server
 * - is always on if connected to CAREL MQTT server.
 *
 * 
 * GSM Model
 * these reflect the current led names
 * 1.	(WORK)  blinks when GME receives data via RS485  This is useful to detect communication trouble
 * 2.	(GPRS)  is on if the connection with the GSM provider was successfully performed
 * 3.	(LINKA) is on if the connection with CAREL MQTT server was successfully performed
 * 4.	(LINKB) shows the status of the configuration (i.e. model file present).
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
/* ================================================================================ */
/*                        This section is for the WiFi Gateway                      */
/* ================================================================================ */
#ifdef IS_A_WIFI_GATEWAY


#ifdef INCLUDE_PLATFORM_DEPENDENT
#define LED_PHY_ON   1
#define LED_PHY_OFF  0
#endif


/**
 * @brief LED_STATUS_IO 
 *        is platform dependent redefine it to match your I/O schema 
 */
#define LED_STATUS_IO	GPIO_NUM_0

volatile Led_Show_Status_t led_status;

void Led_Task_Start(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	xTaskCreate(Led_task, "Led_task", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
#endif
}

void Led_Status_Update(Led_Show_Status_t status){
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  
 
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  		      	    	
    	  gpio_set_level(LED_STATUS_IO, LED_PHY_OFF);
		  #endif
		  break;
		  
		case LED_ON:  
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  
		  gpio_set_level(LED_STATUS_IO, LED_PHY_ON);
		  #endif		  
		  break;
		  
		  
		case LED_BLINK:
		  /* put the I/O instruction here */
		  
		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {       	     	  		
		     /* put the I/O instruction here */
		     #ifdef INCLUDE_PLATFORM_DEPENDENT	
             gpio_set_level(LED_STATUS_IO, LED_PHY_ON);
			 #endif
             blink_status = LED_ON;
            }
            else
            {
			/* put the I/O instruction here */	
			#ifdef INCLUDE_PLATFORM_DEPENDENT		
            gpio_set_level(LED_STATUS_IO, LED_PHY_OFF);
			#endif
            blink_status = LED_OFF;			
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_INTERVAL;
		  }
		    		  		
          break;				
	}
	
	

}



/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds
 * 
 * @param none
 * @return none
 */

void Led_init(void)
{
	/* put you I/O initialization here */
	
	#ifdef INCLUDE_PLATFORM_DEPENDENT	
	gpio_pad_select_gpio(LED_STATUS_IO);
    gpio_set_direction(LED_STATUS_IO, GPIO_MODE_OUTPUT);	

    /* only for debug on ESP32-WROVER */
    #define LED_GREEN   GPIO_NUM_2
    //#define LED_BLUE    GPIO_NUM_4

    gpio_pad_select_gpio(LED_GREEN);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_GREEN, LED_PHY_OFF);

    //gpio_pad_select_gpio(LED_BLUE);
    //gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);
	//gpio_set_level(LED_BLUE, LED_PHY_OFF);


	#endif

    led_status = LED_OFF;
}

#endif //IS_A_WIFI_GATEWAY



/* ================================================================================ */
/*                        This section is for the GSM Gateway                       */
/* ================================================================================ */
#ifdef IS_A_GSM_GATEWAY
/**
 * @brief LED_WORK_IO / LED_GPRS_IO / LED_LINKA_IO / LED_LINKB_IO
 *        is platform dependent redefine it to match your I/O schema 
 */
#define LED_WORK_IO  	GPIO_NUM_1
#define LED_GPRS_IO  	GPIO_NUM_2
#define LED_LINKA_IO  	GPIO_NUM_3
#define LED_LINKB_IO    GPIO_NUM_4

volatile Led_Show_Status_t led_work;
volatile Led_Show_Status_t led_gprs;
volatile Led_Show_Status_t led_linka;
volatile Led_Show_Status_t led_linkb;

void Led_WORK_Update(Led_Show_Status_t status)
{
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  
 
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  		      	    	
    	  gpio_set_level(LED_WORK_IO, 1);		  
		  #endif
		  break;
		  
		case LED_ON:  
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  
		  gpio_set_level(LED_WORK_IO, 0);
		  #endif		  
		  break;
		  
		  
		case LED_BLINK:
		  /* put the I/O instruction here */
		  
		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {       	     	  
             /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT
             gpio_set_level(LED_WORK_IO, 0);
             #endif
			 
             blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
              #ifdef INCLUDE_PLATFORM_DEPENDENT	
              gpio_set_level(LED_WORK_IO, 1);
			  #endif
			  
              blink_status = LED_OFF;			
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_INTERVAL;
		  }
		    		  		
          break;				
	}
	
	

}


void Led_GPRS_Update(Led_Show_Status_t status)
{
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  
 
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  		      	    	
    	  gpio_set_level(LED_GPRS_IO, 1);		  
		  #endif
		  break;
		  
		case LED_ON:  
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  
		  gpio_set_level(LED_GPRS_IO, 0);
		  #endif		  
		  break;
		  
		  
		case LED_BLINK:
		  /* put the I/O instruction here */
		  
		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {       	     	  
		     /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT
             gpio_set_level(LED_GPRS_IO, 0);
			 #endif
             blink_status = LED_ON;
            }
            else
            { /* put the I/O instruction here */
              #ifdef INCLUDE_PLATFORM_DEPENDENT
              gpio_set_level(LED_GPRS_IO, 1);
			  #endif
              blink_status = LED_OFF;			
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_INTERVAL;
		  }
		    		  		
          break;				
	}
	
}


void Led_LINKA_Update(Led_Show_Status_t status)
{
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
  
 
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  		      	    	
    	  gpio_set_level(LED_LINKA_IO, 1);		  
		  #endif
		  break;
		  
		case LED_ON:  
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  
		  gpio_set_level(LED_LINKA_IO, 0);
		  #endif		  
		  break;
		  
		  
		case LED_BLINK:
		  /* put the I/O instruction here */
		  
		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {       
             /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT		
             gpio_set_level(LED_LINKA_IO, 0);
			 #endif
             blink_status = LED_ON;
            }
            else
            { /* put the I/O instruction here */
              #ifdef INCLUDE_PLATFORM_DEPENDENT
              gpio_set_level(LED_LINKA_IO, 1);
			  #endif
              blink_status = LED_OFF;			
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_INTERVAL;
		  }
		    		  		
          break;				
	}
	
}


void Led_LINKB_Update(Led_Show_Status_t status)
{
  static Led_Show_Status_t blink_status = LED_OFF;
  static C_INT32 blink_timer = 0;
   
    switch (status)
	{
		case LED_OFF:
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  		      	    	
    	  gpio_set_level(LED_LINKB_IO, 1);		  
		  #endif
		  break;
		  
		case LED_ON:  
		  /* put the I/O instruction here */
		  #ifdef INCLUDE_PLATFORM_DEPENDENT		  
		  gpio_set_level(LED_LINKB_IO, 0);
		  #endif		  
		  break;
		  
		case LED_BLINK:
		  /* put the I/O instruction here */
		  
		  if (RTC_Get_UTC_Current_Time() >= blink_timer)
		  {
            if (blink_status == LED_OFF)
            {       
             /* put the I/O instruction here */
             #ifdef INCLUDE_PLATFORM_DEPENDENT		
             gpio_set_level(LED_LINKB_IO, 0);
			 #endif
             blink_status = LED_ON;
            }
            else
            {
			  /* put the I/O instruction here */
              #ifdef INCLUDE_PLATFORM_DEPENDENT	
              gpio_set_level(LED_LINKB_IO, 1);
			  #endif
              blink_status = LED_OFF;			
            }
			blink_timer = RTC_Get_UTC_Current_Time() + LED_BLINK_INTERVAL;
		  }
		    		  		
          break;				
	}
	
}


/**
 * @brief Led_init
 *        Initialize the I/O pin related to the leds
 * 
 * @param none
 * @return none
 */

void Led_init(void)
{
	/* put you I/O initialization here */
	
	#ifdef INCLUDE_PLATFORM_DEPENDENT		
    gpio_pad_select_gpio(LED_WORK_IO);
    gpio_set_direction(LED_WORK_IO, GPIO_MODE_OUTPUT);
	
    gpio_pad_select_gpio(LED_GPRS_IO);
    gpio_set_direction(LED_GPRS_IO, GPIO_MODE_OUTPUT)	
	
    gpio_pad_select_gpio(LED_LINKA_IO);
    gpio_set_direction(LED_LINKA_IO, GPIO_MODE_OUTPUT);
	
    gpio_pad_select_gpio(LED_LINKB_IO);
    gpio_set_direction(LED_LINKB_IO, GPIO_MODE_OUTPUT);
	#endif


    led_work = LED_OFF;
    led_gprs = LED_OFF;
    led_linka= LED_OFF;
    led_linkb= LED_OFF;    
}

#endif //IS_A_GSM_GATEWAY




/**
 * @brief Led_task  this routine need to be associated to an OS task to update
 * cyclically the leds, or in a megaloop system put in the main loop, take care
 * in this case that the blink rate could be vary.
 * Define this as a low priority task, it only update the led status
 *       
 * @param none
 * @return none
 */
void Led_task(void)
{
	
    Led_init();
	
    while(1)
    {
		#ifdef IS_A_WIFI_GATEWAY
		Led_Status_Update(led_status);
		#endif
		
		#ifdef IS_A_GSM_GATEWAY
		Led_WORK_Update(led_work_update); 
		Led_GPRS_Update(led_gprs_update); 
		Led_LINKA_Update(led_linka_update);
		Led_LINKB_Update(led_linkb_update);		
		#endif
		
		/*
		include a task delay here to leave time for other task
		*/
		
        Sys__Delay(1000);
    }
}
