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
#include "data_types_CAREL.h"      
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/

C_RES mqtt_client_subscribe(C_CHAR *topic, C_INT16 qos);
C_RES mqtt_client_unsubscribe(C_CHAR *topic);
C_RES mqtt_client_publish(C_CHAR *topic, C_BYTE *data, C_INT16 len, C_INT16 qos, C_INT16 retain);
C_RES mqtt_client_start(void);
C_RES mqtt_client_stop(void);
C_RES mqtt_client_init(void);
void mqtt_client_set_uri(C_CHAR *broker_uri, C_UINT16 broker_port);

extern void mqtt_message_received_callback(C_CHAR *msg, C_UINT16 len);

#ifdef __cplusplus
}
#endif

#endif
