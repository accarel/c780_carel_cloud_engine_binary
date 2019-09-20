/**
 * @file   MQTT_Interface_CAREL.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  this module use the MQTT to send the data 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MQTT_INTERFACE_CAREL_H
#define __MQTT_INTERFACE_CAREL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "MQTT_Interface_IS.h"
#include "data_types_CAREL.h"      
   
   
/* Define --------------------------------------------------------------------*/
#define MQTT_IS_NOT_CONNECTED 0
#define MQTT_IS_CONNECTED     1

   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/


/* Exported global variables -------------------------------------------------*/

extern C_BYTE mqtt_engine_status;
extern C_URI broker_uri;
extern C_UINT16 broker_port;



/* Function prototypes -------------------------------------------------------*/

C_RES mqtt_subscribe_to_default_topics(void);

#ifdef __cplusplus
}
#endif

#endif
