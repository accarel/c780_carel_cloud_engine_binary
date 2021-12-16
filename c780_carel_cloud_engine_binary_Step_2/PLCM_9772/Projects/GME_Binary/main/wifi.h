/**
 * @file wifi.h
 * @author ataayoub, CAREL
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the WiFi
 */

#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_



/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "common.h"
#include"http_server_CAREL.h"
#include "https_client_CAREL.h"
#include "esp_wifi.h"
#include "radio.h"

#include "CAREL_GLOBAL_DEF.h"

/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

 //this define enable the output of the errors
//#define __DEBUG_WIFI_LEV_1

//this define enable the output of others debug informations
//#define __DEBUG_WIFI_LEV_2
#endif


gme_sm_t WiFi__Config (config_sm_t sm);
config_sm_t WiFi_GetConfigSM(void);
void WiFi_SetConfigSM(config_sm_t config_state);

int WiFi__SetDefaultConfig(void);
int WiFi__StartWiFi(void);
esp_err_t WiFi__SetCustomConfig(html_config_param_t config);
void WiFi__ReadCustomConfigFromNVM(void);
void WiFi__WriteCustomConfigInNVM(html_config_param_t config);
void WiFi__ErasingConfig(void);
html_config_param_t WiFi__GetCustomConfig (void);
html_config_param_t* WiFi__GetCustomConfigPTR (void);
void WiFi__WaitConnection(void);
void WIFI__SetSTAStatus(connection_status_t status);
connection_status_t WIFI__GetSTAStatus(void);
C_RES WiFi__GetMac(uint8_t* wifi_mac_address_gw);
int8_t WiFi__GetRSSI(void);
char* GetAvailableAPs(uint8_t index);
esp_err_t test_sta(html_config_param_t config);
void StartTimerForAPConnection(void);
void IsTimerForAPConnectionExpired(void);
#endif /* MAIN_WIFI_H_ */
