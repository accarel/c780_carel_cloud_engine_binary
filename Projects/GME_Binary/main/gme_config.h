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


#include "CAREL_GLOBAL_DEF.h"
#include "esp_err.h"
#include "esp_log.h"
//#include "mb_sense_modbus.h"
#include "common.h"

#define ENABLED		(1)
#define DISABLED	(0)

/*------------------------------
 * DEBUGGING DATA
 *-----------------------------*/
//TODO BILATO da ristrutturare
#define POLL_ENGINE_PRINTF_DEFAULT 	ENABLED

#ifdef __CCL_DEBUG_MODE
//TODO BILATO da ristrutturare
#define MQTT_JSON_PRINTF_DEFAULT 	ENABLED
#define HTTP_SERVER_DEBUG 			ENABLED
#define NVM_DEBUG 					DISABLED
#define DEEP_DEBUG_PRINTF_DEFAULT 	ENABLED
#endif

/*-------------------------------
 * WiFi Interface
 *-----------------------------*/
#define AP_DEF_IP			"10.10.100.254"
#define AP_DEF_GW			"10.10.100.254"
#define AP_DEF_NETMASK		"255.255.255.0"
#define AP_DEF_DHCP_BASE	"10.10.100.10"
#define AP_DEF_SSID			"CGATEM"
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

/*-------------------------------
 * Certificates Allocation
 *-----------------------------*/
#define CERT_MAX_NUMBRER	2
#define CERT_MAX_SIZE		1536


/*-------------------------------
 * MODBUS Config
 *-----------------------------*/
// Define port options for the master application
#define MB_BAUDRATE     19200
#define MB_PORTNUM      2
#define MB_PORTNUM_485  2
#define MB_PORTNUM_TTL  0
#define MB_PARITY       UART_PARITY_DISABLE


//Time to wait after a null alarm
#define NULL_ALARM_TIMER 	(90) //Sec

/*------------------------------
 * Firmware Version
 *-----------------------------*/
#define	FW_VER_XX	90
#define FW_VER_YY	00
#define FW_VER_ZZ	00


#endif /* MAIN_GME_CONFIG_H_ */
