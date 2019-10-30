/**
 * @file   MQTT_Interface_CAREL.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         real time clock.
 *         Probably the MQTT require a persistent handler typical in every implementations
 *         to pass the connection data between the routine, implement as a global variable
 *         or implement a function to retrieve it   
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "MQTT_Interface_CAREL.h"
#include "MQTT_Interface_IS.h"

#if 0

/**
 * @brief mqtt_engine_status contain the status of the MQTT engine 
 *        MQTT_IS_NOT_CONNECTED/MQTT_IS_CONNECTED    
 */  
C_BYTE mqtt_engine_status = MQTT_IS_NOT_CONNECTED;  

static char* ServerCertificate = NULL;
static C_BYTE mqtt_init = 0;
static C_MQTT_TOPIC mqtt_topic;
static mqtt_times_t mqtt_time;

/* Functions implementations -------------------------------------------------------*/

/**
 * @brief mqtt_subscribe_to_default_topics
 *        
 * @param none
 * @return C_SUCCESS/C_FAIL
 */

C_RES MQTT_Subscribe_Default_Topics(void)
{	
	C_GATEWAY_ID dev_id={0};
	C_RES resval;
	C_MQTT_TOPIC topic={0};

	if (MQTT_IS_NOT_CONNECTED == mqtt_engine_status) 
		return C_FAIL;

	Get_Gateway_ID(&dev_id);

	/* /req topic is the topic that receive the command from the cloud */    
	strcpy(topic,(C_SCHAR*)dev_id); 
	strcat(topic, "/req");  
	resval = mqtt_client_subscribe(&topic[0], 0);

	/* after the subscribing we communicate that we are connected */
	memset(topic,0,sizeof(topic));
	strcpy(topic,(C_SCHAR*)dev_id); 
	strcat(topic, "/connected");
	mqtt_client_publish(&topic[0], (C_SBYTE*)"1", 1, 1, 1);

	return C_SUCCESS;
}	

/**
 * @brief MQTT_Start
 *         
 * @param none
 * @return C_SUCCESS/C_FAIL
 */
C_RES MQTT_Start(void)
{
#if 0
	printf("MQTT_Init\n");

	mqtt_config_t mqtt_cfg_nvm = {0};
	
	C_BYTE gw_config_status;

	/* FSC_ReadFile allocate dynamically using memmgr_alloc and return the pointer of the allocated space,
	 * so we have to free the allocated space at the end of the function */
	ServerCertificate = (char*) FSC_ReadFile(CERT_1);

	if((Get_MQTT_username((C_SBYTE*)&mqtt_cfg_nvm.username) == C_SUCCESS) &&
			(Get_MQTT_password((C_SBYTE*)&mqtt_cfg_nvm.password) == C_SUCCESS) &&
			(Get_Gateway_Config(&gw_config_status) == C_SUCCESS) && CONFIGURED == gw_config_status){

		C_URI mqtt_broker;
		Get_MQTT_broker(&mqtt_broker);
		C_UINT16 mqtt_port;
		Get_MQTT_Port(&mqtt_port);
		C_SCHAR mport[5];
		Get_MQTT_keepalive(&mqtt_cfg_nvm.keepalive);
		strcpy(mqtt_cfg_nvm.uri, mqtt_broker);
		strcat(mqtt_cfg_nvm.uri, ":");
		strcat(mqtt_cfg_nvm.uri, itoa(mqtt_port, mport, 10));

		printf("mqtt_configured\n");
	}else{
		char mqtt_port_str[10] = {0};
		sprintf(mqtt_port_str,"%d",MQTT_DEFAULT_PORT);
		printf("mqtt_not_configured\n");
		strcpy(mqtt_cfg_nvm.uri, MQTT_DEFAULT_BROKER);
		strcat(mqtt_cfg_nvm.uri, ":");
		strcat(mqtt_cfg_nvm.uri, mqtt_port_str);
		strcpy(mqtt_cfg_nvm.username, MQTT_DEFAULT_USER);
		strcpy(mqtt_cfg_nvm.password, MQTT_DEFAULT_PWD);
		mqtt_cfg_nvm.keepalive = MQTT_KEEP_ALIVE_DEFAULT_SEC;
	}

	printf("uri=%s\n",mqtt_cfg_nvm.uri);
	printf("username=%s\n",mqtt_cfg_nvm.username);
	printf("password=%s\n",mqtt_cfg_nvm.password);
	printf("keepalive=%d\n",mqtt_cfg_nvm.keepalive);

	strcpy((C_SCHAR*)mqtt_cfg_nvm.cert_pem, ServerCertificate);

	//WiFi__WaitConnection();		//TODO
#ifdef MQTT_DEBUG
	C_UINT32 free;
	Get_freememory(&free);
    DEBUG_MQTT("[APP] Free memory: %ld bytes", free);
#endif
    mqtt_client_init(&mqtt_cfg_nvm);
    return mqtt_client_start();

#else
    return 0;
#endif


}

/**
 * @brief MQTT_Message_Received_Callback
 *         
 * @param param1 broker_uri  - the uri of the broker ie. "mqtt-dev.tera.systems"
 * @param param2 broke_port  - the listen port of the broker ie. 8883
 * @return none 
 */
void MQTT_Message_Received_Callback(C_SCHAR *msg, C_UINT16 len)
{
	CBOR_ReqTopicParser((C_CHAR*)msg, len);
}

void MQTT_Stop(void)
{
	printf("mqtt_stop\n");
	mqtt_client_stop();

}

void MQTT_Status(void)
{
	C_UINT16 pst;
	Get_StatusPeriod(&pst);
	if(RTC_Get_UTC_Current_Time() > (mqtt_time.cbor_status + pst))
	{
		printf("Sending STATUS CBOR \n\n");
		CBOR_SendStatus();
		mqtt_time.cbor_status = RTC_Get_UTC_Current_Time();
	}
}

void MQTT_Values(void)
{
	C_UINT16 pva;
	Get_ValuesPeriod(&pva);
	if(RTC_Get_UTC_Current_Time() > (mqtt_time.cbor_values + pva))
	{
		CBOR_SendFragmentedValues(Get_DB_index(), Get_DB_number());
		mqtt_time.cbor_values = RTC_Get_UTC_Current_Time();
	}
}

void MQTT_Alarms(c_cboralarms alarms)
{
	CBOR_SendAlarms(alarms);
}

void MQTT_PeriodicTasks(void)
{
	MQTT_Values();
	MQTT_Status();
}

C_MQTT_TOPIC* MQTT_GetUuidTopic(C_SCHAR* topic)
{
	
	C_GATEWAY_ID dev_id;
	Get_Gateway_ID(&dev_id);
	
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s", dev_id, (C_SCHAR*)topic);
	printf("topic = %s\n",mqtt_topic);

	return (C_MQTT_TOPIC*)mqtt_topic;
}

C_RES EventHandler(mqtt_client_config_t event)
{
    int msg_id;
    switch (event.event_id) {
        case MQTT_EVENT_CONNECTED:


            DEBUG_MQTT("MQTT_EVENT_CONNECTED");
            msg_id = mqtt_client_subscribe((C_SCHAR*)MQTT_GetUuidTopic("/req"), 0);
            DEBUG_MQTT("sent subscribe successful, msg_id=%d", msg_id);

            msg_id = mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/connected"), "1", 0, 1, 1);
            DEBUG_MQTT("sent publish successful, msg_id=%d", msg_id);

            if(0 == mqtt_init)
			{
				CBOR_SendHello();
            	mqtt_init = 1;
    		}

        	break;
        case MQTT_EVENT_DISCONNECTED:
            DEBUG_MQTT("MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            DEBUG_MQTT("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event.msg_id);

            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            DEBUG_MQTT("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event.msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            DEBUG_MQTT("MQTT_EVENT_PUBLISHED, msg_id=%d", event.msg_id);
            break;
        case MQTT_EVENT_DATA:
        {
        	C_GATEWAY_ID dev_id;
			Get_Gateway_ID(&dev_id);
		
            DEBUG_MQTT("MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event.topic_len, event.topic);
            printf("DATA=%.*s\r\n", event.data_len, event.data);

            C_MQTT_TOPIC parsed_topic;
            memset((void*)parsed_topic,0,sizeof(parsed_topic));
            C_UINT16 rec_topic_len = event.topic_len - strlen(dev_id);	//wifi_mac_address_gw_str
            C_SCHAR* rec_topic = event.topic + strlen(dev_id);	//wifi_mac_address_gw_str

            sprintf(parsed_topic,"%.*s", rec_topic_len, rec_topic);
            printf("parsed_topic = %s\n", parsed_topic);

			if(strcmp(parsed_topic, "/req") == 0){
				printf("/req found_topic\n");
				CBOR_ReqTopicParser((C_CHAR*)event.data, event.data_len);
			}
        }
            break;
        case MQTT_EVENT_ERROR:
            DEBUG_MQTT("MQTT_EVENT_ERROR");
            break;
        default:
            DEBUG_MQTT("Other event id:%d", event.event_id);
            break;
    }
    	
	return C_SUCCESS;
}

#endif
