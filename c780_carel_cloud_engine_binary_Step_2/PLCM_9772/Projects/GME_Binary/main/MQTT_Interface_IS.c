/**
 * @file   MQTT_Interface_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         real time clock.
 *         Probably the MQTT require a persistent handler typical in every implementations
 *         to pass the connection data between the routine, implement as a global variable
 *         or implement a function to retrieve it   
 *
 */

/* Includes ------------------------------------------------------------------------ */
#include "MQTT_Interface_IS.h"
#include "data_types_CAREL.h"
#include "MQTT_Interface_CAREL.h"
#include "utilities_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
static mqtt_client_handle_t mqtt_client;
#endif

esp_mqtt_client_config_t mqtt_cfg;

/* Functions implementations ------------------------------------------------------- */



/**
 * @brief mqtt_client_subscribe
 *          
 * @param param1 Topic name
 * @param param2 Quality of service level
 * @return C_FAIL ie. not connected etc.  C_SUCCESS
 */
C_INT32 mqtt_client_subscribe(C_SCHAR *topic, C_INT16 qos)
{
	C_INT32 msg_id = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	msg_id = esp_mqtt_client_subscribe(MQTT__GetClient(), (C_SCHAR*)MQTT_GetUuidTopic("/req"), qos);
#endif
	return msg_id;
}

/**
 * @brief mqtt_client_unsubscribe
 *          
 * @param param1 Topic name
 * @return C_FAIL ie. not connected etc.  C_SUCCESS
 */

C_RES mqtt_client_unsubscribe(C_SCHAR *topic)
{ 
	/* TO BE implemented */

	/* if (...)
	{
		return C_FAIL;	 
	}	
	*/		
	return C_SUCCESS;
}

/**
 * @brief mqtt_client_publish
 *          
 * @param param1 topic - Topic name
 * @param param2 data  - array of bytes to publish
 * @param param3 len   - num of bytes in data
 * @param param4 qos   - Quality of service level
 * @param param5 retain- indicate if the publish topic is of retain type 
 * @return C_FAIL ie. not connected etc.  C_SUCCESS
 */
C_INT32 mqtt_client_publish(C_SCHAR *topic, C_SBYTE *data, C_INT16 len, C_INT16 qos, C_INT16 retain)
{
	C_INT32 msg_id = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	msg_id = esp_mqtt_client_publish(MQTT__GetClient(), topic, (C_SCHAR*)data, len, qos, retain);
#endif
	return msg_id;
}

/**
 * @brief mqtt_client_start
 *        start the MQTT client engine   
 * @param None
 * @return C_FAIL ie. not connected to internet etc.  C_SUCCESS
 */
C_RES mqtt_client_start(void)
{
	C_RES err = C_SUCCESS;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = esp_mqtt_client_start(MQTT__GetClient());
#endif
	return err;	
}

C_RES mqtt_client_stop(void)
{
	C_RES err = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = esp_mqtt_client_stop(MQTT__GetClient());
#endif
	/*
		if you using an operating system be sure to wait that all the action
		related to MQTT are in a safe state before to stop
		ie. message are sent and the acknowledge receive etc.
		For this reason a timeout mechanism must be implemented in this routine
	*/
	return err;
}

/**
 * @brief mqtt_client_destroy
 *        stop the MQTT client engine and destroy handle
 * @param None
 * @return C_FAIL ie. timeout on wait to action completation / C_SUCCESS
 */
C_RES mqtt_client_destroy(void)
{
	C_RES err = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT	
	err = esp_mqtt_client_destroy(MQTT__GetClient());
#endif
	/* 
		if you using an operating system be sure to wait that all the action
		related to MQTT are in a safe state before to stop
		ie. message are sent and the acknowledge receive etc. 
		For this reason a timeout mechanism must be implemented in this routine
	*/ 
	return err;	
}

/**
 * @brief mqtt_client_init
 *        initialize all the data request by the MQTT client engine     
 * @param None
 * @return C_FAIL ie. no memory available (very strange but everything is possible) / C_SUCCESS
 */
void* mqtt_client_init(mqtt_config_t* mqtt_cfg_nvm)
{ /* TO BE implemented */

	/* The below routine must be called every time a subscribed topic message 
		arrive.  
		You must decomment the below rountine and assign to the proper handler
		or alternatively call it inside your receiving routine.

		mqtt_message_received_callback(C_CHAR *msg, C_UINT len) 
	*/
#ifdef INCLUDE_PLATFORM_DEPENDENT
		C_SBYTE mac[18];
		Get_Gateway_ID(mac);
		C_CHAR conn_buf[25];
		size_t conn_len = CBOR_Connected(conn_buf, 0);

		 mqtt_cfg.event_handle = EventHandler;
		 mqtt_cfg.uri = (C_SCHAR*)mqtt_cfg_nvm->uri;
   	     mqtt_cfg.username = (C_SCHAR*)mqtt_cfg_nvm->username;
		 mqtt_cfg.password = (C_SCHAR*)mqtt_cfg_nvm->password;
		 mqtt_cfg.keepalive =  mqtt_cfg_nvm->keepalive;
		 mqtt_cfg.lwt_topic = (C_SCHAR*)MQTT_GetUuidTopic("/connected");
		 mqtt_cfg.lwt_msg = conn_buf;
		 mqtt_cfg.lwt_msg_len = conn_len;
		 mqtt_cfg.lwt_qos = 0;
		 mqtt_cfg.lwt_retain = 1;
		 mqtt_cfg.cert_pem = mqtt_cfg_nvm->cert_pem;
		 mqtt_cfg.task_stack = 8192;
		 mqtt_cfg.client_id = mac;
	
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

	return mqtt_client;
#else
	return NULL;
#endif
}

/**
 * @brief MQTT__GetClient
 *
 * @param None
 * @return mqtt_client_handle_t mqtt_client
 */
#ifdef INCLUDE_PLATFORM_DEPENDENT
mqtt_client_handle_t MQTT__GetClient (void)
{
	return mqtt_client;
}
#endif

