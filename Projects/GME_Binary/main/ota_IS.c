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

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_https_ota.h"
#endif

#ifdef INCLUDE_PLATFORM_DEPENDENT
static const char *TAG = "OTA_IS";
const esp_partition_t *run_part = NULL;
EventGroupHandle_t s_ota_gme_group;
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
 * @brief UART read bytes from UART buffer
 *
 * @param uart_num UART_NUM_0, UART_NUM_1 or UART_NUM_2
 * @param buf     pointer to the buffer.
 * @param length  data length
 * @param ticks_to_wait sTimeout, count in RTOS ticks
 *
 * @return
 *     - C_FAIL
 *     - OTHERS (>=0) The number of bytes read from UART FIFO
 */
C_INT16 uart_read_bytes_IS(C_BYTE uart_num, C_BYTE *buf, C_UINT32 length, C_UINT32 ticks_to_wait)
{
	C_INT16 len = -1;
	
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	len = uart_read_bytes((uart_port_t)uart_num, buf, (uint32_t)length, (TickType_t)ticks_to_wait);
	#endif
	
	return len;
}



/**
 * @brief Send data to the UART port from a given buffer and length,
 *
 * If the UART driver's parameter 'tx_buffer_size' is set to zero:
 * This function will not return until all the data have been sent out, or at least pushed into TX FIFO.
 *
 * Otherwise, if the 'tx_buffer_size' > 0, this function will return after copying all the data to tx ring buffer,
 * UART ISR will then move data from the ring buffer to TX FIFO gradually.
 *
 * @param uart_num UART_NUM_0, UART_NUM_1 or UART_NUM_2
 * @param src   data buffer address
 * @param size  data length to send
 *
 * @return
 *     - C_FAIL Parameter error
 *     - OTHERS (>=0) The number of sent bytes 
 */
C_INT16 uart_write_bytes_IS(C_BYTE uart_num, const C_BYTE* src, C_INT16 size)
{
	C_INT16 len = -1;

	#ifdef INCLUDE_PLATFORM_DEPENDENT
	len = uart_write_bytes( (uart_port_t)uart_num, (const char*)src, (size_t)size);
	#endif

	return len;
}



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

void OTA_GMEEnd(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	vEventGroupDelete(s_ota_gme_group);
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


void OTAGroup (bool ota_res){
#ifdef INCLUDE_PLATFORM_DEPENDENT
	if(true == ota_res){
        xEventGroupSetBits(s_ota_gme_group, OTA_GME_OK);
	}else{
		xEventGroupSetBits(s_ota_gme_group, OTA_GME_FAIL);
	}
	C_INT16 res = ((OTA_GMEWaitCompletion() == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD);
	OTA_GMEEnd();

	CBOR_SendAsyncResponse(res);

#endif
}
