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



esp_err_t NVM__ReadString(const char* var, char* str, size_t* len);
esp_err_t NVM__WriteString (const char* var, char* str);

esp_err_t NVM__ReadU8Value(const char* var, uint8_t* val);
esp_err_t NVM__WriteU8Value(const char* var, uint8_t val);

esp_err_t NVM__ReadU16Value(const char* var, uint16_t* val);
esp_err_t NVM__WriteU16Value(const char* var, uint16_t val);

esp_err_t NVM__ReadU32Value(const char* var, uint32_t* val);
esp_err_t NVM__WriteU32Value(const char* var, uint32_t val);

esp_err_t NVM__EraseKey(const char* var);

esp_err_t NVM__WriteBlob (const char* var, void* vec, size_t len);
esp_err_t NVM__ReadBlob(const char* var, void* vec, size_t* len);
esp_err_t NVM__EraseAll(void);

void test_write_config_to_nvm(void);

void NVM__Open(void);
void NVM__Close(void);
#endif /* MAIN_NVM_H_ */
