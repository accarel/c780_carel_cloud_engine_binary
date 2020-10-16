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
static char pn[PN_SIZE + 1] = {0};

/**
 * @brief Utilities__CalcMACAddr
 *        retrieve the mac-addr of the esp32 module
 *
 * @param  none
 * @return none
 */
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

/**
 * @brief Utilities__GetMACAddr
 *        return the mac-addr
 *
 * @param  none
 * @return char*
 */
char* Utilities__GetMACAddr(void){
	return wifi_mac_address_gw_str;
}

/**
 * @brief Utilities__CalcIMEICode
 *        return the imei of the 2G module
 *
 * @param  none
 * @return char* Mobile__GetImeiCode()
 */
char* Utilities__CalcIMEICode(void){
	return Mobile__GetImeiCode();
}

/**
 * @brief Utilities_ScanGWConfigData
 *        Read from nvm the GME configurations data
 *
 * @param  none
 * @return none
 */
static void Utilities_ScanGWConfigData(void){
	size_t len = 0;
	NVM__ReadBlob(SET_GW_PARAM_NVM, (void*)&gw_config_data, &len);
}

/**
 * @brief Utilities__GetGWConfigData
 *        Return a pointer to a data structure req_set_gw_config_t
 *        containing a GME configurations data
 *
 * @param  none
 * @return req_set_gw_config_t*
 */
req_set_gw_config_t* Utilities__GetGWConfigData(void){
	return &gw_config_data;
}

/**
 * @brief Utilities__ReadPNFromNVM
 *        Read part number from nvm
 *        If part number is present in nvm, then that part number will be used
 *        otherwise if part number is not present or if it is "standard"
 *        then standard part number will be used (wifi or 2g according to hw)
 *
 * @param  none
 * @return none
 */
void Utilities__ReadPNFromNVM(void){
	size_t len = 0;
	C_RES err = NVM__ReadString(GME_PN, pn, &len);

	if (err == C_FAIL || 		// if no info on part number is saved to nvm, use standard codes
		(strcmp(pn,"standard") == 0) || (strcmp(pn,"STANDARD") == 0)) // if standard, use standard codes
	{
		if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT))
			strcpy(pn, GW_WIFI_PARTNUMBER);
		else if (PLATFORM(PLATFORM_DETECTED_2G))
			strcpy(pn, GW_GSM_PARTNUMBER);
	}
	pn[11] = '\0';  //make sure pn string is properly terminated (for correct hello message composition)
	printf("pn used: %s\n", pn);
}

/**
 * @brief Utilities__GetPN
 *        Read part number from static variable
 *
 * @param  none
 * @return char*, current part number
 */
char* Utilities__GetPN(void){
	return pn;
}
/**
 * @brief Utilities__Init
 *        Init all utility function useful to
 *        retrive util information of the GME
 *        from nvm memory.
 *
 * @param  none
 * @return none
 */
void Utilities__Init(void){

	Utilities_ScanGWConfigData();
	// Recover current configured baud rate
	PollEngine__ReadBaudRateFromNVM();

	Modbus__ReadAddressFromNVM();
	Modbus__ReadDelayFromNVM();
	CBOR_ReadDidFromNVM();
	BinaryModel_Init();

	Utilities__ReadPNFromNVM();

	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
	  Utilities__CalcMACAddr();
	else if (PLATFORM(PLATFORM_DETECTED_2G)) {
	  Utilities__CalcIMEICode();
#ifdef GW_GSM_WIFI
		  Utilities__CalcMACAddr();
#endif
	}

}


/**
 * @brief Utilities__GetStatusPeriod
 * 		  return the time of MQTT transmit status period
 *
 * @param  none
 * @return C_UINT32
 */
C_UINT32 Utilities__GetStatusPeriod(void){

	if(Utilities__GetGWConfigData()->statusPeriod != 0)
		return Utilities__GetGWConfigData()->statusPeriod;
	else return GW_STATUS_TIME;
}
