/*
 * mqtt.h
 *
 *  Created on: Jul 1, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_MQTT_H_
#define MAIN_MQTT_H_

#include "mqtt_config.h"
#include "common.h"
#include "mqtt_client.h"

#define	JS_STATUS_PERIODIC_TIME	(5*60)





//#define MQTT_CONFIG_NVM "mqtt_conf"


#pragma pack(1)
typedef struct mqtt_config_s{
	char uri[100];                        /*!< Complete MQTT broker URI */
	char username[50];                   /*!< MQTT username */
	char password[50];                   /*!< MQTT password */
	int keepalive;
}mqtt_config_t;
#pragma pack()


#pragma pack(1)
typedef struct mqtt_timing_s{
	uint32_t js_status;
	uint32_t js_values;
}mqtt_times_t;
#pragma pack()


#pragma pack(1)
typedef struct mqtt_flags_s{
	uint8_t init:1;
}mqtt_flags_t;
#pragma pack()




char* MQTT__GetUuidTopic(const char* topic);
esp_err_t MQTT__Start(void);
void MQTT__Stop(void);
void MQTT__PeriodicTasks(void);
void MQTT__Alarms( uint16_t alias, uint32_t tstart,  uint32_t tstop, uint8_t alarm_issue);
void MQTT__InitValuesTimer(void);
void MQTT__FlushValues(void);

void MQTT__PeriodicTasks(void);
mqtt_flags_t MQTT__GetFlags(void);
//configuration_t MQTT__GetConfigStatus (void);
esp_mqtt_client_handle_t MQTT__GetClient (void);

char* MQTT__GetCertficate(void);



#endif /* MAIN_MQTT_H_ */
