/*
 * nvm.h
 *
 *  Created on: Jun 18, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_NVM_CAREL_H_
#define MAIN_NVM_CAREL_H_

#include "common.h"
#include "data_types_CAREL.h"

#define SET_GW_CONFIG_NVM "set_gw_conf"
#define SET_GW_PARAM_NVM "set_gw_param"
#define SET_LINE_CONFIG_NVM	"set_line"
#define MB_BAUDRATE_NVM	"mb_baud"
#define SET_DEVS_CONFIG_NVM	"set_dev_conf"
#define MB_CONNECTOR_NVM "mb_conn"
#define MB_CID_NVM "cid"
#define MB_DID_NVM "did"
#define MB_DEV_NVM "dev"
#define MB_CERT_NVM "cert"
#define MB_DELAY_NVM "del"
#define PE_STATUS_NVM "pe_status"
#define CFG_DEF_NVM "cfg_def_copied"
#define GME_PN "gme_pn"

#define MQTT_USER "mqtt_user"
#define MQTT_PASSWORD "mqtt_password"
#define MQTT_BROKER "mqtt_broker"
#define MQTT_PORT "mqtt_port"
#define NTP_SERVER "ntp_server"

#define APN_NAME "apn"
#define APN_USERNAME "apn_user"
#define APN_PASSWORD "apn_pass"

#if NVM_DEBUG == 1
	#define	PRINTF_DEBUG_NVM	printf
#else
	#define	PRINTF_DEBUG_NVM(...)
#endif



C_RES NVM__ReadString(const C_CHAR* var, C_CHAR* str, size_t* len);
C_RES NVM__WriteString (const C_CHAR* var, C_CHAR* str);

C_RES NVM__ReadU8Value(const C_CHAR* var, C_BYTE* val);
C_RES NVM__WriteU8Value(const C_CHAR* var, C_BYTE val);

C_RES NVM__ReadU32Value(const C_CHAR* var, C_UINT32* val);
C_RES NVM__WriteU32Value(const C_CHAR* var, C_UINT32 val);

C_RES NVM__EraseKey(const C_CHAR* var);
C_RES NVM__EraseAll(void);

C_RES NVM__WriteBlob (const C_CHAR* var, void* vec, size_t len);
C_RES NVM__ReadBlob(const C_CHAR* var, void* vec, size_t* len);


#endif /* MAIN_NVM_CAREL_H_ */
