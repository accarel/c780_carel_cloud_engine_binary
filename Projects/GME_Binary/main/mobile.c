#include "common.h"
#include "mobile.h"
#include "https_client_IS.h"
#include "esp_modem.h"
#include "sim800.h"
#include "wifi.h"
#include "utilities_CAREL.h"
#include "IO_Port_IS.h"
#include "radio.h"
#include "sys_IS.h"

static EventGroupHandle_t mobile_event_group = NULL;
static const int CONNECT_BIT = BIT0;
static const int STOP_BIT = BIT1;

static const char *TAG = "mobile";
static connection_status_t MobStatus = DISCONNECTED;
static char gsm_imei_gw_str[MODEM_IMEI_LENGTH] = {0};
static char gsm_imsi_gw_str[MODEM_IMSI_LENGTH] = {0};
static char gsm_mcc_gw_str[MCC_LENGTH] = {0};
static char gsm_mnc_gw_str[MNC_LENGTH] = {0};
static char gsm_lac_gw_str[LAC_LENGTH] = {0};
static char gsm_cellid_gw_str[CELLID_LENGTH] = {0};

static C_TIME MobConnTime = 0;
static modem_dte_t *dte;
static modem_dce_t *dce;

static int16_t rssibuf[GW_SAMPLES_MOBILE] = {0};
static uint8_t command_mode = 0;

static void modem_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id) {
    case MODEM_EVENT_PPP_START:
        ESP_LOGI(TAG, "Modem PPP Started");
        break;
    case MODEM_EVENT_PPP_CONNECT:
        ESP_LOGI(TAG, "Modem Connect to PPP Server");
        ppp_client_ip_info_t *ipinfo = (ppp_client_ip_info_t *)(event_data);
        ESP_LOGI(TAG, "~~~~~~~~~~~~~~");
        ESP_LOGI(TAG, "IP          : " IPSTR, IP2STR(&ipinfo->ip));
        ESP_LOGI(TAG, "Netmask     : " IPSTR, IP2STR(&ipinfo->netmask));
        ESP_LOGI(TAG, "Gateway     : " IPSTR, IP2STR(&ipinfo->gw));
        ESP_LOGI(TAG, "Name Server1: " IPSTR, IP2STR(&ipinfo->ns1));
        ESP_LOGI(TAG, "Name Server2: " IPSTR, IP2STR(&ipinfo->ns2));
        ESP_LOGI(TAG, "~~~~~~~~~~~~~~");
        xEventGroupSetBits(mobile_event_group, CONNECT_BIT);
        Mobile__SetStatus(CONNECTED);
        break;
    case MODEM_EVENT_PPP_DISCONNECT:
        ESP_LOGI(TAG, "Modem Disconnect from PPP Server");
        Mobile__SetStatus(DISCONNECTED);
        break;
    case MODEM_EVENT_PPP_STOP:
        ESP_LOGI(TAG, "Modem PPP Stopped");
        xEventGroupSetBits(mobile_event_group, STOP_BIT);
        break;
    case MODEM_EVENT_UNKNOWN:
        ESP_LOGW(TAG, "Unknow line received: %s", (char *)event_data);
        break;
    default:
        break;
    }
}

gme_sm_t Mobile__Init(void){
	tcpip_adapter_init();
    mobile_event_group = xEventGroupCreate();

    /* create dte object */
    esp_modem_dte_config_t config = ESP_MODEM_DTE_DEFAULT_CONFIG(CONFIG_UART_MODEM_PORT);
    esp_uart_t uart_pins = {Get_Modem_TX(), Get_Modem_RX(), Get_Modem_RTS(), Get_Modem_CTS()};
    dte = esp_modem_dte_init(&config, uart_pins);
    if (dte == NULL) {
        // manage the case dte cannot be created
        return GME_REBOOT;
    }
    /* Register event handler */
    ESP_ERROR_CHECK(esp_modem_add_event_handler(dte, modem_event_handler, NULL));
    /* create dce object */
#if CONFIG_MODEM_DEVICE_SIM800
    dce = sim800_init(dte);
#elif CONFIG_MODEM_DEVICE_BG96
    dce = bg96_init(dte);
#endif
    if (dce == NULL) {
        // manage the case dce cannot be created
        return GME_REBOOT;
    }

    ESP_ERROR_CHECK(dce->set_flow_ctrl(dce, MODEM_FLOW_CONTROL_NONE));
    ESP_ERROR_CHECK(dce->store_profile(dce));

    /* Print Module ID, Operator, IMEI, IMSI */
    ESP_LOGI(TAG, "Module: %s", dce->name);
    ESP_LOGI(TAG, "Operator: %s", dce->oper);
    ESP_LOGI(TAG, "IMEI: %s", dce->imei);
    ESP_LOGI(TAG, "IMSI: %s", dce->imsi);

    Mobile__SaveImeiCode(dce->imei);
    Mobile__SaveImsiCode(dce->imsi);

    return GME_RADIO_CONFIG;
}

C_RES Mobile__WaitNetwork(void){
	/* Get network status */
	uint32_t status = 0;
	ESP_ERROR_CHECK(dce->get_network_status(dce, &status));
	ESP_LOGI(TAG, "status: %d", status);
	if(status != 0)
		return C_FAIL;
	else
		return C_SUCCESS;
}

gme_sm_t Mobile__Config(void){

    // at times first get_qeng does not work,
    // so repeat it some times
    // if it does not receive answer, something is wrong
    int retry = 0;
    C_RES err;
    do{
    	Sys__Delay(1000);
    	err = dce->get_qeng(dce);
    	retry++;
    	printf("err %d, retry %d\n", err, retry);
    }while(strcmp(dce->mcc, "x") == 0 && retry < 5);

    ESP_LOGI(TAG, "MCC: %s", dce->mcc);
    ESP_LOGI(TAG, "MNC: %s", dce->mnc);
    ESP_LOGI(TAG, "LAC: %s", dce->lac);
    ESP_LOGI(TAG, "CELLID: %s", dce->cellid);

    Mobile__SaveMccCode(dce->mcc);
    Mobile__SaveMncCode(dce->mnc);
    Mobile__SaveLacCode(dce->lac);
    Mobile__SaveCidCode(dce->cellid);

    /* Get signal quality */
    uint32_t rssi = 0, ber = 0;
    ESP_ERROR_CHECK(dce->get_signal_quality(dce, &rssi, &ber));
    ESP_LOGI(TAG, "rssi: %d, ber: %d", rssi, ber);
    Mobile_SetSignalQuality(rssi);

    Utilities__Init();

    /* Setup PPP environment */
    char tmp_apn_name[APN_NAME_SIZE];
    char tmp_username[APN_USER_SIZE];
    char tmp_password[APN_PASS_SIZE];
    esp_modem_setup_ppp(dte, GetApnName(tmp_apn_name), GetApnUserName(tmp_username), GetApnPassword(tmp_password));

    return GME_WAITING_FOR_INTERNET;

}

void Mobile__SaveImeiCode(char* buf){
	strcpy(gsm_imei_gw_str, buf);
}

void Mobile__SaveImsiCode(char* buf){
	strcpy(gsm_imsi_gw_str, buf);
}

void Mobile__SaveMccCode(char* buf){
	strcpy(gsm_mcc_gw_str, buf);
}

void Mobile__SaveMncCode(char* buf){
	strcpy(gsm_mnc_gw_str, buf);
}

void Mobile__SaveLacCode(char* buf){
	strcpy(gsm_lac_gw_str, buf);
}

void Mobile__SaveCidCode(char* buf){
	strcpy(gsm_cellid_gw_str, buf);
}

char* Mobile__GetImeiCode(void){
	return gsm_imei_gw_str;
}

char* Mobile__GetImsiCode(void){
	return gsm_imsi_gw_str;
}

char* Mobile__GetMccCode(void){
	return gsm_mcc_gw_str;
}

char* Mobile__GetMncCode(void){
	return gsm_mnc_gw_str;
}

char* Mobile__GetLacCode(void){
	return gsm_lac_gw_str;
}

char* Mobile__GetCidCode(void){
	return gsm_cellid_gw_str;
}

void Mobile__WaitConnection(void)
{
	/* Wait for IP address */
	xEventGroupWaitBits(mobile_event_group, CONNECT_BIT, false, true, portMAX_DELAY);
}


void Mobile__SetStatus(connection_status_t status){
	MobStatus = status;
}

connection_status_t Mobile__GetStatus(void){
	return MobStatus;
}

void Mobile__SetConnTime(void){
	if (Mobile__GetStatus() == CONNECTED)
			MobConnTime = RTC_Get_UTC_Current_Time();
		else
			MobConnTime = 0;
}

C_TIME Mobile__GetConnTime(void){
	if (MobConnTime == 0)
		return -1;
	else
		return (RTC_Get_UTC_Current_Time() - MobConnTime);
}

void Mobile_QuerySignalQuality(void){
	uint32_t rssi = 0, ber = 0;
	C_RES err = C_FAIL;

	printf("Set COMMAND mode at %d\n", RTC_Get_UTC_Current_Time());
	Mobile_SetCommandMode(1);
	err = dte->change_mode(dte,MODEM_COMMAND_MODE);
	printf ("COMMAND mode result: %d\n", err);

	printf("did we enter command mode?\n");
	err = dce->sync(dce);
	if(err == C_FAIL) {
		printf("no, really\n");
		Mobile_SetCommandMode(0);
		return;
	}
	else if(err == 2){ // NO CARRIER in response to AT ---> not currently captured... TODO
		err = dte->change_mode(dte,MODEM_PPP_MODE); // maybe better to reboot...
		printf("try to reconnect %d\n", err);
		Mobile_SetCommandMode(0);
		return;
	}
	else
		printf("it seems we are in command mode\n");

	printf("Get signal quality\n");
	err = dce->get_signal_quality(dce, &rssi, &ber);
	printf("Get signal quality result %d\n", err);
	printf("Set PPP mode\n");
	err = dte->change_mode(dte,MODEM_PPP_MODE_AGAIN);
	Mobile_SetCommandMode(0);
	printf("PPP mode result: %d\n", err);
	Mobile_SetSignalQuality(rssi);

	printf("Read %d rssi, %d ber\n", rssi, ber);

	return;
}

void Mobile_SetSignalQuality(uint16_t rssi){
	static uint16_t count=0;
	printf("Mobile_SetSignalQuality element %d has rssi %d\n", count, rssi);
	rssibuf[count++] = rssi;
	count %= 4;
}

int16_t Mobile_GetSignalQuality(void){
	int16_t rssi_mean = 0;
	int l=0;
	printf("Mobile_GetSignalQuality\n");
	for (int i=0; i < GW_SAMPLES_MOBILE; i++) {
		rssi_mean += rssibuf[i];
		if(rssibuf[i] != 0)
			l++;
		printf("rssibuf[i]:%d\n", rssibuf[i]);
	}
	rssi_mean = (rssi_mean / l);
	rssi_mean = (-113 + 2*rssi_mean);
	return rssi_mean;
}

uint8_t Mobile_GetCommandMode(void){
	return command_mode;
}
void Mobile_SetCommandMode(uint8_t mode){
	command_mode = mode;
}
