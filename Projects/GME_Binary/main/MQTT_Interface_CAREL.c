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
#include "nvm_CAREL.h"
#include "wifi.h"
#include "sys_CAREL.h"
#include "utilities_CAREL.h"
#include "polling_CAREL.h"
#include "polling_IS.h"
#include "Led_Manager_IS.h"
#include "sys_IS.h"
#include "mobile.h"
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
static uint32_t mqtt_periodic_time;
static EventBits_t MQTT_BITS;

/**
 * @brief MQTT_Start
 *         
 * @param none
 * @return C_SUCCESS/C_FAIL
 */
C_RES MQTT_Start(void)
{

    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("MQTT_Init\n");
    #endif

	mqtt_config_t mqtt_cfg_nvm = {0};
	size_t pass_len = 0, user_len = 0;
	uint8_t gw_config_status, mqtt_url;
	uint8_t cert_num;

	char tmp_mqtt_broker[SERVER_SIZE];
	char tmp_mqtt_port[32];		//TODO check max size
	char tmp_mqtt_user[USERNAME_SIZE];
	char tmp_mqtt_pssw[PASSWORD_SIZE];

	strcpy(mqtt_cfg_nvm.uri, GetMqttBroker(tmp_mqtt_broker));             // MQTT_DEFAULT_BROKER
	strcat(mqtt_cfg_nvm.uri, ":");
	strcat(mqtt_cfg_nvm.uri, GetMqttPort(tmp_mqtt_port));   			   // mqtt_port_str
	mqtt_cfg_nvm.keepalive = MQTT_KEEP_ALIVE_DEFAULT_SEC;

	strcpy(mqtt_cfg_nvm.username, GetMqttUser(tmp_mqtt_user));    // MQTT_DEFAULT_USER
	strcpy(mqtt_cfg_nvm.password, GetMqttPassword(tmp_mqtt_pssw));     // MQTT_DEFAULT_PWD

    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
    PRINTF_DEBUG("mqtt_broker %s\n", GetMqttBroker(tmp_mqtt_broker));
    PRINTF_DEBUG("mqtt user   %s\n", GetMqttUser(tmp_mqtt_user));
    PRINTF_DEBUG("mqtt passw  %s\n", GetMqttPassword(tmp_mqtt_pssw));
    #endif




	if(C_SUCCESS == NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status) && CONFIGURED == gw_config_status){
		size_t gw_config_len;
		req_set_gw_config_t gw_config_nvm = {0};
		NVM__ReadBlob(SET_GW_PARAM_NVM,(void*)&gw_config_nvm,&gw_config_len);

		mqtt_cfg_nvm.keepalive = gw_config_nvm.mqttKeepAliveInterval;
	}

	if(C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
		cert_num = CERT_1;
	mqtt_cfg_nvm.cert_pem = Sys__GetCert(cert_num);

	//check if username == '?', then use deviceid as username (to make spiffs image independent from MAC or IMEI)
	if (mqtt_cfg_nvm.username[0] == 0x3F) {
		if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
			strcpy(mqtt_cfg_nvm.username, Utilities__GetMACAddr());
		else if (PLATFORM(PLATFORM_DETECTED_2G))
			strcpy(mqtt_cfg_nvm.username, Mobile__GetImeiCode());
	}


    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("attempt to connect with certificate id %d\n", cert_num);

	PRINTF_DEBUG("uri= %s\n",mqtt_cfg_nvm.uri);
	PRINTF_DEBUG("username= %s\n",mqtt_cfg_nvm.username);
	PRINTF_DEBUG("password= %s\n",mqtt_cfg_nvm.password);
	PRINTF_DEBUG("keepalive= %d\n",mqtt_cfg_nvm.keepalive);
    #endif

    Radio__WaitConnection();
    s_mqtt_event_group = xEventGroupCreate();

    RTC_Set_UTC_MQTTConnect_Time();

    mqtt_client_init(&mqtt_cfg_nvm);

    C_RES err;

    err = mqtt_client_start();

    //TODO Bilato da gestire
    switch (err)
    {
       case	ESP_OK:
    	   PRINTF_DEBUG("mqtt_client_start OK\n");
    	   break;

       case ESP_ERR_INVALID_ARG:
    	   PRINTF_DEBUG("ESP_ERR_INVALID_ARG \n");
    	   break;

       case ESP_FAIL:
    	   PRINTF_DEBUG("ESP_FAIL \n");
    	   break;

       default:
    	   PRINTF_DEBUG("mqtt_client_start unknow code\n");
    	   break;

    }


    MQTT_BITS = xEventGroupWaitBits(s_mqtt_event_group, MQTT_CONNECTED_BIT | MQTT_DISCONNECTED_BIT, true, false, 30000/portTICK_RATE_MS);

    if( ( MQTT_BITS & MQTT_DISCONNECTED_BIT ) != 0 ){

    	err = mqtt_client_stop();
    	// try connecting using the other certificate
    	cert_num = (cert_num == CERT_1) ? CERT_2 : CERT_1;
		mqtt_cfg_nvm.cert_pem = Sys__GetCert(cert_num);

#ifdef __DEBUG_MQTT_INTERFACE_LEV_2
		PRINTF_DEBUG("another attempt to connect with certificate id %d\n", cert_num);
#endif
	//	memset((void*)mqtt_client,0,sizeof(esp_mqtt_client_handle_t));
		mqtt_client_init(&mqtt_cfg_nvm);
		err = mqtt_client_start();
    }
    // save in nvm the index of the last certificate we used to connect
    NVM__WriteU8Value(MB_CERT_NVM, cert_num);

    return err;
}


void MQTT_Stop(void)
{
    #ifdef __DEBUG_MQTT_INTERFACE_LEV_1
	PRINTF_DEBUG("mqtt_stop\n");
    #endif
	mqtt_client_stop();
	vEventGroupDelete(s_mqtt_event_group);
}

void CBOR_CreateSendValues(values_buffer_t *values_buffer, uint16_t values_buffer_count)
{
	uint32_t i, vals_for_ts, firstval_for_ts;
	vals_for_ts = 0;
	firstval_for_ts = 0;

    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("CBOR_CreateSendValues: values_buffer_count: %d\n", values_buffer_count);
    #endif

	if (values_buffer_count != 0)
		vals_for_ts = 1;
	else
		CBOR_SendValues(0, 0, -1); // empty packet to be sent every pva seconds (if no value sent in pva seconds before)

	for (i = 0; i < values_buffer_count; i++)
	{
#ifdef __DEBUG_MQTT_INTERFACE_LEV_2
		PRINTF_DEBUG("i: %d, alias: %d, value: %Lf, err:%d\n", i, values_buffer[i].alias, values_buffer[i].value, values_buffer[i].info_err);
		PRINTF_DEBUG("time %d\n", values_buffer[i].t);
#endif
		while(values_buffer[i].t == values_buffer[i+1].t) {
			vals_for_ts++;   // j is the number of values with same t
			i++;
		}
		CBOR_SendFragmentedValues(firstval_for_ts, vals_for_ts);
		firstval_for_ts += vals_for_ts;
		vals_for_ts = 1;

	}
}

void MQTT_FlushValues(void){

	if (MQTT_GetFlags() == 1) {
		CBOR_CreateSendValues(PollEngine__GetValuesBuffer(), PollEngine__GetValuesBufferCount());
		PollEngine__ResetValuesBuffer();
	}
}

void MQTT_Alarms(c_cboralarms alarms)
{
	CBOR_SendAlarms(alarms);
}

void MQTT_PeriodicTasks(void)
{
	// send status payload on all platforms every pst seconds (configurable)
	if(RTC_Get_UTC_Current_Time() > (mqtt_periodic_time + Utilities__GetStatusPeriod())) {
		#ifdef __DEBUG_MQTT_INTERFACE_LEV_2
		printf("Sending STATUS CBOR every %d seconds\n\n", Utilities__GetStatusPeriod());
		#endif
		CBOR_SendStatus();
		mqtt_periodic_time = RTC_Get_UTC_Current_Time();
	}
	// send mobile payload only for 2G model every GW_MOBILE_TIME seconds (fixed)
	else if(PLATFORM(PLATFORM_DETECTED_2G)) {
		if(RTC_Get_UTC_Current_Time() > (mqtt_periodic_time + GW_MOBILE_TIME)) {
			#ifdef __DEBUG_MQTT_INTERFACE_LEV_2
			printf("Sending MOBILE CBOR \n\n");
			#endif
			CBOR_SendMobile();
			mqtt_periodic_time = RTC_Get_UTC_Current_Time();
		}
	}
}

C_MQTT_TOPIC* MQTT_GetUuidTopic(C_SCHAR* topic)
{
	
	C_GATEWAY_ID dev_id;
	Get_Gateway_ID((C_SBYTE*)&dev_id);
	
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s", dev_id, (C_SCHAR*)topic);

    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("topic = %s\n",mqtt_topic);
    #endif

	return (C_MQTT_TOPIC*)mqtt_topic;
}

C_RES EventHandler(mqtt_event_handle_t event)
{

	int msg_id;
	static int previous_poll_engine_status = STOPPED;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:

        	xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);

            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("MQTT_EVENT_CONNECTED");
            #endif
            msg_id = mqtt_client_subscribe((C_SCHAR*)MQTT_GetUuidTopic("/req"), 0);

            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("sent subscribe successful, msg_id=%d", msg_id);
            #endif

            if(2 == mqtt_init){
            	mqtt_client_reinit();
            	mqtt_init = 1;
            }
            C_CHAR conn_buf[25];
            size_t conn_len = CBOR_Connected(conn_buf, 1);

            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            for (int i=0;i<conn_len;i++){
                PRINTF_DEBUG("%02X ", conn_buf[i]);
            }
            PRINTF_DEBUG("\n");
            #endif

            msg_id = mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/connected"), conn_buf, conn_len, QOS_1, RETAIN);

            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("sent publish successful, msg_id=%d", msg_id);
            #endif

            if(0 == mqtt_init)
			{
            	Update_Led_Status(LED_STAT_MQTT_CONN, LED_STAT_ON);

				CBOR_SendHello();
				MQTT_PeriodicTasks();
				mqtt_init = 1;
				mqtt_periodic_time = RTC_Get_UTC_Current_Time();
			}

        	break;

        case MQTT_EVENT_DISCONNECTED:
        	Update_Led_Status(LED_STAT_MQTT_CONN, LED_STAT_OFF);
        	if(mqtt_init != 0)	// change state only if gme was already connected
        		mqtt_init = 2;
        	xEventGroupSetBits(s_mqtt_event_group, MQTT_DISCONNECTED_BIT);
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_1
            DEBUG_MQTT("MQTT_EVENT_DISCONNECTED");
            #endif
            break;

        case MQTT_EVENT_SUBSCRIBED:
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            #endif
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            #endif
            break;

        case MQTT_EVENT_PUBLISHED:
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            #endif
            break;

        case MQTT_EVENT_DATA:
        {
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
            DEBUG_MQTT("MQTT_EVENT_DATA");
            PRINTF_DEBUG("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            PRINTF_DEBUG("DATA=%.*s\r\n", event->data_len, event->data);
            #endif

        	int ret = 0;
            previous_poll_engine_status = PollEngine_GetEngineStatus_CAREL();
            PollEngine_StopEngine_CAREL();

            C_GATEWAY_ID dev_id;
            Get_Gateway_ID(&dev_id);

            C_MQTT_TOPIC parsed_topic;
            memset((void*)parsed_topic,0,sizeof(parsed_topic));
            C_UINT16 rec_topic_len = event->topic_len - strlen(dev_id);	//wifi_mac_address_gw_str
            C_SCHAR* rec_topic = event->topic + strlen(dev_id);	//wifi_mac_address_gw_str

            sprintf(parsed_topic,"%.*s", rec_topic_len, rec_topic);

            #ifdef __DEBUG_MQTT_INTERFACE_LEV_1
            PRINTF_DEBUG("parsed_topic = %s\n", parsed_topic);
            #endif

			if(strcmp(parsed_topic, "/req") == 0)
			{
                #ifdef __DEBUG_MQTT_INTERFACE_LEV_1
				PRINTF_DEBUG("/req found_topic\n");
                #endif
				ret = CBOR_ReqTopicParser((C_CHAR*)event->data, event->data_len);
			}

			if(previous_poll_engine_status == RUNNING && ret == 0)  // do not restart polling if received command was a stop polling
				PollEngine_StartEngine_CAREL();
        }
            break;
        case MQTT_EVENT_ERROR:
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_1
            DEBUG_MQTT("MQTT_EVENT_ERROR");
            #endif
            break;

        default:
            #ifdef __DEBUG_MQTT_INTERFACE_LEV_1
            DEBUG_MQTT("Other event id:%d", event->event_id);
            #endif
            break;
    }
    	
	return C_SUCCESS;
}

C_BYTE MQTT_GetFlags(void){
	return mqtt_init;
}
