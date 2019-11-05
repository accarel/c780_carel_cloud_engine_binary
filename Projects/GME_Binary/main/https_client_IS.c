/**
 * @file   https_client_IS.c
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the download of some files via HTTPS
 */
 
 
/* Includes ------------------------------------------------------------------------ */
#include "https_client_IS.h"
#include "sys.h"
#include "esp_http_client.h"

static const char *TAG = "HTTP_CLIENT_IS";


#ifdef INCLUDE_PLATFORM_DEPENDENT
/*Description : http task events handler
 *              all the functions in this files are using the same event handler
 * TODO BILATO  da trasformare in eliminabile automaticamente
 */
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
            if (!esp_http_client_is_chunked_response(evt->client)) {
            }
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
 http_client_handle_t http_client_init(c_http_client_config_t *config, C_BYTE cert_num)
 {
	http_client_handle_t client = NULL;

	#ifdef INCLUDE_PLATFORM_DEPENDENT 
	esp_http_client_config_t c_config;
	 
	c_config.url           = config->url;
	c_config.event_handler = _http_event_handler;
	c_config.auth_type     = HTTP_AUTH_TYPE_BASIC;
	c_config.cert_pem      = Sys__GetCert((uint8_t)cert_num);
    
	client = esp_http_client_init(&c_config);
    #endif

	return client;
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
C_INT32 http_client_open(http_client_handle_t client, C_INT32 write_len)
{
	C_INT32 err_val = C_FAIL;
	
    #ifdef INCLUDE_PLATFORM_DEPENDENT 
	esp_err_t err2;	
	err2 = http_client_open(client, 0);
	
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
C_INT32 http_client_fetch_headers(http_client_handle_t client)
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
C_INT32 http_client_read(http_client_handle_t client, C_CHAR *buffer, C_INT32 len)
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
C_INT32 http_client_close(http_client_handle_t client)
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
C_INT32 http_client_cleanup(http_client_handle_t client)
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
	
	
	
	
	
	
	
	
	
	
	
	
	
