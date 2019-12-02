/*
 * wifi.h
 *
 *  Created on: Jun 17, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#include "common.h"
#include"http_server.h"
#include "https_client_CAREL.h"
#include "esp_wifi.h"

typedef enum{
	DISCONNECTED = 0,
	CONNECTED,
}connection_status_t;

int WiFi__SetDefaultConfig(void);
int WiFi__StartWiFi(void);
esp_err_t WiFi__SetCustomConfig(html_config_param_t config);
void WiFi__ReadCustomConfigFromNVM(void);
void WiFi__WriteCustomConfigInNVM(html_config_param_t config);
void WiFi__ErasingConfig(void);
html_config_param_t WiFi__GetCustomConfig (void);
html_config_param_t* WiFi__GetCustomConfigPTR (void);
void WiFi__WaitConnection(void);
void WIFI__SetSTAConnectionTime(void);
uint32_t WIFI__GetSTAConnectionTime(void);
void WIFI__SetSTAStatus(connection_status_t status);
connection_status_t WIFI__GetSTAStatus(void);
C_RES WiFi__GetMac(uint8_t* wifi_mac_address_gw);
int8_t WiFi__GetRSSI(void);

esp_err_t test_sta(html_config_param_t config);



#endif /* MAIN_WIFI_H_ */
