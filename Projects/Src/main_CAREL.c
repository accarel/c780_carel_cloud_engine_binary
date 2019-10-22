/**
 * @file main_carel.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  this is ONLY an example of the flow request to make a  
 *         functional system based on the Carel Cloud Library. 
 *         Some routine could be called as a task if an 
 *         operating system is available.
 *         In case the OS is not available is possible to use the
 *         routine in a mega-loop but take care that the system are
 *         able to run without significat jitter.
 */

#include "data_types_CAREL.h"
#include "RTOS_IS.h"
#include "File_System_IS.h"
#include "File_System_CAREL.h"
#include "Button_IS.h"
#include "Leds_IS.h"
#include "DNS_IS.h"
#include "DHCP_IS.h"
#include "SecureTransport_IS.h"
#include "MQTT_Interface_CAREL.h"
#include "RS485_IS.h"
#include "RTC_IS.h"
#ifdef IS_A_GSM_GATEWAY
#include "GSM_Miscellaneous_IS.h"
#endif

#ifdef IS_A_WIFI_GATEWAY
#include "WiFi_Miscellaneous_IS.h"
#endif

void reboot(void)
{
return;
}

/* Functions implementation -------------------------------------------------------*/


void main_carel(void)
{
	C_RES retval;

	C_URI broker_uri;
	C_UINT16 broker_port;

	C_BYTE rs485_port, rs485_stop_bits, rs485_parity;
	C_UINT32 rs485_baud_rate;
	
	C_IPV4 dns_address;

	C_URI ntp_server;
	C_UINT16 ntp_port;

	C_CHAR apn[30];

	/* Initialize OS */
	/* This function will be fully implemented by USR */
	RTOS_Init();

	/* Initialize File System */
	/* This function will be fully implemented by USR */
	File_System_Init();

	/* Initialize button gpio and task that monitors its state */
	/* button task has to manage the following: 
	   If, at startup, pressed for more than 5 seconds call function Set_Factory(),
	   implemented by Carel, and reboot
	   If, during operation, pressed for more than 2 seconds and less than 5, reboot */
	Button_Init();

	/* Initialize leds gpio and task that sets their state */
	/* led task has to give information on the status of connections
	   calling functions Get_MQTT_Connection_Status(), Get_Modbus_Connection_Status(), 
	   Get_2G_Connection_Status(), Get_AP_Connection_Status(), Is_Model_Present() */
	Leds_Init();

	/* Recover configuration file */
	/* This function will be fully implemented by Carel */
	File_System_Config_Load((C_CHAR*)"config.bin");

	/* Connect to WiFi access point or 2G provider according to GW part number */
	/* These functions will be fully implemented by USR */
#ifdef IS_A_WIFI_GATEWAY
	WiFi_Init();
	DHCP_Client_Init();
#endif
#ifdef IS_A_GSM_GATEWAY
	Get_GSM_APN(apn);
	GSM_Init(apn);
#endif

	/* DNS client initialization */
	/* Retrieve info on DNS client (by Carel) */
	Get_DNS(&dns_address);
	/* This function will be fully implemented by USR */
	DNS_Client_Init(dns_address);

	/* Initialize RTC and load certificate */
	/* Retrieve NTP server and port from configuration file */
	Get_NTP_server(&ntp_server);
	Get_NTP_port(&ntp_port);
	/* RTC_Init function will be fully implemented by USR 
	   File_System_Cert_Load will be implemented by Carel */
	if(RTC_Init(ntp_server, ntp_port) != C_SUCCESS)
		reboot();
	else {
		if(File_System_Cert_Load((C_CHAR*)"cert1.crt") != C_SUCCESS)
			if(File_System_Cert_Load((C_CHAR*)"cert2.crt") != C_SUCCESS)
				reboot();
	}
	/* Save boot time */
	RTC_Set_UTC_Boot_Time();
	
	/* TLS and HTTPS initialization */
	/* These functions will be fully implemented by USR */
	TLS_Init();
	HTTPS_Client_Init();

	/* MQTT initialization */
	MQTT_Start();
	MQTT_subscribe_to_default_topics();

	/* Recover model file */
	/* This function will be fully implemented by Carel */
	File_System_Model_Load((C_CHAR*)"model.bin");

	/* RS485 initialization */
	/* Retrieve information on RS485 data format and baud rate (implemented by Carel) */
	Get_RS485_TTL(&rs485_port);
	Get_RS485_BaudRate(&rs485_baud_rate);
	Get_RS485_StopBits(&rs485_stop_bits);
	Get_RS485_Parity(&rs485_parity);
	 
	/* This function will be fully implemented by USR */
	RS485_Init(rs485_port, rs485_baud_rate, rs485_stop_bits, rs485_parity);

	/* Polling engine initialization */
	/* This function will be fully implemented by Carel */
	/* Polling engine is temporarily stopped */
	Set_Polling_Status(C_STOP);
	Polling_Engine_Init();

}



