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
#include "File_System_CAREL.h"
#include "nvm.h"
#include "wifi.h"
#include "sys.h"
#include "utilities.h"
#include "polling_CAREL.h"
#include "polling_IS.h"

#include "Led_Manager_IS.h"
#if INCLUDE_PLATFORM_DEPENDENT
#include "mqtt_client.h"
#include "mqtt_config.h"
#include "freertos/FreeRTOS.h"
#endif
/**
 * @brief mqtt_engine_status contain the status of the MQTT engine 
 *        MQTT_IS_NOT_CONNECTED/MQTT_IS_CONNECTED    
 */  
C_BYTE mqtt_engine_status = MQTT_IS_NOT_CONNECTED;  

EventGroupHandle_t s_mqtt_event_group;
const int MQTT_CONNECTED_BIT = BIT0;
const int MQTT_DISCONNECTED_BIT = BIT1;

static C_BYTE mqtt_init = 0;
static C_MQTT_TOPIC mqtt_topic;
static mqtt_times_t mqtt_time;
static EventBits_t MQTT_BITS;
#if 0
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
#endif
/**
 * @brief MQTT_Start
 *         
 * @param none
 * @return C_SUCCESS/C_FAIL
 */
C_RES MQTT_Start(void)
{
	PRINTF_DEBUG("MQTT_Init\n");

	mqtt_config_t mqtt_cfg_nvm = {0};
	size_t pass_len = 0, user_len = 0;
	uint8_t gw_config_status, mqtt_url;

#if 0
		if(ESP_OK == NVM__ReadString(MQTT_USER, mqtt_cfg_nvm.username, &user_len)
			&& ESP_OK == NVM__ReadString(MQTT_PASS, mqtt_cfg_nvm.password, &pass_len)
			&& ESP_OK == NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status) && CONFIGURED == gw_config_status){

		size_t gw_config_len;
		req_set_gw_config_t gw_config_nvm = {0};

		NVM__ReadBlob(SET_GW_PARAM_NVM,(void*)&gw_config_nvm,&gw_config_len);

		mqtt_cfg_nvm.keepalive = gw_config_nvm.mqttKeepAliveInterval;
		strcpy(mqtt_cfg_nvm.uri, WiFi__GetCustomConfig().mqtt_server_addr);
		strcat(mqtt_cfg_nvm.uri, ":");
		strcat(mqtt_cfg_nvm.uri, WiFi__GetCustomConfig().mqtt_server_port);

		PRINTF_DEBUG("mqtt_configured\n");

	}else{

		char mqtt_port_str[10] = {0};
		sprintf(mqtt_port_str,"%d",MQTT_DEFAULT_PORT);
		PRINTF_DEBUG("mqtt_not_configured\n");
		strcpy(mqtt_cfg_nvm.uri, MQTT_DEFAULT_BROKER);
		strcat(mqtt_cfg_nvm.uri, ":");
		strcat(mqtt_cfg_nvm.uri, mqtt_port_str);
		strcpy(mqtt_cfg_nvm.username, MQTT_DEFAULT_USER);
		strcpy(mqtt_cfg_nvm.password, MQTT_DEFAULT_PWD);
		mqtt_cfg_nvm.keepalive = MQTT_KEEP_ALIVE_DEFAULT_SEC;
	}


#else

	char mqtt_port_str[10] = {0};
	sprintf(mqtt_port_str,"%d",MQTT_DEFAULT_PORT);
	//PRINTF_DEBUG("mqtt_not_configured\n");
	strcpy(mqtt_cfg_nvm.uri, MQTT_DEFAULT_BROKER);
	strcat(mqtt_cfg_nvm.uri, ":");
	strcat(mqtt_cfg_nvm.uri, mqtt_port_str);
	mqtt_cfg_nvm.keepalive = MQTT_KEEP_ALIVE_DEFAULT_SEC;


	if(C_SUCCESS != NVM__ReadString(MQTT_USER, mqtt_cfg_nvm.username, &user_len)
		|| C_SUCCESS != NVM__ReadString(MQTT_PASS, mqtt_cfg_nvm.password, &pass_len)){
		printf("mqtt user pass check default \n");
		strcpy(mqtt_cfg_nvm.username, MQTT_DEFAULT_USER);
		strcpy(mqtt_cfg_nvm.password, MQTT_DEFAULT_PWD);
	}

	if(C_SUCCESS == NVM__ReadU8Value(MQTT_URL, &mqtt_url) && CONFIGURED == mqtt_url){
		printf("mqtt url port check ok \n");
		strcpy(mqtt_cfg_nvm.uri, WiFi__GetCustomConfig().mqtt_server_addr);
		strcat(mqtt_cfg_nvm.uri, ":");
		strcat(mqtt_cfg_nvm.uri, WiFi__GetCustomConfig().mqtt_server_port);
	}

	if(C_SUCCESS == NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status) && CONFIGURED == gw_config_status){
		size_t gw_config_len;
		req_set_gw_config_t gw_config_nvm = {0};
		printf("mqtt keepalive val check ok \n");
		NVM__ReadBlob(SET_GW_PARAM_NVM,(void*)&gw_config_nvm,&gw_config_len);

		mqtt_cfg_nvm.keepalive = gw_config_nvm.mqttKeepAliveInterval;
	}
#endif

	mqtt_cfg_nvm.cert_pem = Sys__GetCert(CERT_1);
	PRINTF_DEBUG("uri= %s\n",mqtt_cfg_nvm.uri);
	PRINTF_DEBUG("username= %s\n",mqtt_cfg_nvm.username);
	PRINTF_DEBUG("password= %s\n",mqtt_cfg_nvm.password);
	PRINTF_DEBUG("keepalive= %d\n",mqtt_cfg_nvm.keepalive);

    WiFi__WaitConnection();
    s_mqtt_event_group = xEventGroupCreate();

    mqtt_client_init(&mqtt_cfg_nvm);
    C_RES err = mqtt_client_start();

    MQTT_BITS = xEventGroupWaitBits(s_mqtt_event_group, MQTT_CONNECTED_BIT | MQTT_DISCONNECTED_BIT, true, false, 30000/portTICK_RATE_MS);

    if( ( MQTT_BITS & MQTT_DISCONNECTED_BIT ) != 0 ){

    	err = mqtt_client_stop();
		mqtt_cfg_nvm.cert_pem = Sys__GetCert(CERT_2);
	//	memset((void*)mqtt_client,0,sizeof(esp_mqtt_client_handle_t));
		mqtt_client_init(&mqtt_cfg_nvm);
		err = mqtt_client_start();
    }

    return err;
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
	vEventGroupDelete(s_mqtt_event_group);
}


db_values val_array[10];
C_TIME Get_SamplingTime(C_UINT16 index){
	PRINTF_DEBUG("Get_SamplingTime index: %d, t:%d\n", index, val_array[index].t);
	return val_array[index].t;}
C_CHAR a[1];
C_CHAR* Get_Alias(C_UINT16 index, C_UINT16 i){
	PRINTF_DEBUG("Get_Alias index: %d, i: %d, alias:%s\n", index, i, val_array[index].vls[i].alias);
	return val_array[index].vls[i].alias;}
C_CHAR* Get_Value(C_UINT16 index, C_UINT16 i){
	PRINTF_DEBUG("Get_Value index: %d, i: %d, vls:%s\n", index, i, val_array[index].vls[i].values);
	return val_array[index].vls[i].values;}

void CBOR_CreateSendValues(values_buffer_t *values_buffer, values_buffer_timing_t *time_values_buff, uint16_t values_buffer_index, uint16_t values_buffer_read_section)
{
	uint32_t i, j;
	j = 0;
	PRINTF_DEBUG("CBOR_CreateSendValues values_buffer_read_section: %d, values_buffer_index: %d\n", values_buffer_read_section, values_buffer_index);
	for(i = 0; i < values_buffer_read_section - 1; i++){

		val_array[i].t = time_values_buff[i].t_stop;

		while(time_values_buff[i].index == values_buffer[j].index){
			PRINTF_DEBUG("i: %d, j: %d, alias: %d, value: %Lf, err:%d\n", i, j, values_buffer[j].alias, values_buffer[j].value, values_buffer[j].info_err);
			PRINTF_DEBUG("time index %d, values index %d\n", time_values_buff[i].index, values_buffer[j].index);
			itoa(values_buffer[j].alias, (char*)val_array[i].vls[j].alias, 10);
			if(0 != values_buffer[j].info_err)
				memcpy(val_array[i].vls[j].values, "", 1);
			else{
				itoa(values_buffer[j].value, (char*)val_array[i].vls[j].values, 10);
			}
			j++;
		}

	}
	CBOR_SendFragmentedValues(0, j);

}

void MQTT_FlushValues(void){

	CBOR_CreateSendValues(PollEngine__GetValuesBuffer(), PollEngine__GetTimeBuffer(),
			PollEngine__GetValuesBufferIndex(),	PollEngine__GetTimerBufferIndex());


	mqtt_time.cbor_values =  RTC_Get_UTC_Current_Time();

	PollEngine__ResetValuesBuffer();

}

void MQTT_Status(void)
{
	if(RTC_Get_UTC_Current_Time() > (mqtt_time.cbor_status + 240))   // Utilities__GetGWConfigData()->statusPeriod
	{
		printf("Sending STATUS CBOR \n\n");
		CBOR_SendStatus();
		mqtt_time.cbor_status = RTC_Get_UTC_Current_Time();
	}
}

void MQTT_Values(void)
{
	if(RTC_Get_UTC_Current_Time() > (mqtt_time.cbor_values + 240)) // Utilities__GetGWConfigData()->valuesPeriod
	{
		MQTT_FlushValues();
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
	Get_Gateway_ID((C_SBYTE*)&dev_id);
	
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s", dev_id, (C_SCHAR*)topic);
	printf("topic = %s\n",mqtt_topic);

	return (C_MQTT_TOPIC*)mqtt_topic;
}

C_RES EventHandler(mqtt_event_handle_t event)
{

	int msg_id;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:

        	xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);

            DEBUG_MQTT("MQTT_EVENT_CONNECTED");
            msg_id = mqtt_client_subscribe((C_SCHAR*)MQTT_GetUuidTopic("/req"), 0);
            DEBUG_MQTT("sent subscribe successful, msg_id=%d", msg_id);

            msg_id = mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/connected"), (C_SBYTE*)"1", 0, 1, 1);
            DEBUG_MQTT("sent publish successful, msg_id=%d", msg_id);

            if(0 == mqtt_init)
			{

                #ifdef IS_A_WIFI_GATEWAY
            	led_status = LED_ON;
                #endif



				CBOR_SendHello();
				MQTT_Status();
				mqtt_init = 1;
            	mqtt_time.cbor_status = RTC_Get_UTC_Current_Time();
				mqtt_time.cbor_values = RTC_Get_UTC_Current_Time();
    		}

        	break;
        case MQTT_EVENT_DISCONNECTED:
        	xEventGroupSetBits(s_mqtt_event_group, MQTT_DISCONNECTED_BIT);
            DEBUG_MQTT("MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            DEBUG_MQTT("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            DEBUG_MQTT("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            DEBUG_MQTT("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
        {
        	C_GATEWAY_ID dev_id;
			Get_Gateway_ID(&dev_id);
		
            DEBUG_MQTT("MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            C_MQTT_TOPIC parsed_topic;
            memset((void*)parsed_topic,0,sizeof(parsed_topic));
            C_UINT16 rec_topic_len = event->topic_len - strlen(dev_id);	//wifi_mac_address_gw_str
            C_SCHAR* rec_topic = event->topic + strlen(dev_id);	//wifi_mac_address_gw_str

            sprintf(parsed_topic,"%.*s", rec_topic_len, rec_topic);
            printf("parsed_topic = %s\n", parsed_topic);

			if(strcmp(parsed_topic, "/req") == 0)
			{
				printf("/req found_topic\n");
				CBOR_ReqTopicParser((C_CHAR*)event->data, event->data_len);
			}
        }
            break;
        case MQTT_EVENT_ERROR:
            DEBUG_MQTT("MQTT_EVENT_ERROR");
            break;
        default:
            DEBUG_MQTT("Other event id:%d", event->event_id);
            break;
    }
    	
	return C_SUCCESS;
}

C_BYTE MQTT_GetFlags(void){
	return mqtt_init;
}
