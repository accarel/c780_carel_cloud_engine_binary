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

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_log.h"


#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_partition.h"

#include "wifi.h"

//#include "file_system.h"

#include "polling_CAREL.h"
#include "polling_IS.h"
 
#include "sys_CAREL.h"
#endif





static const char *TAG = "OTA_IS";

#ifdef INCLUDE_PLATFORM_DEPENDENT
const esp_partition_t *run_part = NULL;

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
	C_RES resval = C_FAIL;
	
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	uart_read_bytes_IS((uart_port_t)uart_num, buf, (uint32_t)length, (TickType_t)ticks_to_wait);
	#endif
	
	return resval;
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
	C_INT16 retval = C_FAIL;

	#ifdef INCLUDE_PLATFORM_DEPENDENT
	if (uart_write_bytes( (uart_port_t)uart_num, (const char*)src, (size_t)size) == ESP_OK) retval = C_SUCCESS;
	#endif

	return retval;
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





