/**
 * @file   ota_IS.c
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the update of the FW of the attached device 
 *         via HTTPS
 */
 
/* Includes ------------------------------------------------------------------------ */

#include "CAREL_GLOBAL_DEF.h"
#include "ota_IS.h"
#include "https_client_IS.h"
#include "ota_CAREL.h"
#include "sys_CAREL.h"
#include "nvm_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_https_ota.h"
#endif

#ifdef INCLUDE_PLATFORM_DEPENDENT
static const char *TAG = "OTA_IS";
const esp_partition_t *run_part = NULL;
EventGroupHandle_t s_ota_gme_group;
EventGroupHandle_t s_ota_dev_group;

const int OTA_GME_OK = BIT0;
const int OTA_GME_FAIL = BIT1;

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

#endif

/**
 * @brief Alias of uart_flush_input.
 *        UART ring buffer flush. This will discard all data in the UART RX buffer.
 * @note  Instead of waiting the data sent out, this function will clear UART rx buffer.
 *        In order to send all the data in tx FIFO, we can use uart_wait_tx_done function.
 * @param uart_num UART_NUM_0, UART_NUM_1 or UART_NUM_2
 *
 * @return
 *     - C_SUCCES Success
 *     - C_FAIL   Parameter error
 */
C_RES uart_flush_IS(C_BYTE uart_num)
{  
  C_RES retval = C_FAIL;
	
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  if (uart_flush( (uart_port_t)uart_num) == ESP_OK) retval = C_SUCCESS;
  #endif		
  
  return retval; 	
}



/**
 * @brief Clear input buffer, discard all the data is in the ring-buffer.
 * @note  In order to send all the data in tx FIFO, we can use uart_wait_tx_done function.
 * @param uart_num UART_NUM_0, UART_NUM_1 or UART_NUM_2
 *
 * @return
 *     - C_SUCCES Success
 *     - C_FAIL   Parameter error
 */
C_RES uart_flush_input_IS(C_BYTE uart_num)
{
  C_RES retval = C_FAIL;
	
  #ifdef INCLUDE_PLATFORM_DEPENDENT
  if (uart_flush_input((uart_port_t)uart_num) == ESP_OK) retval = C_SUCCESS;
  #endif  
  
  return retval; 	
}

C_RES https_ota(c_http_client_config_t* c_config)
{
	C_RES ret = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	esp_http_client_config_t config = {
		.url = c_config->url,
		.username = c_config->username,
		.password = c_config->password,
		.cert_pem = Sys__GetCert(c_config->cert_num),
		.event_handler = _http_event_handler,
		.auth_type = HTTP_AUTH_TYPE_BASIC,
	};

	printf("url : %s\n",config.url);

	//This function manage the ota operation, the checksum validation and the ota partition switch
	ret = esp_https_ota(&config);
#endif
	return ret;
}


C_RES OTA_GMEWaitCompletion(void)
{
	C_RES ret = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	EventBits_t bits = xEventGroupWaitBits(s_ota_gme_group, OTA_GME_OK | OTA_GME_FAIL, true, false, portMAX_DELAY);
	if ((bits & OTA_GME_OK) != 0)
		return C_SUCCESS;
#endif
	return ret;
}

void OTA_End(EventGroupHandle_t s_ota_group)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	vEventGroupDelete(s_ota_group);
#endif
}

/**
 * @brief OTA__GMEInit Create a task for ota GME upgrade
 * @param c_cborrequpdgmefw
 *
 * @return
 *     void
 */
void OTA__GMEInit(c_cborrequpdgmefw update_gw_fw)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	s_ota_gme_group = xEventGroupCreate();
    xTaskCreate(&GME_ota_task, "GME_ota_task", 8192, (void*)&update_gw_fw, 5, NULL);
#endif
}

void OTA__DEVInit(c_cborrequpddevfw update_dev_fw)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	s_ota_dev_group = xEventGroupCreate();
	xTaskCreate(&DEV_ota_task, "DEV_ota_task", 8192, (void*)&update_dev_fw, 5, NULL);
#endif
}

void OTA__ModelInit(c_cborreqdwldevsconfig download_devs_config)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	xTaskCreate(&Model_ota_task, "Model_ota_task", 8192, (void*)&download_devs_config, 5, NULL);
#endif
}

void OTA__CAInit(c_cborrequpdatecacert update_ca)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	xTaskCreate(&CA_ota_task, "CA_ota_task", 8192, (void*)&update_ca, 9, NULL);
#endif
}

void OTADEVGroup (bool ota_res){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if(true == ota_res){
		xEventGroupSetBits(s_ota_dev_group, OTA_GME_OK);
	}else{
		xEventGroupSetBits(s_ota_dev_group, OTA_GME_FAIL);
	}
	OTA_End(s_ota_dev_group);

	ota_res == TRUE ? CBOR_SendAsyncResponse(0) : CBOR_SendAsyncResponse(1);

	// restart polling if needed
	uint8_t pe_status;
	NVM__ReadU8Value(PE_STATUS_NVM, &pe_status);
	if (pe_status == RUNNING)
		PollEngine_StartEngine_CAREL();	// restart polling
#endif
}

void OTAGroup (bool ota_res){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if(true == ota_res){
        xEventGroupSetBits(s_ota_gme_group, OTA_GME_OK);
	}else{
		xEventGroupSetBits(s_ota_gme_group, OTA_GME_FAIL);
	}
	C_INT16 res = ((OTA_GMEWaitCompletion() == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD);
	OTA_End(s_ota_gme_group);

	CBOR_SendAsyncResponse(res);
	// restart polling if needed
	uint8_t pe_status;
	NVM__ReadU8Value(PE_STATUS_NVM, &pe_status);
	if (pe_status == RUNNING)
		PollEngine_StartEngine_CAREL();	// restart polling
#endif
}
