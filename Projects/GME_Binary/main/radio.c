#include "radio.h"
#include "wifi.h"
#include "mobile.h"

gme_sm_t Radio__Config(void){
	gme_sm_t config_status = DISCONNECTED;
#if (NETWORK_INTERFACE == WIFI_INTERFACE)
	config_status = WiFi__Config(WiFi_GetConfigSM());
#elif (NETWORK_INTERFACE == GSM_INTERFACE)
	config_status = Mobile__Config();
#endif
	return config_status;
}

void Radio__WaitConnection(void)
{
#if (NETWORK_INTERFACE == WIFI_INTERFACE)
	WiFi__WaitConnection();
#elif (NETWORK_INTERFACE == GSM_INTERFACE)
	Mobile__WaitConnection();
#endif
}

connection_status_t Radio__GetStatus(void){

#if (NETWORK_INTERFACE == WIFI_INTERFACE)
	return WIFI__GetSTAStatus();
#elif (NETWORK_INTERFACE == GSM_INTERFACE)
	return Mobile__GetStatus();
#endif
}
