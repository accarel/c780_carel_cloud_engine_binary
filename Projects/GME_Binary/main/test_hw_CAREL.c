#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_IS.h"
#include "main_CAREL.h"
#include "esp_modem.h"
#include "sim800.h"
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


char* SendATCommandAndWaitAnswer(char* command, char* data) {

	size_t length;
	int len = uart_write_bytes(CONFIG_UART_MODEM_PORT, command, strlen(command));
	do {
		uart_get_buffered_data_len(CONFIG_UART_MODEM_PORT, &length);
	} while (length==0);
	length = MIN(256, length);
	len = uart_read_bytes(CONFIG_UART_MODEM_PORT, (uint8_t*)data, length, -1);

	return data;
}

void test_hw(C_BYTE platform)
{
	C_RES wifi_condition;
	static uint8_t t;

	if PLATFORM(PLATFORM_DETECTED_WIFI)
			stm = TEST_PRINT_MACADDR;
	else if PLATFORM(PLATFORM_DETECTED_2G)
			stm = TEST_PRINT_IMSI;
	else {
		printf("cannot proceed with test: unrecognized platform\n");
		return;
	}

	while(1)
	{
		switch(stm)
		{
			case TEST_PRINT_MACADDR:
			{
				// 1) read and print MAC-ADDR

//			    C_GATEWAY_ID dev_id;

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
			    eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

			    // 3) TRY TO POLL MODBUS via RS485
				Modbus_Init(19200, UART_PARITY_DISABLE, 1, MB_PORTNUM_485);
				Sys__Delay(1000);

				Modbus_Task_Start();
				Sys__Delay(2000);

				errorReq = MB_MRE_NO_ERR;
			    retry = 0;
				addr = 1;
				numOf = 1;

				Modbus__ReadDelayFromNVM();

				do {
					errorReq = app_holding_register_read(1, addr, numOf);
					retry++;
				} while(errorReq != MB_MRE_NO_ERR && retry < 10);

				Sys__Delay(1000);

				read_value = param_buffer[0];
				if(errorReq != MB_MRE_NO_ERR)
					printf("HR=FAIL\n");
				else
					printf("HR=%d\r\n", read_value);

				stm = TEST_END;
				break;
			}

			case TEST_PRINT_IMSI:
			{
				Init_IO_IS();

				printf("GSM_POWER_SUPPLY_ON\n");
				GSM_Module_Pwr_Supply_On_Off(GSM_POWER_SUPPLY_ON);
				printf("GSM_PWRKEY_ON\n");
				GSM_Module_PwrKey_On_Off(GSM_PWRKEY_ON);
				Sys__Delay(16000);

				/* Config UART */
				uart_config_t uart_config = {
						.baud_rate = 115200,
						.data_bits = UART_DATA_8_BITS,
						.parity = UART_PARITY_DISABLE,
						.stop_bits = UART_STOP_BITS_1,
						.flow_ctrl = MODEM_FLOW_CONTROL_NONE
				};
				uart_param_config(CONFIG_UART_MODEM_PORT, &uart_config);
				uart_set_pin(CONFIG_UART_MODEM_PORT, Get_Modem_TX(), Get_Modem_RX(), UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
				uart_driver_install(CONFIG_UART_MODEM_PORT, CONFIG_UART_RX_BUFFER_SIZE, CONFIG_UART_TX_BUFFER_SIZE, 0, NULL, 0);

				char* data = (char*) malloc(256);
				memset(data,0,256);

				// sync M95
				SendATCommandAndWaitAnswer("AT\r", data);
				char* ret = strstr(data, "OK");
				if (ret == NULL){
					printf("M95=FAIL\n");
					stm = TEST_END;
					break;
				}
				else printf("M95=OK\n");
				// get imei
				memset(data,0,256);
				SendATCommandAndWaitAnswer("AT+CGSN\r", data);
				char imei[MODEM_IMEI_LENGTH] = {0};
				memcpy(imei, &data[2], 15);
				printf("IMEI=%.15s\n", imei);
 				// check sim
				memset(data,0,256);
				SendATCommandAndWaitAnswer("AT+CIMI?\r", data);
				char imsi[MODEM_IMSI_LENGTH] = {0};
				strcpy(imsi, data);
				//printf("imsi: %s\n", imsi);
				ret = strstr(data, "ERROR");
				if (ret != NULL) printf("SIM=FAIL\n");
				else printf("SIM=OK\n");
				// get operator
				memset(data,0,256);
				SendATCommandAndWaitAnswer("AT+COPS?\r", data);
				char operator[256] = {0};
				strcpy(operator, data);
				printf("GSM=%s\n", operator);

				stm = TEST_PRINT_MODBUS;
				break;
			}

			case TEST_END:
			{
				if PLATFORM(PLATFORM_DETECTED_2G) {
					GSM_Module_PwrKey_On_Off(GSM_PWRKEY_OFF);
					Sys__Delay(12000);
					PRINTF_DEBUG("Powering down 2G module... power down\n");
				}
				//
				// For wifi do nothing...just wait to power off the GME
				// the system reboots due to the WDT
				//
				while(1);
				break;
			}

			default:
			{
				printf("cannot proceed with test: unrecognized step\n");
				break;
			}


		}
	}
}

