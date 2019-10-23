/*
 * nvm.h
 *
 *  Created on: Jun 18, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_NVM_H_
#define MAIN_NVM_H_

#include "common.h"
#include "data_types_IS.h"


#define SET_GW_CONFIG_NVM "set_gw_conf"
#define SET_GW_PARAM_NVM "set_gw_param"
#define SET_LINE_CONFIG_NVM	"set_line"
#define MB_BAUDRATE_NVM	"mb_baud"
#define SET_DEVS_CONFIG	"set_dev_conf"

#define MQTT_USER "mqtt_user"
#define MQTT_PASS "mqtt_pass"
#define MQTT_URL "mqtt_url"

#define MODEL_GUID "model_guid"
#define LINE_ID "js_lindid"


#if NVM_DEBUG == 1
	#define	PRINTF_DEBUG_NVM	printf
#else
	#define	PRINTF_DEBUG_NVM(...)
#endif


C_INT16 NVM__ReadString(const C_SCHAR* var, C_SCHAR* str, size_t* len);
C_INT16 NVM__WriteString (const C_SCHAR* var, C_SCHAR* str);

C_INT16 NVM__ReadU8Value(const C_SCHAR* var, C_BYTE* val);
C_INT16 NVM__WriteU8Value(const C_SCHAR* var, C_BYTE val);

C_INT16 NVM__ReadU16Value(const C_SCHAR* var, C_UINT16* val);
C_INT16 NVM__WriteU16Value(const C_SCHAR* var, C_UINT16 val);

C_INT16 NVM__ReadU32Value(const C_SCHAR* var, C_UINT32* val);
C_INT16 NVM__WriteU32Value(const C_SCHAR* var, C_UINT32 val);

C_INT16 NVM__EraseKey(const C_SCHAR* var);

C_INT16 NVM__WriteBlob (const C_SCHAR* var, void* vec, size_t len);
C_INT16 NVM__ReadBlob(const C_SCHAR* var, void* vec, size_t* len);
C_INT16 NVM__EraseAll(void);

void test_write_config_to_nvm(void);

#ifdef INCLUDE_PLATFORM_DEPENDENT
void NVM__Open(void);
void NVM__Close(void);
#endif
#endif /* MAIN_NVM_H_ */
