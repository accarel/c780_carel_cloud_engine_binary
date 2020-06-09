/*
 * utilities.c
 *
 *  Created on: Sep 9, 2019
 *      Author: ataayoub
 */

#include "CAREL_GLOBAL_DEF.h"
#include "wifi.h"
#include "mobile.h"
#include "nvm_CAREL.h"
#include "utilities_CAREL.h"
#include "polling_CAREL.h"
#include "modbus_IS.h"
#include "gme_config.h"
#include "Led_Manager_IS.h"


static req_set_gw_config_t gw_config_data = {0};

static uint8_t wifi_mac_address_gw[6] = {0};
static char wifi_mac_address_gw_str[14] = {0};
static char gsm_imei_gw_str[16] = {0};

void Utilities__CalcMACAddr(void){
	C_RES err = WiFi__GetMac(wifi_mac_address_gw);
	if(C_SUCCESS == err){
		memset((void*)wifi_mac_address_gw_str, 0, sizeof(wifi_mac_address_gw_str));
		for(uint8_t i=0; i<6 ; i++){
		  char temp[3];
		  sprintf(temp, "%02X", wifi_mac_address_gw[i]);
		  strcat(wifi_mac_address_gw_str,temp);
		}

        #ifdef __DEBUG_UTILITIES_CAREL_LEV_2
		printf("\nmac_address = %s\n\n", wifi_mac_address_gw_str);
        #endif

	}
}

char* Utilities__GetMACAddr(void){
	return wifi_mac_address_gw_str;
}

char* Utilities__CalcIMEICode(void){
	return Mobile__GetImeiCode();
}

char* Utilities__GetIMEICode(void){
	return gsm_imei_gw_str;
}

static void Utilities_ScanGWConfigData(void){
	size_t len = 0;
	NVM__ReadBlob(SET_GW_PARAM_NVM, (void*)&gw_config_data, &len);
}

req_set_gw_config_t* Utilities__GetGWConfigData(void){
	return &gw_config_data;
}

void Utilities__Init(void){

	Utilities_ScanGWConfigData();
	// Recover current configured baud rate
	PollEngine__ReadBaudRateFromNVM();

	Modbus__ReadAddressFromNVM();
	Modbus__ReadDelayFromNVM();
	CBOR_ReadDidFromNVM();

	if (BinaryModel_Init() == C_SUCCESS)
	{
	  Update_Led_Status(LED_STAT_MODEL_CFG, LED_STAT_ON);
	}

	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
	  Utilities__CalcMACAddr();
	else if (PLATFORM(PLATFORM_DETECTED_2G))
	  Utilities__CalcIMEICode();

}
