/*
 * gme_config.h
 *
 *  Created on: Jun 14, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_GME_CONFIG_H_
#define MAIN_GME_CONFIG_H_


#include "esp_err.h"
#include "esp_log.h"
//#include "mb_sense_modbus.h"
#include "common.h"

#define ENABLED		(1)
#define DISABLED	(0)

/*------------------------------
 * DEBUGGING DATA
 *-----------------------------*/
#define POLL_ENGINE_PRINTF_DEFAULT 	ENABLED
#define MQTT_JSON_PRINTF_DEFAULT 	ENABLED
#define HTTP_SERVER_DEBUG 			ENABLED
#define NVM_DEBUG 					DISABLED
#define DEEP_DEBUG_PRINTF_DEFAULT 	ENABLED



/*-------------------------------
 * Radio Interface
 *-----------------------------*/
#define NETWORK_INTERFACE 	WIFI_INTERFACE
//#define NETWORK_INTERFACE 	GSM_INTERFACE
#define WIFI_INTERFACE 		1
#define GSM_INTERFACE 		0


/*-------------------------------
 * WiFi Interface
 *-----------------------------*/
#define STA_DEF_IP			"192.168.43.50"
#define STA_DEF_GW			"192.168.43.1"
#define STA_DEF_DHCP		1
#define STA_DEF_NETMASK		"255.255.255.0"
#define STA_DEF_DNS1		"8.8.8.8"
#define STA_DEF_DNS2		"8.8.4.4"

#define AP_DEF_IP			"10.10.100.254"
#define AP_DEF_GW			"10.10.100.254"
#define AP_DEF_NETMASK		"255.255.255.0"
#define AP_DEF_DHCP_BASE	"10.10.100.10"
#define AP_DEF_SSID			"cgatem"
#define AP_DEF_PSSWD		"12345678"
#define AP_DEF_SSID_HIDDEN	0
#define AP_DEF_MAX_CONN		10
#define AP_DEF_DHCP			1
#define AP_DHCP_IP_RANGE	50


/*-------------------------------
 * Timer Constants
 *-----------------------------*/
#define ALARM_POLLING_TIME	(2)


#define PASS_MODE_TIMER		(60)

#define WAITING_CONF_COUNTER (10)

/*-------------------------------
 * FILES DIRECTORY
 *-----------------------------*/
#define CERT_1	0
#define CERT_2	1
#define CERT1_SPIFFS		"/spiffs/cert1.crt"
#define CERT2_SPIFFS		"/spiffs/cert2.crt"

#define MODEL_FILE  		"/spiffs/model.bin"

#define LOGIN_HTML 			"/spiffs/login.html"
#define CHANGE_CRED_HTML	"/spiffs/chcred.html"
#define CONFIG_HTML 		"/spiffs/config.html"
#define STYLE_CSS 			"/spiffs/style.css"
#define FAV_ICON 			"/spiffs/fav.ico"

#define CFG_DEF			     "/spiffs/cfgdef.bin"
#define CFG_USR		         "/spiffs/cfgusr.bin"


#define FILE_CFG_DEF		 0
#define FILE_CFG_USR         1

/*-------------------------------
 * Certificates Allocation
 *-----------------------------*/
#define CERT_MAX_NUMBRER	2
#define CERT_MAX_SIZE		1536


/*-------------------------------
 * MODBUS Config
 *-----------------------------*/
// Define port options for the master application
#define MB_BAUDRATE 19200
#define MB_PORTNUM 2
#define MB_PARITY UART_PARITY_DISABLE

#ifdef __USE_CAREL_BCU_HW
#define ECHO_TEST_TXD  (19)
#define ECHO_TEST_RXD  (3)
#define ECHO_TEST_RTS  (22)
#endif

#ifdef __USE_ESP_WROVER_KIT
// Note: UART2 default pins IO16, IO17 do not work on ESP32-WROVER module
// because these pins connected to PSRAM
#define ECHO_TEST_TXD  (26)
#define ECHO_TEST_RXD  (25)

// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (18)
#endif

#ifdef __USE_USR_WIFI_HW
#define ECHO_TEST_TXD  (26)
#define ECHO_TEST_RXD  (25)
// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (18)
#endif

#ifdef __USE_USR_2G_HW
#define ECHO_TEST_TXD  (26)
#define ECHO_TEST_RXD  (25)
// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (18)
#endif


// for test TODO CHIEBAO
/*
#define ECHO_TEST_TXD  (GPIO_NUM_21)
#define ECHO_TEST_RXD  (GPIO_NUM_22)
// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (GPIO_NUM_18)
*/


//Time to wait after a null alarm
#define NULL_ALARM_TIMER 	(90) //Sec

/*------------------------------
 * Firmware Version
 *-----------------------------*/
#define	FW_VER_XX	90
#define FW_VER_YY	00
#define FW_VER_ZZ	00



/*------------------------------
 * RESET DATA
 *-----------------------------*/
//Reset Buttons
#define CONFIG_RESET_BUTTON		GPIO_NUM_15
#define FACTORY_RESET_BUTTON	GPIO_NUM_16
//Time to keep Reset buttonS pressed to execute a factory reset
#define CONFIG_RESET_SEC		(10)	//Write the value in seconds
#define FACTORY_RESET_SEC		(30)	//Write the value in seconds

#endif /* MAIN_GME_CONFIG_H_ */
