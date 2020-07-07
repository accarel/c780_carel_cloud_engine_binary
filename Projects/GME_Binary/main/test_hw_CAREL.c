#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_IS.h"
#include "main_CAREL.h"

#include "gme_types.h"
#include "data_types_CAREL.h"
#include "modbus_IS.h"
#include "utilities_CAREL.h"

#include "sys_CAREL.h"
#include "sys_IS.h"

#include "common.h"

#include "wifi.h"
#include "polling_IS.h"
#include "nvm_CAREL.h"
#include "Led_Manager_IS.h"
#include "http_server_IS.h"
#include "mobile.h"
#include "radio.h"
#include "binary_model.h"

#include "SoftWDT.h"
#include "IO_Port_IS.h"

#include "nvm_IS.h"

#include "test_hw_CAREL.h"


extern USHORT param_buffer[2];
static char mac_address_gw_str[14] = {0};
static uint8_t stm;

static uint16_t read_value = 0;


// defaul configuration to connect GME to an AP
//
// sta_ssid = ?
// sta_pswd = ?
//
static html_config_param_t WiFiConfig = {
		//.gateway_mode = 0,
		.ap_ssid = AP_DEF_SSID,
		.ap_pswd = AP_DEF_PSSWD,
		.ap_ssid_hidden = AP_DEF_SSID_HIDDEN,
		.ap_ip = AP_DEF_IP,
		.sta_ssid = "TEST_AP",
		.sta_encryption = "wpa2",
		.sta_pswd= "12345678",
		.sta_dhcp_mode = 1,
		.sta_static_ip = "",
		.sta_netmask = "",
		.sta_gateway_ip = "",
		.sta_primary_dns = "",
		.sta_secondary_dns = "",
};


void test_hw(void)
{
	C_RES wifi_condition;
	static uint8_t t;

	while(1)
	{
		switch(stm)
		{
			case TEST_PRINT_MACADDR:
			{
				// 1) read and print MAC-ADDR

			    C_GATEWAY_ID dev_id;

				uint8_t s_id_tmp[6];
				esp_read_mac(&s_id_tmp[0], ESP_MAC_WIFI_STA);

				memset((void*)mac_address_gw_str, 0, sizeof(mac_address_gw_str));
				for(uint8_t i=0; i<6 ; i++){
				  char temp[3];
				  sprintf(temp, "%02X", s_id_tmp[i]);
				  strcat(mac_address_gw_str,temp);
				}
				printf("MAC=%s\r\n", mac_address_gw_str);

				stm = TEST_TRY_CONNECT;
			    break;
			}

			case TEST_TRY_CONNECT:
			{
				// 2) TRY TO CONNECT TO AP

				NVM_Init();
				WiFi__SetCustomConfig(WiFiConfig);
				WiFi__StartWiFi();

				Sys__Delay(2000);

				t = 25;

				stm = TEST_WAIT_RESP_CONNECT;
				break;
			}


			case TEST_WAIT_RESP_CONNECT:
			{
			  while(t)
			  {
				t-=1;
				Radio__WaitConnection();
				Sys__Delay(2000);
				break;
			  }
			    stm = TEST_PRINT_RESP_CONNECT;
				break;
			}


			case TEST_PRINT_RESP_CONNECT:
			{
				Sys__Delay(3000);

				wifi_condition = WIFI__GetSTAStatus();

				if(wifi_condition == CONNECTED)
					printf("STA=CONNECTED\n\r");
				else
					printf("STA=DISCONNECTED\r\n");

				stm = TEST_PRINT_MODBUS;
				break;
			}

			case TEST_PRINT_MODBUS:
			{
				uint8_t addr = 0;
			    uint8_t numOf = 0;
			    uint8_t retry = 0;
			    eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

			    // 3) TRY TO POLL MODBUS via RS485
				Modbus_Init(19200, UART_PARITY_DISABLE, 1, MB_PORTNUM_485);
				Sys__Delay(1000);

				Modbus_Task_Start();
				Sys__Delay(2000);

				errorReq = MB_MRE_NO_REG;
			    retry = 0;
				addr = 0;
				numOf = 1;

				Modbus__ReadDelayFromNVM();

				do {
					errorReq = app_holding_register_read(1, addr, numOf);
					retry++;
				} while(errorReq != MB_MRE_NO_REG && retry < 10);

				Sys__Delay(1000);

				read_value = param_buffer[0];

				printf("HR=%d\r\n", read_value);

				stm = TEST_END;
				break;
			}

			case TEST_END:
			{
				// DO nothing...just wait to power off the GME
				break;
			}


		}
	}
}


