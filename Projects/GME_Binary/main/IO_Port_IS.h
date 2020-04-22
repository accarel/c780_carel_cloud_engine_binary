/**
 * @file   IO_Port_IS.h
 * @author carel
 * @date 9 May 2020
 * @brief  functions implementations specific related to the managment of the
 *         I/O Port. 
 *
 */
#ifndef __IO_Port_IS_DEF
#define __IO_Port_IS_DEF

/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE
 //this define enable the output of the errors
//#define __DEBUG_IO_PORT_IS_LEV_1

//this define enable the output of others debug informations
#define __DEBUG_IO_PORT_IS_LEV_2
#endif

/* ========================================================================== */
/* general purpose I/O common to all                                          */
/* ========================================================================== */
#define HW_PLATFORM_DETECT_PIN GPIO_NUM_34

/* ========================================================================== */
/* bCU only related I/O                                                       */
/* ========================================================================== */
#ifdef __USE_CAREL_BCU_HW
#define ECHO_TEST_TXD  (19)
#define ECHO_TEST_RXD  (3)
#define ECHO_TEST_RTS  (22)
#endif

#define LED_GREEN_BCU     GPIO_NUM_27

/* ========================================================================== */
/* WiFi only related I/O                                                      */
/* ========================================================================== */

#ifdef __USE_ESP_WROVER_KIT
// Note: UART2 default pins IO16, IO17 do not work on ESP32-WROVER module
// because these pins connected to PSRAM
#define ECHO_TEST_TXD  (26)
#define ECHO_TEST_RXD  (25)
// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (18)

// for TTL one wire
#define TTL_TXD         (21)
#define TTL_RXD 		(3)
#define TTL_RTS			(1)

#endif


#ifdef __USE_USR_WIFI_HW
#define ECHO_TEST_TXD  (26)
#define ECHO_TEST_RXD  (25)
// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (18)


// for TTL one wire
#define TTL_TXD         (21)
#define TTL_RXD 		(3)
#define TTL_RTS			(1)
#endif


#define LED_GREEN_WIFI     GPIO_NUM_27
#define LED_RED_WIFI       GPIO_NUM_5



/* ========================================================================== */
/* 2G only related I/O                                                        */
/* ========================================================================== */
#define USR_SAMPLE

#ifdef USR_SAMPLE
#define GSM_POWER_CTRL_PIN   GPIO_NUM_22
#elif
//TODO BILATO solo temporaneamente
#define GSM_POWER_CTRL_PIN   GPIO_NUM_12
#endif


#define GSM_PWRKEY_PIN       GPIO_NUM_23

#ifdef __USE_USR_2G_HW
#define ECHO_TEST_TXD  (GPIO_NUM_26)
#define ECHO_TEST_RXD  (GPIO_NUM_25)
// RTS for RS485 Half-Duplex Mode manages DE/~RE

#ifdef USR_SAMPLE
#define ECHO_TEST_RTS   (GPIO_NUM_12)
#elif
#define ECHO_TEST_RTS   (GPIO_NUM_22)
#endif

// for TTL one wire
//TODO CHIEBAO da sistemare
#define TTL_TXD         (GPIO_NUM_21)
#define TTL_RXD 		(GPIO_NUM_3)
#define TTL_RTS			(GPIO_NUM_1)
#endif

#define LED_GREEN_2G     GPIO_NUM_27
#define LED_RED_2G       GPIO_NUM_5
#define LED_BLU_2G       GPIO_NUM_33


/* ========================================================================== */
/* 2G only defines and constants                                              */
/* ========================================================================== */

/* take a look to M95_Hardware_Design time are in ms */
#define GSM_PWRKEY_ON_TIME		800
#define GSM_PWRKEY_OFF_TIME		700


 typedef enum{
 	GSM_PWRKEY_ON   = 0,
 	GSM_PWRKEY_OFF  = 1
 }GSM_PWRKEY_Status_t;

 typedef enum{
	GSM_POWER_SUPPLY_OFF  = 0,
 	GSM_POWER_SUPPLY_ON   = 1
 }GSM_Power_Supply_Status_t;


/* ========================================================================== */
/* debugging board only related I/O                                           */
/* ========================================================================== */
#define LED_GREEN_ESP_WROVER_KIT       GPIO_NUM_2
#define LED_RED_ESP_WROVER_KIT  	   GPIO_NUM_0
#define LED_BLU_ESP_WROVER_KIT         GPIO_NUM_4



/* ========================================================================== */
/* function prototype                                                         */
/* ========================================================================== */
void Init_Button_Pin(void);
int Get_Button_Pin(void);
void Configure_IO_Check_HW_Platform_IS(void);
C_BYTE Check_HW_Platform_IS(void);
void Init_IO_IS(void);

void GSM_Module_Pwr_Supply_On_Off(C_BYTE set_status);
void GSM_Module_PwrKey_On_Off(C_BYTE set_status);



#endif
