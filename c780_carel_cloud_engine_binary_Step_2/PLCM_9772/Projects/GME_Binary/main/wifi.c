/**
 * @file wifi.h
 * @author ataayoub, CAREL
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the WiFi
 */

#include "wifi.h"
#include "nvm_CAREL.h"
#include <tcpip_adapter.h>
#include"http_server_IS.h"
#include "utilities_CAREL.h"
#include "MQTT_Interface_CAREL.h"
#include "sys_IS.h"
#include "esp_wps.h"
#include "lwip/inet.h"
#include "IO_Port_IS.h"

#include "esp_netif_types.h"

static const char *TAG = "wifi";

static connection_status_t STAStatus = DISCONNECTED;

EventGroupHandle_t s_wifi_event_group;
const int CONNECTED_BIT = BIT0;

#define AP_SHOWN_IN_PAGE	11
char AAP[AP_SHOWN_IN_PAGE][33];

//Debugging
static int test2=0;

//Variables
static config_sm_t config_sm = CHECK_FOR_CONFIG;
static httpd_handle_t AP_http_server = NULL;
static uint8_t wifi_conf;

static html_config_param_t WiFiConfig = {
		//.gateway_mode = 0,
		.ap_ssid = AP_DEF_SSID,
		.ap_pswd = AP_DEF_PSSWD,
		.ap_ssid_hidden = AP_DEF_SSID_HIDDEN,
		.ap_ip = AP_DEF_IP,
		//.ap_dhcp_mode = AP_DEF_DHCP,
		//.ap_dhcp_ip = AP_DEF_DHCP_BASE,
		.sta_ssid = "",
		.sta_encryption = "",
		.sta_pswd= "",
		.sta_dhcp_mode = 1,
		.sta_static_ip = "",
		.sta_netmask = "",
		.sta_gateway_ip = "",
		.sta_primary_dns = "",
		.sta_secondary_dns = "",
};

/**
 * @brief SaveAvailableAPs
 *        Save the access point name
 *
 * @param  uint8_t* ssid
 * @param  uint8_t index
 * @return none
 */
void SaveAvailableAPs(uint8_t* ssid, uint8_t index){
	strcpy(AAP[index],(char*)ssid);
}

/**
 * @brief GetAvailableAPs
 *       return the access point name
 *
 * @param  uint8_t index
 * @return char*
 */
char* GetAvailableAPs(uint8_t index){
	return AAP[index];
}

static C_INT32 TimerForAPConnection = 0;
static uint16_t connect_attempt = 0;

/**
 * @brief event_handler
 *        manage the access point event
 *
 * @param  void *ctx
 * @param  system_event_t *event
 * @return esp_err_t
 */
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	uint16_t apCount;
	switch(event->event_id) {

//AP MODE
		case SYSTEM_EVENT_AP_START:
			ESP_LOGI(TAG, "SYSTEM_EVENT_AP_START");
		break;

		case SYSTEM_EVENT_AP_STAIPASSIGNED:
			ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STAIPASSIGNED");
// in case of AP mode, GME does not receive an IP station address but event handler steps in too
// so, avoid doing a scan of available APs when in AP mode
#ifndef GW_GSM_WIFI
			ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
					MAC2STR(event->event_info.sta_connected.mac),
					event->event_info.sta_connected.aid);

			PRINTF_DEBUG("wait for some time before scanning\n");
			Sys__Delay(3000);
			connect_attempt = 21;

			// Let us test a WiFi scan ...
			wifi_scan_config_t scanConf = {
				.ssid = NULL,
				.bssid = NULL,
				.channel = 0,
				.show_hidden = true
			};
			ESP_ERROR_CHECK(esp_wifi_scan_start(&scanConf, true));	//The true parameter cause the function to block until
																	//the scan is done.

#endif
		break;

		case SYSTEM_EVENT_AP_STADISCONNECTED:
			ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STADISCONNECTED");
// in case of AP mode, GME does not receive an IP station address but event handler steps in too
// so, avoid doing a scan of available APs when in AP mode
#ifndef GW_GSM_WIFI
			ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
					MAC2STR(event->event_info.sta_disconnected.mac),
					event->event_info.sta_disconnected.aid);
			if(WIFI__GetSTAStatus() == DISCONNECTED){
				connect_attempt = 0;
				StartTimerForAPConnection();
				ESP_ERROR_CHECK(esp_wifi_connect());
			}
#endif
		break;

		case SYSTEM_EVENT_AP_STACONNECTED:
			ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STACONNECTED");
			ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
					MAC2STR(event->event_info.sta_disconnected.mac),
					event->event_info.sta_disconnected.aid);
		//	connect_attempt = 21;
		break;

//STA MODE

		case SYSTEM_EVENT_STA_START:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
			wifi_config_t wifi_config_STA;
			ESP_ERROR_CHECK(esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config_STA));
			if(wifi_config_STA.ap.ssid[0] != '\0') // only attempt to connect if a ssid is specified
				ESP_ERROR_CHECK(esp_wifi_connect());
		break;

		case SYSTEM_EVENT_STA_CONNECTED:
			connect_attempt = 0;
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED");
		break;

		case SYSTEM_EVENT_STA_GOT_IP:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
			ESP_LOGI(TAG, "Got IP: '%s'",
			ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

//			connect_attempt = 0;
			xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);

			WIFI__SetSTAStatus(CONNECTED);

			P_COV_LN;
		break;

		case SYSTEM_EVENT_STA_DISCONNECTED:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED %d", connect_attempt);

			// try to reconnect for about one minute, then make a 2 minute break
			if(connect_attempt < 20) {
				connect_attempt++;
				ESP_ERROR_CHECK(esp_wifi_connect());
			}
			else{
				connect_attempt = 21;
				//ESP_ERROR_CHECK(esp_wifi_disconnect());
				StartTimerForAPConnection();
			}

			GME__CheckHTMLConfig();

			xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
			WIFI__SetSTAStatus(DISCONNECTED);
			P_COV_LN;
		break;

		case SYSTEM_EVENT_SCAN_DONE:
			ESP_LOGI(TAG, "SYSTEM_EVENT_SCAN_DONE");
			esp_wifi_scan_get_ap_num(&apCount);

			if (apCount == 0) {
				return ESP_OK;
			}
			wifi_ap_record_t *list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);
			ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&apCount, list));
			int i;
			int l=0;


			PRINTF_DEBUG("Number of access points found: %d\n",event->event_info.scan_done.number);
			PRINTF_DEBUG("======================================================================\n");
			PRINTF_DEBUG("             SSID             |    RSSI    |           AUTH           \n");
			PRINTF_DEBUG("======================================================================\n");


			for (i=0; i<apCount; i++) {
				 char *authmode;
				 switch(list[i].authmode) {
					case WIFI_AUTH_OPEN:
					   authmode = "WIFI_AUTH_OPEN";
					   break;
					case WIFI_AUTH_WEP:
					   authmode = "WIFI_AUTH_WEP";
					   break;
					case WIFI_AUTH_WPA_PSK:
					   authmode = "WIFI_AUTH_WPA_PSK";
					   break;
					case WIFI_AUTH_WPA2_PSK:
					   authmode = "WIFI_AUTH_WPA2_PSK";
					   break;
					case WIFI_AUTH_WPA_WPA2_PSK:
					   authmode = "WIFI_AUTH_WPA_WPA2_PSK";
					   break;
					default:
					   authmode = "Unknown";
					   break;
				 }


				 PRINTF_DEBUG("%26.26s    |    % 4d    |    %22.22s\n",list[i].ssid, list[i].rssi, authmode);


				 /* only protected AP will be listed */
				 if (  list[i].ssid[0] != '\0' &&
					 ((list[i].authmode==WIFI_AUTH_WPA_PSK) ||
					  (list[i].authmode==WIFI_AUTH_WPA2_PSK) ||
					  (list[i].authmode==WIFI_AUTH_WPA_WPA2_PSK)) &&
						 l<AP_SHOWN_IN_PAGE){

					 SaveAvailableAPs(list[i].ssid, l);
					 l++;
				 }
			}

            #ifdef __DEBUG_WIFI_LEV_2
			for (i=0; i<AP_SHOWN_IN_PAGE; i++)
			{
				PRINTF_DEBUG("WEB PAGE AP List[%d] %s\r\n",i, AAP[i]);
			}
			PRINTF_DEBUG("\n\n");
            #endif

			free(list);
		break;

		case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_WPS_ER_SUCCESS");
			/* esp_wifi_wps_start() only gets ssid & password, so call esp_wifi_connect() here. */
			ESP_ERROR_CHECK(esp_wifi_wps_disable());
			wifi_config_t wifi_config_temp;
			ESP_ERROR_CHECK(esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config_temp));

			PRINTF_DEBUG("WPS AP %s PWD %s\n", wifi_config_temp.ap.ssid, wifi_config_temp.ap.password);

			SetWpsParameters(wifi_config_temp);
			SetConfigReceived();
			P_COV_LN;
		break;

		default:
			ESP_LOGI(TAG, "event; %d\n",event->event_id);
			P_COV_LN;
		break;
    }
    return ESP_OK;
}


/**
 * @brief StartTimerForAPConnection
 *        catch the started time of AP connection
 *
 * @param  none
 * @return none
 *
 */
void StartTimerForAPConnection(void){
	PRINTF_DEBUG("TimerForAPConnection Started\n");
	TimerForAPConnection = RTC_Get_UTC_Current_Time();
}

/**
 * @brief IsTimerForAPConnectionExpired
 *        check if the AP connection are done in a
 *
 *
 * @param  none
 * @return none
 *
 */
void IsTimerForAPConnectionExpired(void){
	if(TimerForAPConnection != 0){
		if(RTC_Get_UTC_Current_Time() >= TimerForAPConnection + 120){
			PRINTF_DEBUG("TimerForAPConnection Expired \n");
			TimerForAPConnection = 0;
			connect_attempt = 0;
			ESP_ERROR_CHECK(esp_wifi_connect());
		}
	}
}

/**
 * @brief WiFi_GetConfigSM
 *       return state machine config status
 *
 * @param  none
 * @return C_RES
 */
config_sm_t WiFi_GetConfigSM(void){
    return config_sm;
}

/**
 * @brief WiFi_SetConfigSM
 *      set state machine config status
 *
 * @param  none
 * @return C_RES
 */
void WiFi_SetConfigSM(config_sm_t config_state){
    config_sm = config_state;
}

/**
 * @brief WiFi__Config
 *   Configure the WiFi interface:
 * @Param:
 * 		sm: state variable
 * @return:
 * 		gme_sm_t: SM__Start state
 *
 * @Description:
 * 	Situation 1:
 * 	Starts with checking the NVM if exists old configuration. If it finds old config, it will read them from the NVM
 * 	and configure the system directly, starts the wifi then run the whole system in base of the configuration found.
 *
 *	Situation 2:
 * 	If it finds nothing, it will start the http_server and the wifi with the default configuration (check gme_config.h),
 * 	then attends for the html configuration. After receiving them, it writes them in NVM and does a reboot. After the reboot,
 * 	the system will be found in situation 1.
 *
 * 	Note: 	1) The HTTP_Server is launched as Task, so it can be accessed after the configuration, during the system running, in case
 * 				we wanted to re-configure the wifi.
 * 			2) In case of the gateway is configured as AP Mode, the varies functionalities don't start and the system will attend the
 * 				new configuration.
 *
 *
 * */
gme_sm_t WiFi__Config (config_sm_t sm)
{
    config_sm = sm;
    while(1)
    {
        switch(config_sm)
        {
        case CHECK_FOR_CONFIG:
        	PRINTF_DEBUG("WiFi__Config .... CHECK_FOR_CONFIG\n");
            /*Check in NVM*/
            if(C_SUCCESS == NVM__ReadU8Value("wifi_conf", &wifi_conf) && CONFIGURED == wifi_conf){
            	WiFi__ReadCustomConfigFromNVM();
                config_sm = CONFIGURE_GME;
            }else{
                config_sm = SET_DEFAULT_CONFIG;
            }
            break;

        case SET_DEFAULT_CONFIG:
        	PRINTF_DEBUG("WiFi__Config .... SET_DEFAULT_CONFIG\n");

            if(WiFi__SetDefaultConfig()){
                config_sm = START_WIFI;
            }

            PRINTF_DEBUG("WiFi__Config .... SET_DEFAULT_CONFIG  END\n");

            break;

        case START_WIFI:
        	PRINTF_DEBUG("WiFi__Config .... START_WIFI\n");

            if (WiFi__StartWiFi()){
                ESP_ERROR_CHECK(HTTPServer__StartFileServer(&AP_http_server, "/spiffs"));

                if(CONFIGURED == wifi_conf /*&& APSTA_MODE == WiFi__GetCustomConfig().gateway_mode*/){
        			return GME_WAITING_FOR_INTERNET;
                }else{
                    config_sm = WAITING_FOR_HTML_CONF_PARAMETERS;
                }
            }
            break;

        case WAITING_FOR_HTML_CONF_PARAMETERS:
            if(test2 == 0){
            	//PRINTF_DEBUG("\nGateway Mode = %d, Wifi Conf has %d config\n\n",WiFi__GetCustomConfig().gateway_mode,wifi_conf);
            	PRINTF_DEBUG("\nWifi Conf has %d config\n\n",wifi_conf);
            	PRINTF_DEBUG("WiFi__Config .... WAITING_FOR_HTML_CONF_PARAMETERS\n");
            	test2=10;
            }
            if(IsConfigReceived()){
                PRINTF_DEBUG("Configuration Received\n");

            	WiFi__WriteCustomConfigInNVM(HTTPServer__GetCustomConfig());
            	//TODO CPPCHECK meglio se questa routine ritorna C_SUCCESS o C_FAIL e poi gestiamo la mancata
            	//scrittura qui sotto non facendo procedere la macchina a stati che dite ?

                if(C_SUCCESS == NVM__WriteU8Value("wifi_conf", (uint8_t)CONFIGURED)){
                	config_sm = CONFIGURE_GME;
                }
                if(wifi_conf == CONFIGURED){			//for future implementation
                	wifi_conf = TO_RECONFIGURE;
                }
            }
            if(IsWpsMode()){
            	ESP_LOGI(TAG, "start wps...");
            	static esp_wps_config_t config = WPS_CONFIG_INIT_DEFAULT(WPS_TYPE_PBC);
            	ESP_ERROR_CHECK(esp_wifi_wps_enable(&config));
            	ESP_ERROR_CHECK(esp_wifi_wps_start(0));
            	UnSetWpsMode();
            	P_COV_LN;
            }
            break;

        case CONFIGURE_GME:
        	PRINTF_DEBUG("WiFi__Config .... CONFIGURE_GME\n");

            if(CONFIGURED == wifi_conf){
                P_COV_LN;
            	WiFi__SetCustomConfig(WiFi__GetCustomConfig());
                config_sm = START_WIFI;
            }else{
                return GME_REBOOT;
            }
            break;


        default:
        	//P_COV_SINGLE("%s|%s|%d|%d\r\n",COV_MARK,__FILE__, __LINE__, config_sm);
            break;
        }

        //Check reboot/factory reset button
        if (Get_Button_Pin() >= 0)
            Sys__ResetCheck();

    }
}


/**
 * @brief SetAPConfig
 *        Set the AccessPoint configuration
 *
 * @param  onst char* ip
 * @param  const char* gw
 * @param  const char* netmask
 *
 * @return none
 */
static void SetAPConfig(const char* ip, const char* gw, const char* netmask){

	tcpip_adapter_ip_info_t ap_ip;
	ap_ip.ip.addr = ipaddr_addr(ip);
	ap_ip.gw.addr = ipaddr_addr(gw);
	ap_ip.netmask.addr = ipaddr_addr(netmask);
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP,&ap_ip));
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
}

/**
 * @brief SetSTAStaticIP
 *        Set the Station static IP
 *
 * @param  const char* ip
 * @param  const char* gw
 * @param  const char* netmask
 * @param  const char* pri_dns
 * #param  const char* sec_dns
 *
 * @return none
 */
static void SetSTAStaticIP(const char* ip, const char* gw, const char* netmask, const char* pri_dns, const char* sec_dns)
{
	tcpip_adapter_ip_info_t sta_ip;
	sta_ip.ip.addr = ipaddr_addr(ip);
	tcpip_adapter_dns_info_t sta_primary_dns;
	tcpip_adapter_dns_info_t sta_secondary_dns;

	sta_ip.gw.addr = ipaddr_addr(gw);
	sta_ip.netmask.addr = ipaddr_addr(netmask);
	ESP_ERROR_CHECK(tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA));
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA,&sta_ip));


	sta_primary_dns.ip.u_addr.ip4.addr = ipaddr_addr(pri_dns);
	sta_secondary_dns.ip.u_addr.ip4.addr = ipaddr_addr(sec_dns);
	ESP_ERROR_CHECK(tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &sta_primary_dns));
	ESP_ERROR_CHECK(tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_BACKUP, &sta_secondary_dns));
}

/**
 * @brief WiFi__SetDefaultConfig
 *        if no custom config are already done
 *        the wifi start with a default configuration.
 *
 * @param  none
 *
 * @return int
 */
int WiFi__SetDefaultConfig(void)
{
	char* ap_ssid_def;
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();

	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    SetAPConfig(AP_DEF_IP, AP_DEF_GW, AP_DEF_NETMASK);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();


    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    Utilities__Init();

    ap_ssid_def = HTTPServer__SetAPDefSSID(AP_DEF_SSID);

    wifi_config_t wifi_config_AP = {
   		.ap = {
   			.password = "",
			.ssid_hidden = 0,
   			.max_connection = AP_DEF_MAX_CONN,
   			.authmode = WIFI_AUTH_OPEN
   		},
       };


    strcpy((char*)wifi_config_AP.ap.ssid, ap_ssid_def);
    wifi_config_AP.ap.ssid_len = strlen(ap_ssid_def);
#ifdef GW_GSM_WIFI
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));	// enable AP mode only in case of a 2G HW model with both 2G and WiFi AP active
#else
    	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
#endif
    	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_AP));

    PRINTF_DEBUG("wifi_init_AP:\nSSID: %s\nPswd: Open Network\n" , ap_ssid_def);

    return 1;
}


/**
 * @brief WiFi__StartWiFi
 *        start the wifi module.
 *        call a "esp_wifi_start()" function
 *        for more details see esp-idf manual.
 *
 * @param  none
 *
 * @return int
 */
int WiFi__StartWiFi(void){
    ESP_ERROR_CHECK(esp_wifi_start());
    return 1;
}

/**
 * @brief WiFi__ReadCustomConfigFromNVM
 *        Read from the NVM all the info
 *        useful for the wifi connection
 *
 * @param  none
 * @return none
 */
void WiFi__ReadCustomConfigFromNVM(void){
    
	size_t len = 0;
    NVM__ReadString(HTMLCONF_AP_SSID, WiFiConfig.ap_ssid, &len);
    NVM__ReadU8Value(HTMLCONF_AP_SSID_HIDDEN, &WiFiConfig.ap_ssid_hidden);
    NVM__ReadString(HTMLCONF_AP_PSWD, WiFiConfig.ap_pswd, &len);
    NVM__ReadString(HTMLCONF_AP_IP, WiFiConfig.ap_ip, &len);

    NVM__ReadString(HTMLCONF_STA_SSID, WiFiConfig.sta_ssid, &len);
    NVM__ReadString(HTMLCONF_STA_ENCRYP, WiFiConfig.sta_encryption, &len);
    NVM__ReadString(HTMLCONF_STA_PSWD, WiFiConfig.sta_pswd, &len);
    NVM__ReadU8Value(HTMLCONF_STA_DHCP_MODE, &WiFiConfig.sta_dhcp_mode);
    NVM__ReadString(HTMLCONF_STA_STATIC_IP, WiFiConfig.sta_static_ip, &len);
    NVM__ReadString(HTMLCONF_STA_NETMASK, WiFiConfig.sta_netmask, &len);
    NVM__ReadString(HTMLCONF_STA_GATEWAY_IP, WiFiConfig.sta_gateway_ip, &len);
    NVM__ReadString(HTMLCONF_STA_PRI_DNS, WiFiConfig.sta_primary_dns, &len);
    NVM__ReadString(HTMLCONF_STA_SCND_DNS, WiFiConfig.sta_secondary_dns, &len);


    // read from file system
    GetNtpServer(WiFiConfig.ntp_server_addr);
    // NB. the esp library use always the default port 123...so the file system contain the value but is not used!!!

	// read from file system
    GetMqttBroker(WiFiConfig.mqtt_server_addr);
    GetMqttPort(WiFiConfig.mqtt_server_port);

	PRINTF_DEBUG("ap_ssid: %s\n",WiFiConfig.ap_ssid);
    PRINTF_DEBUG("ap_ssid_hidden: %d\n",WiFiConfig.ap_ssid_hidden);
    PRINTF_DEBUG("ap_pswd: %s\n",WiFiConfig.ap_pswd);
    PRINTF_DEBUG("ap_ip: %s\n",WiFiConfig.ap_ip);
    PRINTF_DEBUG("sta_ssid: %s\n",WiFiConfig.sta_ssid);
    PRINTF_DEBUG("sta_encryption: %s\n",WiFiConfig.sta_encryption);
    PRINTF_DEBUG("sta_pswd: %s\n",WiFiConfig.sta_pswd);
    PRINTF_DEBUG("sta_dhcp_mode: %d\n",WiFiConfig.sta_dhcp_mode);
    PRINTF_DEBUG("sta_static_ip: %s\n",WiFiConfig.sta_static_ip);
    PRINTF_DEBUG("sta_netmask: %s\n",WiFiConfig.sta_netmask);
    PRINTF_DEBUG("sta_gateway_ip: %s\n",WiFiConfig.sta_gateway_ip);
    PRINTF_DEBUG("sta_primary_dns: %s\n",WiFiConfig.sta_primary_dns);
    PRINTF_DEBUG("sta_secondary_dns: %s\n",WiFiConfig.sta_secondary_dns);

    PRINTF_DEBUG("\nntp_server_addr: %s\n",WiFiConfig.ntp_server_addr);
}


/**
 * @brief WiFi__ReadCustomConfigFromNVM
 *        Read from the NVM all the info
 *        useful for the wifi connection
 *
 * @param  none
 * @return none
 */
void WiFi__WriteCustomConfigInNVM(html_config_param_t config){
	//TODO CPPCHECK valori di ritorno non testati che succede se sbagliamo a scrivere la es. la pwd
	//ne scriviamo una sporca e poi ciccia non si ricollegano più ?

    NVM__WriteString(HTMLCONF_AP_SSID, config.ap_ssid);
    NVM__WriteU8Value(HTMLCONF_AP_SSID_HIDDEN, config.ap_ssid_hidden);
    NVM__WriteString(HTMLCONF_AP_PSWD, config.ap_pswd);
    NVM__WriteString(HTMLCONF_AP_IP, config.ap_ip);

    NVM__WriteString(HTMLCONF_STA_SSID, config.sta_ssid);
    NVM__WriteString(HTMLCONF_STA_ENCRYP, config.sta_encryption);
    NVM__WriteString(HTMLCONF_STA_PSWD, config.sta_pswd);

    NVM__WriteU8Value(HTMLCONF_STA_DHCP_MODE, config.sta_dhcp_mode);
    NVM__WriteString(HTMLCONF_STA_STATIC_IP, config.sta_static_ip);
    NVM__WriteString(HTMLCONF_STA_NETMASK, config.sta_netmask);
    NVM__WriteString(HTMLCONF_STA_GATEWAY_IP, config.sta_gateway_ip);
    NVM__WriteString(HTMLCONF_STA_PRI_DNS, config.sta_primary_dns);
    NVM__WriteString(HTMLCONF_STA_SCND_DNS, config.sta_secondary_dns);

}

/**
 * @brief WiFi__GetCustomConfig
 *        Return the info
 *        of the wifi configuration
 *
 * @param  none
 * @return html_config_param_t
 */
html_config_param_t WiFi__GetCustomConfig (void){
    return WiFiConfig;
}

/**
 * @brief WiFi__GetCustomConfigPTR
 *
 *
 * @param  none
 * @return html_config_param_t*
 */
html_config_param_t* WiFi__GetCustomConfigPTR (void){
    return &WiFiConfig;
}

/**
 * @brief WiFi__SetCustomConfig
 *
 *
 * @param  html_config_param_t config
 * @return esp_err_t
 */
esp_err_t WiFi__SetCustomConfig(html_config_param_t config){
    
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();


    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    Utilities__Init();	// take mac from fuses for ap name setting

    SetAPConfig(config.ap_ip, config.ap_ip, AP_DEF_NETMASK);

    wifi_config_t wifi_config_AP = {
   		.ap = {
   			.ssid_len = strlen(config.ap_ssid),
			.ssid_hidden = config.ap_ssid_hidden,
   			.max_connection = AP_DEF_MAX_CONN,
   			.authmode = WIFI_AUTH_WPA_WPA2_PSK
   			},
       };
	strcpy((char*)wifi_config_AP.ap.ssid,config.ap_ssid);
	strcpy((char*)wifi_config_AP.ap.password,config.ap_pswd);
    wifi_config_AP.ap.ssid_len = strlen(config.ap_ssid);


	u32_t end_ip = 0;
	u32_t temp = 0;
	//Set dhcp server base ip
	tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
	dhcps_lease_t optValue;
	optValue.enable = true;

	optValue.start_ip.addr = inet_addr(AP_DEF_IP);

	end_ip = optValue.start_ip.addr & 0x00FFFFFF;
	temp = optValue.start_ip.addr >> 24;
	temp += AP_DHCP_IP_RANGE;
	if(temp > 0xFA){
		temp = 0xFA000000;
	}else{
		temp = temp << 24;
	}
	end_ip |= temp;
	optValue.end_ip.addr = end_ip; // Start IP Address
	tcpip_adapter_dhcps_option(
		TCPIP_ADAPTER_OP_SET,
		TCPIP_ADAPTER_REQUESTED_IP_ADDRESS,
		&optValue, sizeof(optValue));
	tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);

	wifi_config_t wifi_config_STA = {
		.sta = {
			.ssid = "STAPROBLEM",
			.password = "staproblem",
			},
	   };

	strcpy((char*)wifi_config_STA.sta.ssid,config.sta_ssid);
	strcpy((char*)wifi_config_STA.sta.password,config.sta_pswd);
	//wifi_config_AP.sta.ssid_len = strlen(config.ap_ssid);

	PRINTF_DEBUG("\nSTA SSID = %s  and  Password = %s\n",wifi_config_STA.sta.ssid,wifi_config_STA.sta.password);

	if(!config.sta_dhcp_mode)
	{
		PRINTF_DEBUG("DHCP STA OFF\n");
		SetSTAStaticIP(config.sta_static_ip,
						config.sta_gateway_ip,
						config.sta_netmask,					
						config.sta_primary_dns,
						config.sta_secondary_dns);
		P_COV_LN;
	}
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_STA));

	PRINTF_DEBUG("wifi_init_AP\n  SSID: %s\n  Pass: %s\n" , wifi_config_AP.ap.ssid, wifi_config_AP.ap.password);

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_AP));

    return ESP_OK;
}

/**
 * @brief WiFi__WaitConnection
 *
 *
 * @param  none
 * @return none
 */
void WiFi__WaitConnection(void)
{
    xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT, false, true, 30000/portTICK_RATE_MS); // wait for a limited time,
    																							 // state machine will come back here over and over
}

/**
 * @brief WIFI__SetSTAStatus
 *		  set the status of the wifi
 *
 * @param  connection_status_t status
 * @return none
 */
void WIFI__SetSTAStatus(connection_status_t status){
	STAStatus = status;
}

/**
 * @brief WIFI__GetSTAStatus
 *	      return the wifi status
 *
 * @param  none
 * @return connection_status_t
 */
connection_status_t WIFI__GetSTAStatus(void){
	return STAStatus;
}

/**
 * @brief WiFi__GetMac
 *	      return the wifi macaddr and save it
 *	      to the passed parameter
 *
 * @param  uint8_t* wifi_mac_address_gw
 * @return C_RES
 */
C_RES WiFi__GetMac(uint8_t* wifi_mac_address_gw){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
		err = esp_wifi_get_mac(ESP_IF_WIFI_STA, wifi_mac_address_gw);
#endif
	return err;
}

/**
 * @brief WiFi__GetRSSI
 *	      return the wifi RSSI signal
 *
 * @param  none
 * @return int8_t
 */
int8_t WiFi__GetRSSI(void){
	int8_t rssi = 0;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	wifi_ap_record_t wifidata;
	esp_wifi_sta_get_ap_info(&wifidata);
	rssi = wifidata.rssi;
#endif
	return rssi;
}
