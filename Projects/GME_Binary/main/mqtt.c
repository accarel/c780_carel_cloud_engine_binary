/*
 * mqtt.c
 *
 *  Created on: Jul 1, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */


#include"mqtt.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"
#include "wifi.h"
#include "utilities.h"
#include "nvm.h"
#include "file_system.h"
#include "sys.h"

#define TOPICS_NUM_MAX (sizeof(JsRxTable)/sizeof(js_rx_table_t))


static const char *TAG = "MQTT";

EventGroupHandle_t s_mqtt_event_group;
const int MQTT_CONNECTED_BIT = BIT0;
const int MQTT_DISCONNECTED_BIT = BIT1;

static mqtt_times_t mqtt_time;
static mqtt_flags_t mqtt_flags = {0};
static esp_mqtt_client_handle_t mqtt_client;
static uint8_t MQTTConfigStatus = DEFAULT;
static EventBits_t MQTT_BITS;
//static char *mqtt_topic = NULL;
static char mqtt_topic[100] = {0};

static void MQTT__Status(void);
static void MQTT__Values(void);

#if 0
static void MQTT__Status(void){
	if(Get_UTC_Current_Time() > (mqtt_time.js_status + Utilities__GetGWConfigData()->statusPeriod)){
		PRINTF_DEBUG("Sending STATUS json \n\n");
		JSON__CreateSendStatus(MQTT__GetClient());
		mqtt_time.js_status = Get_UTC_Current_Time();
		PRINTF_DEBUG("mqtt_time.js_status = %d\n",mqtt_time.js_status);
		PRINTF_DEBUG("statusPeriod = %d\n",Utilities__GetGWConfigData()->statusPeriod);
	}
}

static void MQTT__Values(void){
	if(Get_UTC_Current_Time() > (mqtt_time.js_values + Utilities__GetGWConfigData()->valuesPeriod)){
		MQTT__FlushValues();
		mqtt_time.js_values = Get_UTC_Current_Time();
	}
}
#endif

#if 1
char* MQTT__GetUuidTopic(const char* topic){
	if(NULL != mqtt_topic){
	}
#if (NETWORK_INTERFACE == WIFI_INTERFACE)
	char* mac_addr_str = Utilities__GetMACAddr();
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s",mac_addr_str, topic);
	PRINTF_DEBUG("topic = %s\n",mqtt_topic);

#elif (NETWORK_INTERFACE == GSM_INTERFACE)
	char* imei_code_str = Utilities__GetIMEICode();
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s",imei_code_str, topic);
	PRINTF_DEBUG("topic = %s\n",mqtt_topic);
#endif

	return mqtt_topic;
}

#elif


char* MQTT__GetUuidTopic(const char* topic){
	if(NULL != mqtt_topic){
		free(mqtt_topic);
	}
#if (NETWORK_INTERFACE == WIFI_INTERFACE)
	char* mac_addr_str = Utilities__GetMACAddr();
	mqtt_topic = malloc(strlen(mac_addr_str) + sizeof(topic) + 1);
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s",mac_addr_str, topic);
	PRINTF_DEBUG("topic = %s\n",mqtt_topic);

#elif (NETWORK_INTERFACE == GSM_INTERFACE)
	char* imei_code_str = Utilities__GetIMEICode();
	mqtt_topic = malloc(strlen(imei_code_str) + sizeof(topic) + 1);
	memset((void*)mqtt_topic,0,sizeof(mqtt_topic));
	sprintf(mqtt_topic,"%s%s",imei_code_str, topic);
	PRINTF_DEBUG("topic = %s\n",mqtt_topic);
#endif

	return mqtt_topic;
}

#endif


static esp_err_t EventHandler(esp_mqtt_event_handle_t event)
{
#if 0
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:

        	xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, MQTT__GetUuidTopic("/req"), 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_publish(client, MQTT__GetUuidTopic("/connected"), "1", 0, 1, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            if(0 == mqtt_flags.init){
 //           	JSON__CreateSendHello(client);
            	MQTT__Status();
            	mqtt_flags.init = 1;
            	mqtt_time.js_status = Get_UTC_Current_Time();
				mqtt_time.js_values = Get_UTC_Current_Time();
    		}

        	break;
        case MQTT_EVENT_DISCONNECTED:
        	xEventGroupSetBits(s_mqtt_event_group, MQTT_DISCONNECTED_BIT);
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
        {
        	char* uuid;

		#if (NETWORK_INTERFACE == WIFI_INTERFACE)
        	uuid = Utilities__GetMACAddr();

		#elif (NETWORK_INTERFACE == GSM_INTERFACE)
        	uuid = Utilities__GetIMEICode();
		#endif

            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            PRINTF_JSON_MQTT(("TOPIC=%.*s\r\n", event->topic_len, event->topic))
            PRINTF_JSON_MQTT(("DATA=%.*s\r\n", event->data_len, event->data))

            char *parsed_topic;
            parsed_topic = malloc(event->topic_len + 1);
            memset((void*)parsed_topic,0,sizeof(parsed_topic));
            uint16_t rec_topic_len = event->topic_len - strlen(uuid);	//wifi_mac_address_gw_str
            char *rec_topic = event->topic + strlen(uuid);	//wifi_mac_address_gw_str


            sprintf(parsed_topic,"%.*s", rec_topic_len, rec_topic);
            PRINTF_DEBUG("parsed_topic = %s\n", parsed_topic);

			if(strcmp(parsed_topic, "/req") == 0){
				PRINTF_DEBUG("/req found_topic\n");
				JSON__ReqTopicParser(event->data, event->data_len);
			}

            free(parsed_topic);
        }

            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }

#endif
    //GME__CheckHTMLConfig();
    return ESP_OK;

}


esp_err_t MQTT__Start(void)
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




	if(ESP_OK != NVM__ReadString(MQTT_USER, mqtt_cfg_nvm.username, &user_len)
		|| ESP_OK != NVM__ReadString(MQTT_PASS, mqtt_cfg_nvm.password, &pass_len)){
		printf("mqtt user pass check default \n");
		strcpy(mqtt_cfg_nvm.username, MQTT_DEFAULT_USER);
		strcpy(mqtt_cfg_nvm.password, MQTT_DEFAULT_PWD);

	}

	if(ESP_OK == NVM__ReadU8Value(MQTT_URL, &mqtt_url) && CONFIGURED == mqtt_url){
		printf("mqtt url port check ok \n");
		strcpy(mqtt_cfg_nvm.uri, WiFi__GetCustomConfig().mqtt_server_addr);
		strcat(mqtt_cfg_nvm.uri, ":");
		strcat(mqtt_cfg_nvm.uri, WiFi__GetCustomConfig().mqtt_server_port);

	}

	if(ESP_OK == NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status) && CONFIGURED == gw_config_status)
	{
	#if 0
		size_t gw_config_len;
		req_set_gw_config_t gw_config_nvm = {0};
		printf("mqtt keepalive val check ok \n");
		NVM__ReadBlob(SET_GW_PARAM_NVM,(void*)&gw_config_nvm,&gw_config_len);

		mqtt_cfg_nvm.keepalive = gw_config_nvm.mqttKeepAliveInterval;

	#endif

	}
#endif


	esp_mqtt_client_config_t mqtt_cfg = {
		.event_handle = EventHandler,
		.uri = mqtt_cfg_nvm.uri,
		.username = mqtt_cfg_nvm.username,
		.password = mqtt_cfg_nvm.password,
		.keepalive =  mqtt_cfg_nvm.keepalive,
		.lwt_topic = MQTT__GetUuidTopic("/connected"),
		.lwt_msg = "0",
		.lwt_qos = 0,
		.lwt_retain = 0,
		.cert_pem =  Sys__GetCert(CERT_1),
		.task_stack = 8192,
	};

	PRINTF_DEBUG("uri= %s\n",mqtt_cfg.uri);
	PRINTF_DEBUG("username= %s\n",mqtt_cfg.username);
	PRINTF_DEBUG("password= %s\n",mqtt_cfg.password);
	PRINTF_DEBUG("keepalive= %d\n",mqtt_cfg.keepalive);

    WiFi__WaitConnection();
    s_mqtt_event_group = xEventGroupCreate();

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_err_t err = esp_mqtt_client_start(mqtt_client);


    MQTT_BITS = xEventGroupWaitBits(s_mqtt_event_group, MQTT_CONNECTED_BIT | MQTT_DISCONNECTED_BIT, true, false, 30000/portTICK_RATE_MS);

    if( ( MQTT_BITS & MQTT_DISCONNECTED_BIT ) != 0 ){

    	err = esp_mqtt_client_stop(mqtt_client);
		mqtt_cfg.cert_pem = Sys__GetCert(CERT_2);
		memset((void*)mqtt_client,0,sizeof(esp_mqtt_client_handle_t));
		mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
		err = esp_mqtt_client_start(mqtt_client);
    }



    return err;
}

void MQTT__Stop(void){
	PRINTF_DEBUG("mqtt_stop\n");
	esp_mqtt_client_destroy(mqtt_client);
	vEventGroupDelete(s_mqtt_event_group);
}


void MQTT__FlushValues(void){
#if 0
	JSON__CreateSendValues(MQTT__GetClient(),
			PollEngine__GetValuesBuffer(),
			PollEngine__GetTimeBuffer(),
			PollEngine__GetValuesBufferIndex(),
			PollEngine__GetTimerBufferIndex());

	mqtt_time.js_values = 0;

	PollEngine__ResetValuesBuffer();
#endif
}


void MQTT__Alarms( uint16_t alias, uint32_t tstart,  uint32_t tstop, uint8_t alarm_issue){
#if 0
	JSON__CreateSendAlarms(MQTT__GetClient(), alias, tstart,  tstop, alarm_issue);
#endif
}


void MQTT__PeriodicTasks(void){
//	MQTT__Values();
//	MQTT__Status();
}

mqtt_flags_t MQTT__GetFlags(void){
	return mqtt_flags;
}

configuration_t MQTT__GetConfigStatus (void){
	return MQTTConfigStatus;
}

esp_mqtt_client_handle_t MQTT__GetClient (void){
	return mqtt_client;
}

void MQTT__InitValuesTimer(void){
	mqtt_time.js_values = Get_UTC_Current_Time();
}

