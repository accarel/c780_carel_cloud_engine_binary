/**
 * @file   MQTT_Interface_CAREL.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  this module use the MQTT to send the data 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MQTT_INTERFACE_CAREL_H
#define __MQTT_INTERFACE_CAREL_H

#include "CBOR_CAREL.h"
#ifdef __cplusplus
 extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "data_types_CAREL.h"
#include "CAREL_GLOBAL_DEF.h"
#include "MQTT_Interface_IS.h"
#include "File_System_CAREL.h"
#include "CBOR_CAREL.h"      
#include "RTC_IS.h"

/* Define --------------------------------------------------------------------*/
#define MQTT_IS_NOT_CONNECTED 0
#define MQTT_IS_CONNECTED     1


/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

//this define enable the output of the command and communications errors
//#define __DEBUG_MQTT_INTERFACE_LEV_1

//this define enable the output of others debug informations
//#define __DEBUG_MQTT_INTERFACE_LEV_2
#endif



   
/* Exported types ------------------------------------------------------------*/ 

#pragma pack(1)
typedef struct {
	char *data;                         /*!< Data asociated with this event */
    int data_len;                       /*!< Lenght of the data for this event */
    char *topic;                        /*!< Topic asociated with this event */
    int topic_len;                      /*!< Length of the topic for this event asociated with this event */
    int msg_id;                         /*!< MQTT messaged id of message */
    
	void* client;    /*!< MQTT client handle for this event */
	int event_id;
	
}mqtt_client_config_t;
#pragma pack()

#if 0
#pragma pack(1)
typedef struct req_download_devs_config_s{
	char *username;
	char *password;
	char *uri;
	uint16_t crc;
}req_download_devs_config_t;
#pragma pack()

typedef req_download_devs_config_t req_update_ca_cert_t;

#pragma pack(1)
typedef struct req_update_dev_fw_s{
	char *username;
	char *password;
	char *uri;
	uint32_t file_index;
	uint32_t sec_to_wait;
}req_update_dev_fw_t;
#pragma pack()
#endif


/* Exported constants --------------------------------------------------------*/

#define QOS_0 	0
#define QOS_1 	1

#define NO_RETAIN  	0
#define RETAIN  	1

#define MQTT_DEBUG
#ifdef MQTT_DEBUG
#define DEBUG_MQTT(format, ...) printf("MQTT: " #format "\n", ##__VA_ARGS__);
#else
#define DEBUG_MQTT(format, ...)
#endif


/* Exported global variables -------------------------------------------------*/


/* Function prototypes -------------------------------------------------------*/
C_RES MQTT_Check_Status(void);
C_RES MQTT_Subscribe_Default_Topics(void);
C_RES MQTT_Start(void);
void MQTT_Stop(void);
void MQTT_Values(void);
void MQTT_Alarms(c_cboralarms alarms);
void MQTT_PeriodicTasks(void);
void MQTT_FlushValues(void);
C_MQTT_TOPIC* MQTT_GetUuidTopic(C_SCHAR* topic);
#ifdef INCLUDE_PLATFORM_DEPENDENT
C_RES EventHandler(mqtt_event_handle_t event);
#endif
C_BYTE MQTT_GetFlags(void);

void SetMsgIdCompare(int value);
int GetMsgIdCompare(void);


#ifdef __cplusplus
}
#endif

#endif
