/**
 * @file MQTT_Interface_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief   functions implementations specific related to the managment of 
 *          real time clock.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MQTT_INTERFACE_IS_H
#define __MQTT_INTERFACE_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "mqtt_client.h"
#include "mqtt_config.h"
#endif
/* Exported types ------------------------------------------------------------*/ 

#pragma pack(1)
typedef struct mqtt_config_s{
	C_URI uri;                     		   /*!< Complete MQTT broker URI */
	C_USERNAME username;                   /*!< MQTT username */
	C_PASSWORD password;                   /*!< MQTT password */
	int keepalive;
	const char *cert_pem;
}mqtt_config_t;
#pragma pack()

#ifdef INCLUDE_PLATFORM_DEPENDENT
typedef esp_mqtt_event_handle_t mqtt_event_handle_t;
typedef esp_mqtt_client_handle_t mqtt_client_handle_t;
#endif

/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_INT32 mqtt_client_subscribe(C_SCHAR *topic, C_INT16 qos);
C_RES mqtt_client_unsubscribe(C_SCHAR *topic);
C_INT32 mqtt_client_publish(C_SCHAR *topic, C_SBYTE *data, C_INT16 len, C_INT16 qos, C_INT16 retain);
C_RES mqtt_client_start(void);
C_RES mqtt_client_destroy(void);
C_RES mqtt_client_stop(void);
void* mqtt_client_init(mqtt_config_t* mqtt_cfg_nvm);
mqtt_client_handle_t MQTT__GetClient (void);

void mqtt_client_reinit(void);

#ifdef __cplusplus
}
#endif

#endif
