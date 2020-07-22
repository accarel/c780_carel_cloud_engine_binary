/**
 * @file   radio.c
 * @author carel
 * @date   9 Sep 2019
 * @brief  high level functions related to the radio management platform independent
 *
 */
#include "radio.h"
#include "wifi.h"
#include "mobile.h"

/**
 * @brief Radio__Config
 *        configure the radio based on the detected platform
 *
 * @param  none
 *
 * @return
 */
gme_sm_t Radio__Config(void){
	gme_sm_t config_status = DISCONNECTED;
	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		config_status = WiFi__Config(WiFi_GetConfigSM());
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		config_status = Mobile__Config();

	return config_status;
}


/**
 * @brief Radio__WaitConnection
 *
 * @param  none
 *
 * @return none
 */
void Radio__WaitConnection(void)
{
	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		WiFi__WaitConnection();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		Mobile__WaitConnection();
}


/**
 * @brief Radio__GetStatus
 * @param  none
 *
 * @return connection_status_t CONNECTED/DISCONNECTED
 */
connection_status_t Radio__GetStatus(void){

	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		return WIFI__GetSTAStatus();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		return Mobile__GetStatus();
	else return DISCONNECTED;
}


/**
 * @brief Radio__GetRSSI
 * @param  none
 *
 * @return int8_t rssi value
 */
int8_t Radio__GetRSSI(void){
	if (PLATFORM(PLATFORM_DETECTED_WIFI) ||
			PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) ||
			PLATFORM(PLATFORM_DETECTED_BCU))
		return WiFi__GetRSSI();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		return 0;	// for 2G model, wifi is off, so no wifi power is measured
	return 0;
}

C_RES Radio_WaitNetwork(void){
	if (PLATFORM(PLATFORM_DETECTED_2G))
		return Mobile__WaitNetwork();
	else
		return C_SUCCESS;
}
