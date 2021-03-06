/**
 * @file   https_client_IS.c
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the download of some files via HTTPS
 */
 
 
/* Includes ------------------------------------------------------------------------ */
#include "https_client_IS.h"
#include "sys_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_http_client.h"
#endif

static const char *TAG = "HTTP_CLIENT_IS";


http_client_handle_t client_http_client_init_IS = NULL;
esp_http_client_config_t esp_config_http_client_init_IS;


#ifdef INCLUDE_PLATFORM_DEPENDENT

/**
 * @brief _http_event_handler
all the functions in this files are using the same event handler
 *
 * @param   download_devs_config : the connection data uri/password/username
 *
 * @param   esp_http_client_event_t *evt
 *          the event
 *
 * @return  ESP_OK/ESP_FAIL
 *
 */
static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
			P_COV_LN;
            break;
			
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
			P_COV_LN;
            break;
			
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
			P_COV_LN;
            break;
			
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
			P_COV_LN;
            break;
			
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);

            if (!esp_http_client_is_chunked_response(evt->client)) {

                #ifdef __CCL_DEBUG_MODE
            	printf("--------------------\n");
            	printf("%d\n", evt->data_len);

                #ifdef __CCL_DEBUG_MODE_1
            	char *p;

            	p = (char *)(evt->data);

            	for (int ciclo=0; ciclo < evt->data_len; ciclo++)
            	{
            		printf("%02X ", *(p+ciclo));
            	}
            	printf("\n++++++++++++++++++++\n");
                #endif
            	#endif
            }
            break;
			
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
			P_COV_LN;
            break;
			
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
			P_COV_LN;
            break;

        default:
        	ESP_LOGD(TAG, "HTTP EVENT UNKNOW");
			P_COV_LN;
        	break;
    }
    return ESP_OK;
}
#endif



/* Functions implementations ------------------------------------------------------- */


/**
 * @brief      Start a HTTP session
 *             This function must be the first function to call,
 *             and it returns a http_client_handle_t that you must use as input to other functions in the interface.
 *             This call MUST have a corresponding call to http_client_cleanup when the operation is complete.
 *
 * @param[in]  config    The configurations, see `c_http_client_config_t`
 * @param      cert_num  The index of HTTPS certificate to be use
 *
 * @return
 *     - `http_client_handle_t`
 *     - NULL if any errors
 */
 http_client_handle_t http_client_init_IS(c_http_client_config_t *config, C_BYTE cert_num)
 {

	client_http_client_init_IS = NULL;


	PRINTF_DEBUG("%s CERTIFICATO %d \r\n", TAG, cert_num);

	#ifdef INCLUDE_PLATFORM_DEPENDENT 

	esp_config_http_client_init_IS.url = config->url;
	esp_config_http_client_init_IS.username = config->username;
	esp_config_http_client_init_IS.password = config->password;
	esp_config_http_client_init_IS.event_handler = _http_event_handler;
	esp_config_http_client_init_IS.auth_type = HTTP_AUTH_TYPE_BASIC;
	esp_config_http_client_init_IS.cert_pem =  Sys__GetCert((uint8_t)cert_num);
	esp_config_http_client_init_IS.buffer_size = 2048;

	esp_config_http_client_init_IS.timeout_ms = 15000;

	//esp_config_http_client_init_IS.skip_cert_common_name_check = 1;

	//if you need to skip certificate CN validation include the row below
	//esp_config.skip_cert_common_name_check = true;

	client_http_client_init_IS = esp_http_client_init(&esp_config_http_client_init_IS);
    #endif

	return client_http_client_init_IS;
 }
 
 
 
 /**
 * @brief      This function will be open the connection, write all header strings and return
 *
 * @param[in]  client     The http_client handle
 * @param[in]  write_len  HTTP Content length need to write to the server 0 if nothing to do
 *
 * @return
 *  - C_FAIL   
 *  - C_SUCCESS
 */
C_INT32 http_client_open_IS(http_client_handle_t client, C_INT32 write_len)
{
	C_INT32 err_val = C_FAIL;
	
    #ifdef INCLUDE_PLATFORM_DEPENDENT 
	esp_err_t err2;
	err2 = esp_http_client_open(client, 0);
	
	if (err2 != ESP_OK)
	  err_val = C_FAIL;
    else
	  err_val = C_SUCCESS;
	
	#endif		
	
	return err_val;
}

 
 

/**
 * @brief      This function need to call after http_client_open, it will read from http stream, process all receive headers
 *
 * @param[in]  client  The http_client handle
 *
 * @return
 *     - (0) if stream doesn't contain content-length header, or chunked encoding (checked by `esp_http_client_is_chunked` response)
 *     - (-1: C_FAIL) if any errors
 *     - Download data length defined by content-length header
 */
C_INT32 http_client_fetch_headers_IS(http_client_handle_t client)
{
  C_INT32 ret_val = C_FAIL;
 
 
 #ifdef INCLUDE_PLATFORM_DEPENDENT
 ret_val =  esp_http_client_fetch_headers(client);
 #endif
 
 return ret_val;
 
}
 
 
 
 

/**
 * @brief      Read data from http stream
 *
 * @param[in]  client  The http_client handle
 * @param      buffer  The buffer
 * @param[in]  len     The length
 *
 * @return
 *     - C_FAIL if any errors
 *     - Length of data was read
 */
C_INT32 http_client_read_IS(http_client_handle_t client, C_CHAR *buffer, C_INT32 len)
{    
   C_INT32 read_len = C_FAIL;
 
   #ifdef INCLUDE_PLATFORM_DEPENDENT
   read_len = esp_http_client_read(client, buffer, len);
   #endif
   
   return read_len;
}
 
 
 
/**
 * @brief      Close http connection, still kept all http request resources
 *
 * @param[in]  client  The http_client handle
 *
 * @return
 *     - C_SUCCESS
 *     - C_FAIL
 */
C_INT32 http_client_close_IS(http_client_handle_t client)
{
  C_INT32 ret_val = C_FAIL; 
	
  #ifdef INCLUDE_PLATFORM_DEPENDENT 
  esp_err_t err_t;
  err_t = esp_http_client_close(client);
  if (ESP_OK == err_t)
	 ret_val = C_SUCCESS; 
  #endif
 
  return ret_val;
}
 
 
 
 
/**
 * @brief      This function must be the last function to call for a session.
 *             It is the opposite of the http_client_init function and must be called with the same handle as input that a 
               http_client_init call returned.
 *             This might close all connections this handle has used and possibly has kept open until now.
 *
 * @param[in]  client  The http_client handle
 *
 * @return
 *     - C_SUCCESS
 *     - C_FAIL
 */
C_INT32 http_client_cleanup_IS(http_client_handle_t client)
{
 
 C_INT32 ret_val = C_FAIL; 

 #ifdef INCLUDE_PLATFORM_DEPENDENT
 esp_err_t err_t;
 err_t = esp_http_client_cleanup(client);
 if (ESP_OK == err_t)
	 ret_val = C_SUCCESS; 
 #endif

 return ret_val;	
}
	
	
	
	
	
	
	
	
	
	
	
	
	
