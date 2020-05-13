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
#define _DEBUG_LED_MANAGER_IS
#endif

/* time in ms TAKE CARE is a busy wait time */

#define LED_TEST_DELAY_2G 500
#define LED_TEST_DELAY 100

/* time in seconds */
#define LED_BLINK_FAST_INTERVAL    		0
#define LED_BLINK_NOTSOFAST_INTERVAL	1
#define LED_BLINK_SLOW_INTERVAL    		2

#define LED_STAT_OFF   0
#define LED_STAT_ON    1

/* this are the possible leds status */
#define LED_STAT_ALL_OFF      0x0000
#define LED_STAT_MODEL_CFG    0x0001
#define LED_STAT_MQTT_CONN    0x0002
#define LED_STAT_RS485        0x0004
//#define LED_STAT_
#define LED_STAT_TEST         0x0080 
#define LED_STAT_FACT_DEF_A   0x0100
#define LED_STAT_FACT_DEF_B   0x0200


#if LED_DEBUG == 1
	#define	PRINTF_DEBUG_LED	printf
#else
	#define	PRINTF_DEBUG_LED(...)
#endif

void Update_Led_Status(C_UINT16 set_status, C_BYTE status);

void Led_Task_Start(void);

void Led_task(void);

#endif
