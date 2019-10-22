/*
 * utilities.c
 *
 *  Created on: Sep 9, 2019
 *      Author: ataayoub
 */



//#include "js_msgs_hndlr.h"
//#include "js_hndlr_table.h"
#include"mqtt.h"
//#include "lwip/sockets.h"
//#include "lwip/dns.h"
//#include "lwip/netdb.h"
//#include "mqtt_client.h"
#include "wifi.h"
#include "nvm.h"
#include "utilities.h"

static req_set_gw_config_t gw_config_data = {0};

#define IMEI "301960018615360"


static uint8_t wifi_mac_address_gw[6] = {0};
static char wifi_mac_address_gw_str[14] = {0};

static uint8_t gsm_imei_gw_[15] = {0};
static char gsm_imei_gw_str[16] = {0};

static void Utilities__ScanGWConfigData(void);



void Utilities__CalcMACAddr(void){

	esp_err_t err = esp_wifi_get_mac(ESP_IF_WIFI_STA, wifi_mac_address_gw);
	if(ESP_OK == err){
		memset((void*)wifi_mac_address_gw_str,0,sizeof(wifi_mac_address_gw_str));
		printf ("mac addres :");
		for(uint8_t i=0; i<6 ; i++){
		  char temp[3];
		  sprintf(temp,"%02X",wifi_mac_address_gw[i]);
		  printf("%s",temp );
		  strcat(wifi_mac_address_gw_str,temp);
		}
		printf("\nmac_address = %s\n\n", wifi_mac_address_gw_str);
	}

}

char* Utilities__GetMACAddr(void){
	return wifi_mac_address_gw_str;
}


void Utilities__CalcIMEICode(void){
	sprintf(gsm_imei_gw_str, "%s", IMEI);
}

char* Utilities__GetIMEICode(void){
	return gsm_imei_gw_str;
}



size_t getFilesize(const char* filename) {
    struct stat st;
    if(stat(filename, &st) != 0) {
        return 0;
    }
    return st.st_size;
}



//NVM

static void Utilities__ScanGWConfigData(void){
	size_t len = 0;
	NVM__ReadBlob(SET_GW_PARAM_NVM,(void*)&gw_config_data,&len);
}


req_set_gw_config_t* Utilities__GetGWConfigData(void){
	return &gw_config_data;
}




void Utilities__Init(void){

	Utilities__ScanGWConfigData();
	//JSON__ParseLineId();
//	Set_last_boot_time();
//	WIFI__SetSTAConnectionTime();

#if (NETWORK_INTERFACE == WIFI_INTERFACE)
	Utilities__CalcMACAddr();
#elif (NETWORK_INTERFACE == GSM_INTERFACE)
	Utilities__CalcIMEICode();
#endif

}
