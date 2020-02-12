/**
 * @file Led_Manager_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the LEDs
 */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_MANAGER_H
#define __LED_MANAGER_H


/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "CAREL_GLOBAL_DEF.h"

/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */
/**
 * @brief _DEBUG_LEDS enable the RGB leds on ESP-WROVER
 */
#ifdef __CCL_DEBUG_MODE
#define _DEBUG_LEDS
#endif

typedef enum{
	LED_OFF   = 0,
	LED_ON    = 1,
	LED_BLINK = 2
}Led_Show_Status_t;

/**
 * @brief LED_BLINK_INTERVAL  in second no less than 2                         
 */
#define LED_BLINK_INTERVAL 1 


/* ========================================================================== */
/*                           IS_A_WIFI_GATEWAY                                */
/* ========================================================================== */
#ifdef IS_A_WIFI_GATEWAY

extern volatile Led_Show_Status_t led_status;
extern volatile Led_Show_Status_t led_polling;


#ifdef _DEBUG_LEDS
extern volatile Led_Show_Status_t led_blue;
#endif

#endif


/* ========================================================================== */
/*                           IS_A_GSM_GATEWAY                                 */
/* ========================================================================== */
#ifdef IS_A_GSM_GATEWAY
extern volatile Led_Show_Status_t led_work;
extern volatile Led_Show_Status_t led_gprs;
extern volatile Led_Show_Status_t led_linka;
extern volatile Led_Show_Status_t led_linkb;
#endif


void Led_Task_Start(void);
void Led_task(void);

#endif
