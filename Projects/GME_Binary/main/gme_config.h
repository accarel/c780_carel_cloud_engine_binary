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

/*-------------------------------
 * Default HTML Login
 *-----------------------------*/
#define HTMLLOGIN_DEF_USR			"admin"
#define HTMLLOGIN_DEF_PSWD			"admin"


/*-------------------------------
 * WiFi Interface
 *-----------------------------*/
#define STA_DEF_IP			"192.168.43.50"
#define STA_DEF_GW			"192.168.43.1"
#define STA_DEF_DHCP		1
#define STA_DEF_NETMASK		"255.255.255.0"
#define STA_DEF_DNS1		"8.8.8.8"
#define STA_DEF_DNS2		"8.8.4.4"

#define AP_DEF_IP			"192.168.100.1"
#define AP_DEF_GW			"192.168.100.1"
#define AP_DEF_NETMASK		"255.255.255.0"
#define AP_DEF_DHCP_BASE	"192.168.100.10"
#define AP_DEF_SSID			"GME"
#define AP_DEF_PSSWD		"NoPass"
#define AP_DEF_SSID_HIDDEN	0
#define AP_DEF_MAX_CONN		10
#define AP_DEF_DHCP			1
#define AP_DHCP_IP_RANGE	50


/*-------------------------------
 * MQTT Interface
 *-----------------------------*/
//#define MQTT_BROKER_URL		"m21.cloudmqtt.com"
//#define MQTT_BROKER_IP		"192.168.43.237"
//#define MQTT_BROKER_PORT	10672
//#define MQTT_SSL_NEED		1


/*-------------------------------
 * POLLING Time
 *-----------------------------*/
#define ALARM_POLLING_TIME  1
//25
#define HIGH_POLLING_TIME   11
//65
#define LOW_POLLING_TIME	33
//120

/*-------------------------------
 * BINARY MODEL DIRECTORY
 *-----------------------------*/
#define MODEL_NAME  "/spiffs/cpco_model.bin"

/*-------------------------------
 * FILES DIRECTORY
 *-----------------------------*/
#define MODEL_FILE  	"/spiffs/model.bin"
#define CERT_1 			"/spiffs/cert1.crt"
#define CERT_2 			"/spiffs/cert2.crt"

#define LOGIN_HTML 			"/spiffs/login.html"
#define CHANGE_CRED_HTML	"/spiffs/chcred.html"
#define CONFIG_HTML 		"/spiffs/config.html"
#define STYLE_CSS 			"/spiffs/style.css"
#define FAV_ICON 			"/spiffs/fav.ico"

/*-------------------------------
 * MODBUS Config
 *-----------------------------*/
// Define port options for the master application
#define MB_BAUDRATE 19200
#define MB_PORTNUM 2
#define MB_PARITY UART_PARITY_DISABLE

// Note: UART2 default pins IO16, IO17 do not work on ESP32-WROVER module
// because these pins connected to PSRAM
#define ECHO_TEST_TXD  (26)
//(GPIO_NUM_26)
#define ECHO_TEST_RXD  (25)
//(GPIO_NUM_25)

// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS   (18)




/*------------------------------
 * Firmware Version
 *-----------------------------*/
#define	FW_VER_XX	90
#define FW_VER_YY	00
#define FW_VER_ZZ	00



//Time to keep Reset button pressed to execute a factory reset
#define FACTORY_RESET_SEC	1	//Write the value in seconds

#endif /* MAIN_GME_CONFIG_H_ */
