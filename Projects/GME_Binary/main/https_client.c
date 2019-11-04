/**
 * @file   https_client_CAREL.c
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the download of some files via HTTPS
 */
#include "CAREL_GLOBAL_DEF.h"
#include "http_client_IS.h"
#include "https_client_CAREL.h"
#include "poll_engine.h"
#include "sys.h"
#include "MQTT_Interface_CAREL.h"


/**
 * @brief MAX_HTTP_RECV_BUFFER 
 *        is used to store temporarily the data coming from the HTTPS server the 
 *        space depends from the available memory
 */
#define MAX_HTTP_RECV_BUFFER 2048


#ifdef __CCL_DEBUG_MODE 
/**
 * @brief _DEBUG_HTTPS_CLIENT_CAREL 
 *        if defined print on the debug console some debugger message but only if 
 *        global __CCL_DEBUG_MODE is defined
 */
#define _DEBUG_HTTPS_CLIENT_CAREL 
static const char *TAG = "HTTP_CLIENT_CAREL";
#endif




/**
 * @brief HttpsClient__DownloadFile
 *          Routine for downloading a file's from the server using the passed certificate
 *          Downloads the file and rewrite it in the filesystem*        
 *        
 * @param   download_devs_config : the connection data uri/password/username
 *
 * @param   cert_num : the index of certificate to be used
 * @param   filename : the name of the file that will be updated
 * @return 
 */
https_conn_err_t HttpsClient__DownloadFile(req_download_devs_config_t *download_devs_config, uint8_t cert_num, const char *filename)
{	
	https_conn_err_t err = CONN_OK;	
	c_http_client_config_t c_config 
		
    C_INT err2;
	http_client_handle_t client;
		
	int total_read_len = 0;
    int read_len = 0;
	int content_length;
	
	char *buffer = malloc(MAX_HTTP_RECV_BUFFER + 1);
	uint16_t url_len = strlen(download_devs_config->uri) + strlen(download_devs_config->password) + strlen(download_devs_config->username);
	char *url = malloc(url_len+5);

    if (buffer == NULL) 
		return NO_HEAP_MEMORY;
	
	if (url == NULL) 
		return NO_HEAP_MEMORY;

	memset((void*)url, 0, url_len);    
	sprintf(url,"%.*s%s:%s@%s",8,download_devs_config->uri,download_devs_config->username,download_devs_config->password,download_devs_config->uri+8);
	
	#ifdef _DEBUG_HTTPS_CLIENT_CAREL 
	printf("%s\n",url);
	#endif

	c_config.url = download_devs_config->uri;
	c_config.username = download_devs_config->username;
	c_config.password = download_devs_config->password;
	c_config.cert_num = cert_num;
	
	client = http_client_init(&config, cert_num);


	if ((err2 = http_client_open(client, 0)) != C_SUCCESS) {
		#ifdef _DEBUG_HTTPS_CLIENT_CAREL 		
		//ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err2));				
		PRINTF_DEBUG("Failed to open HTTP connection");		
		#endif
		
		free(buffer);
		free(url);
		return CONN_FAIL;
	}


	content_length =  http_client_fetch_headers(client);
	
	
	if (total_read_len < content_length && content_length <= MAX_HTTP_RECV_BUFFER) {
		
		read_len = http_client_read(client, buffer, content_length);
		
		if (read_len >= 0) 
		{
		  buffer[read_len] = 0;

          #if (DEEP_DEBUG_PRINTF_DEFAULT == 1)
		  ESP_LOGD(TAG, "read_len = %d", read_len);
		  ESP_LOG_BUFFER_HEXDUMP(__func__, buffer, MAX_HTTP_RECV_BUFFER, ESP_LOG_INFO);
          #endif


          /* TODO BILATO
		  if (C_SUCCESS != FS_SaveFile(buffer , (size_t)read_len, filename)){
		    err = FILE_NOT_SAVED;
		    }
			
			
		  */
			
		}
		else
		{
			#ifdef _DEBUG_HTTPS_CLIENT_CAREL 
			PRINTF_DEBUG("HttpsClient__DownloadModelFile - Error read data");			
			#endif
		}

		
	}


    #ifdef _DEBUG_HTTPS_CLIENT_CAREL 
	ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %d",
     				esp_http_client_get_status_code(client),
					esp_http_client_get_content_length(client));
    #endif					


	free(buffer);
	free(url);

	http_client_close(client);
	http_client_cleanup(client);
	return err;
}



/**
 * @brief HttpsClient__UpdateCertificate
 *        Routine for downloading an update a new certificate file's from the server 
 *        This routine use the current certificate to log in and update the second one
 *        So in short if you pass 1 it will update the no.2
 *                    if you pass 2 it will update the no.1
 *        
 * @param  download_devs_config : the connection data uri/password/username
 *
 * @param   cert_num : the index of certificate to be used to log in 
 * @return 
 */

https_conn_err_t HttpsClient__UpdateCertificate(req_download_devs_config_t *update_ca_cert, C_BYTE cert_num)
{
	C_BYTE filename[32];
	
	
    //check the passed certificate and update the other one
	if(CERT_1 == cert_num)
	{
	  strcpy(filename, CERT2_SPIFFS);	
	}else if (CERT_2 == cert_num){
	  strcpy(filename, CERT1_SPIFFS);	
	}
		
	return HttpsClient__DownloadFile(update_ca_cert, cert_num, &filename[0])
}




#ifdef INCLUDE_PLATFORM_DEPENDENT
//Description: Task called from cmdline to test carel server
void HTTPClient__TestTask(void *pvParameters)
{
    ESP_LOGI(TAG, "Connected to AP, begin http example");
    PollEngine__MBSuspend();
    https_carel_server_test();

    PollEngine__MBResume();
    vTaskDelete(NULL);
}
#endif