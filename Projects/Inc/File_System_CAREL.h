/**
 * @file File_System_CAREL.h
 * @author carel
 * @date 9 Sep 2019
 * @brief CAREL functions related to the managment of the file system.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILE_SYSTEM_CAREL_H
#define __FILE_SYSTEM_CAREL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"   

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp32/file_system.h"
#endif

typedef struct configuration_file
{
	C_UINT32 RS485_baudrate;
	C_UINT16 address;
	
	C_BYTE gateway_mode;
	C_CHAR ap_ssid[30];
//	C_BYTE ap_ssid_hidden;	???
	C_CHAR ap_pswd[30];
	C_CHAR ap_ip[15];
	C_CHAR ap_netmask[15]; //non c'è nel GTW0000WT0 ma c'è nella pagina di configurazione
//	C_BYTE ap_dhcp_mode;
//	C_CHAR ap_dhcp_ip[15];
	C_CHAR sta_ssid[30];
//	C_CHAR sta_encryption[30];  ???
	C_CHAR sta_pswd[30];
	C_BYTE sta_dhcp_mode;
	C_CHAR sta_static_ip[15];
	C_CHAR sta_netmask[15];
	C_CHAR sta_gateway_ip[15];
	
    C_CHAR apn_name[30];

	C_CHAR sta_primary_dns[15];
//	C_CHAR sta_secondary_dns[15]; c'è nel GTW0000WT0?
    C_CHAR ntp_server[30];
	C_UINT16 ntp_port;

}configuration_file;

typedef struct model_file{

	C_CHAR header_signature[8];
	C_UINT16 header_version;
	C_BYTE mod_guid[16];
	C_UINT32 mod_version;

	C_BYTE RS485_stopbits;
	C_BYTE RS485_parity;

	C_UINT16 low_coils;
	C_UINT16 low_di;
	C_UINT16 low_hr;
	C_UINT16 low_ir;

	C_UINT16 high_coils;
	C_UINT16 high_di;
	C_UINT16 high_hr;
	C_UINT16 high_ir;

	C_UINT16 alarm_coils;
	C_UINT16 alarm_di;
	C_UINT16 alarm_hr;
	C_UINT16 alarm_ir;

	/*other modbus stuff*/

}model_file;

/* Function prototypes -------------------------------------------------------*/
C_INT32 FSC_filesize(const C_CHAR *fname);
C_RES FSC_file_checksum_check(C_CHAR *fname);
C_RES File_System_Check_File(void);
C_RES File_System_Config_Load(C_CHAR* name);
C_RES File_System_DefaultConfig_Load(void);

C_RES File_System_Cert_Check(C_CHAR* name);
C_RES File_System_Cert_Load(C_CHAR* name);
C_RES File_System_Cert_Store(C_CHAR* name);
C_RES File_System_Model_Check(C_CHAR* name);
C_RES File_System_Model_Load(C_CHAR* name);
C_RES File_System_DefaultModel_Load(void);
C_RES File_System_Model_Store(void);

C_RES Get_Device_Address(C_UINT16* device);
C_RES Get_Guid(C_BYTE* Guid);

C_RES Get_RS485_TTL(C_BYTE* rs485_port);
C_RES Get_RS485_BaudRate(C_UINT32* rs485_baud_rate);
C_RES Get_RS485_StopBits(C_BYTE* rs485_stop_bits);
C_RES Get_RS485_Parity(C_BYTE* rs485_parity);
C_RES Get_MQTT_broker(C_URI* mqtt_broker);
C_RES Get_MQTT_Port(C_UINT16* mqtt_port);

C_RES Get_Gateway_ID(C_GATEWAY_ID *s_id);
C_RES Get_DNS(C_IPV4* dns_address);
C_RES Get_NTP_server(C_URI* ntp_server);
C_RES Get_NTP_port(C_UINT16* ntp_port);
C_RES Get_GSM_APN(C_CHAR* apn);

C_RES Get_LowSpeedSamplingTime(C_UINT16* time);
C_RES Get_HighSpeedSamplingTime(C_UINT16* time);
C_RES Set_LowSpeedSamplingTime(C_UINT16 time);
C_RES Set_HighSpeedSamplingTime(C_UINT16 time);

#ifdef __cplusplus
}
#endif

#endif
