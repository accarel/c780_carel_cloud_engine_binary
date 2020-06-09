#include "common.h"
#include "mobile.h"
#include "https_client_IS.h"
#include "esp_modem.h"
#include "sim800.h"
#include "wifi.h"
#include "utilities_CAREL.h"
#include "IO_Port_IS.h"
#include "radio.h"

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

gme_sm_t Mobile__Config(void){

	tcpip_adapter_init();
    mobile_event_group = xEventGroupCreate();

    /* create dte object */
    esp_modem_dte_config_t config = ESP_MODEM_DTE_DEFAULT_CONFIG(CONFIG_UART_MODEM_PORT);
    esp_uart_t uart_pins = {Get_Modem_TX(), Get_Modem_RX(), Get_Modem_RTS(), Get_Modem_CTS()};
    modem_dte_t *dte = esp_modem_dte_init(&config, uart_pins);

    /* Register event handler */
    ESP_ERROR_CHECK(esp_modem_add_event_handler(dte, modem_event_handler, NULL));
    /* create dce object */
#if CONFIG_MODEM_DEVICE_SIM800
    modem_dce_t *dce = sim800_init(dte);
#elif CONFIG_MODEM_DEVICE_BG96
    modem_dce_t *dce = bg96_init(dte);
#endif
    ESP_ERROR_CHECK(dce->set_flow_ctrl(dce, MODEM_FLOW_CONTROL_NONE));
    ESP_ERROR_CHECK(dce->store_profile(dce));

    /* Print Module ID, Operator, IMEI, IMSI */
    ESP_LOGI(TAG, "Module: %s", dce->name);
    ESP_LOGI(TAG, "Operator: %s", dce->oper);
    ESP_LOGI(TAG, "IMEI: %s", dce->imei);
    ESP_LOGI(TAG, "IMSI: %s", dce->imsi);

    Mobile__SaveImeiCode(dce->imei);
    Mobile__SaveImsiCode(dce->imsi);

    ESP_ERROR_CHECK(dce->get_qeng(dce));

    ESP_LOGI(TAG, "Module: %s", dce->mcc);
    ESP_LOGI(TAG, "Operator: %s", dce->mnc);
    ESP_LOGI(TAG, "IMEI: %s", dce->lac);
    ESP_LOGI(TAG, "IMSI: %s", dce->cellid);

    Mobile__SaveMccCode(dce->mcc);
    Mobile__SaveMncCode(dce->mnc);
    Mobile__SaveLacCode(dce->lac);
    Mobile__SaveCidCode(dce->cellid);

    /* Get signal quality */
    uint32_t rssi = 0, ber = 0;
    ESP_ERROR_CHECK(dce->get_signal_quality(dce, &rssi, &ber));
    ESP_LOGI(TAG, "rssi: %d, ber: %d", rssi, ber);
    /* Get battery voltage */
    uint32_t voltage = 0, bcs = 0, bcl = 0;
    ESP_ERROR_CHECK(dce->get_battery_status(dce, &bcs, &bcl, &voltage));
    ESP_LOGI(TAG, "Battery voltage: %d mV", voltage);

    Utilities__Init();

    /* Setup PPP environment */
    char tmp_apn_name[64];
    esp_modem_setup_ppp(dte, GetApnName(tmp_apn_name));

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
