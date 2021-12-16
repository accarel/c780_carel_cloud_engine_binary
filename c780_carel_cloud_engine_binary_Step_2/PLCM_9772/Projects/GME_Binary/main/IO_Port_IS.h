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
#define HW_PLATFORM_DETECT_PIN      GPIO_NUM_34

//TP5
#define HW_PLATFORM_TEST_PIN        GPIO_NUM_15


/* ========================================================================== */
/* bCU only related I/O                                                       */
/* ========================================================================== */

#define TTL_TXD_BCU				GPIO_NUM_19
#define TTL_RXD_BCU				GPIO_NUM_3
#define TTL_RTS_BCU				GPIO_NUM_22

#define LED_GREEN_BCU			GPIO_NUM_27

/* ========================================================================== */
/* WROVER only related I/O                                                      */
/* ========================================================================== */

// Note: UART2 default pins IO16, IO17 do not work on ESP32-WROVER module
// because these pins connected to PSRAM
#define ECHO_TEST_TXD_WROVER		GPIO_NUM_26
#define ECHO_TEST_RXD_WROVER		GPIO_NUM_25
// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS_WROVER		GPIO_NUM_18

// for TTL one wire
#define TTL_TXD_WROVER				GPIO_NUM_21
#define TTL_RXD_WROVER				GPIO_NUM_3
#define TTL_RTS_WROVER				GPIO_NUM_1

// leds
#define LED_GREEN_ESP_WROVER_KIT       GPIO_NUM_2
#define LED_RED_ESP_WROVER_KIT  	   GPIO_NUM_0
#define LED_BLU_ESP_WROVER_KIT         GPIO_NUM_4

// button pin
#define CONFIG_RESET_BUTTON_WROVER	GPIO_NUM_19

/* ========================================================================== */
/* WiFi only related I/O                                                      */
/* ========================================================================== */

// RS485
#define ECHO_TEST_TXD_WIFI		GPIO_NUM_26
#define ECHO_TEST_RXD_WIFI		GPIO_NUM_25

// TTL one wire
#define TTL_TXD_WIFI			GPIO_NUM_21
#define TTL_RXD_WIFI			GPIO_NUM_3
// the DIR pin is not used
#define TTL_DIR_WIFI			GPIO_NUM_1

// leds
#define LED_GREEN_WIFI			GPIO_NUM_5
#define LED_RED_WIFI			GPIO_NUM_27

// button pin
#define CONFIG_RESET_BUTTON_WIFI	GPIO_NUM_0

/* ========================================================================== */
/* 2G only related I/O                                                        */
/* ========================================================================== */

// RS485
#define ECHO_TEST_TXD_2G		GPIO_NUM_26
#define ECHO_TEST_RXD_2G		GPIO_NUM_25

// TTL one wire
#define TTL_TXD_2G				GPIO_NUM_21
#define TTL_RXD_2G				GPIO_NUM_3
// the DIR pin is not used
#define TTL_DIR_2G				GPIO_NUM_1

// leds
#define LED_GREEN_2G     		GPIO_NUM_5		// WORK
#define LED_RED_2G       		GPIO_NUM_27		// LINKA
#define LED_BLU_2G       		GPIO_NUM_33		// LINKB

// M95 port
#define CONFIG_UART_MODEM_PORT 		1
// M95 powering
#define GSM_POWER_CTRL_PIN   	GPIO_NUM_22
#define GSM_PWRKEY_PIN       	GPIO_NUM_23
// M95 serial
#define UART_MODEM_TX_2G		GPIO_NUM_17
#define UART_MODEM_RX_2G		GPIO_NUM_16

// button pin
#define CONFIG_RESET_BUTTON_GSM 	GPIO_NUM_0

// M95 power key press time [ms]
#define GSM_PWRKEY_TIME		1000


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



/* ========================================================================== */
/* function prototype                                                         */
/* ========================================================================== */
void Init_Pins(void);
int Get_Button_Pin(void);

int Get_TEST_TXD(void);
int Get_TEST_RXD(void);
int Get_TEST_RTS(void);
int Get_TTL_TXD(void);
int Get_TTL_RXD(void);
int Get_TTL_RTS(void);

void Configure_IO_Check_HW_Platform_IS(void);
C_BYTE Check_HW_Platform_IS(void);
void Init_IO_IS(void);

void GSM_Module_Pwr_Supply_On_Off(C_BYTE set_status);
void GSM_Module_PwrKey_On_Off(C_BYTE set_status);

int Get_Modem_TX(void);
int Get_Modem_RX(void);
int Get_Modem_RTS(void);
int Get_Modem_CTS(void);
#endif
