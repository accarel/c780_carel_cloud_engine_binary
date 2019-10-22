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
#include "https_client.h"
#include "esp_wifi.h"

int WiFi__SetDefaultConfig(void);
int WiFi__StartWiFi(void);
esp_err_t WiFi__SetCustomConfig(html_config_param_t config);
void WiFi__ReadCustomConfigFromNVM(void);
void WiFi__WriteCustomConfigInNVM(html_config_param_t config);
void WiFi__ErasingConfig(void);
html_config_param_t WiFi__GetCustomConfig (void);
void WiFi__WaitConnection(void);

esp_err_t test_sta(html_config_param_t config);

#endif /* MAIN_WIFI_H_ */
