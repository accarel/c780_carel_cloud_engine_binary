#include "radio.h"
#include "wifi.h"
#include "mobile.h"

gme_sm_t Radio__Config(void){
	gme_sm_t config_status = DISCONNECTED;
	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		config_status = WiFi__Config(WiFi_GetConfigSM());
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		config_status = Mobile__Config();

	return config_status;
}

void Radio__WaitConnection(void)
{
	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		WiFi__WaitConnection();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		Mobile__WaitConnection();
}

connection_status_t Radio__GetStatus(void){

	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		return WIFI__GetSTAStatus();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		return Mobile__GetStatus();
	else return DISCONNECTED;
}

int8_t Radio__GetRSSI(void){
	if (PLATFORM(PLATFORM_DETECTED_WIFI) ||
			PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) ||
			PLATFORM(PLATFORM_DETECTED_BCU))
		return WiFi__GetRSSI();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		return 0;	// for 2G model, wifi is off, so no wifi power is measured
	return 0;
}
