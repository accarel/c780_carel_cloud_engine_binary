/*
 * https_client.h
 *
 *  Created on: Jun 21, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#include "https_client.h"
#include "wifi.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "file_system.h"
#include "poll_engine.h"

#include "esp_http_client.h"
#include "sys.h"
#include "MQTT_Interface_CAREL.h"

#define MAX_HTTP_RECV_BUFFER 2048	//1045
static const char *TAG = "HTTP_CLIENT";



/*	Description:http task events handler
 * 				all the functions in this files are using the same event handler
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


//Description: Testing https connection on carel server using the spiffs certificates , used from CMD LINE
static void https_carel_server_test()
{
char username_devs[20] = "gme";
char pass_devs[20] = "+q9aOC-&EejZuV;~";
char uri_devs[100] = "https://mqtt-dev.tera.systems:8080/";

	esp_err_t err;

		char *buffer = malloc(MAX_HTTP_RECV_BUFFER + 1);

		if (buffer == NULL) {
			ESP_LOGE(TAG, "Cannot malloc http receive buffer");
		}

		uint16_t url_len = strlen(username_devs) + strlen(pass_devs) + strlen(uri_devs);
		char *url = malloc(url_len+5);
		memset((void*)url, 0, url_len);
		sprintf(url,"%.*s%s:%s@%s",8,uri_devs,username_devs,pass_devs,uri_devs+8);

		PRINTF_DEBUG("%s\n",url);

		esp_http_client_config_t config = {
			.url = url,
			.event_handler = _http_event_handler,
			.auth_type = HTTP_AUTH_TYPE_BASIC,
			.cert_pem = Sys__GetCert(CERT_1),
		};


		esp_http_client_handle_t client = esp_http_client_init(&config);


		if ((err = esp_http_client_open(client, 0)) != ESP_OK) {
			ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
			free(buffer);
			free(url);
		}else{


		ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %d",
						esp_http_client_get_status_code(client),
						esp_http_client_get_content_length(client));
		esp_http_client_close(client);
		esp_http_client_cleanup(client);

		free(buffer);
		free(url);
		}
}


/*	Description:Routine for downloading the file's model from the server using the passed certificate
 * 				Downloads the model file and rewrite it in the spiffs
 */

https_conn_err_t HttpsClient__DownloadModelFile(req_download_devs_config_t *download_devs_config, uint8_t cert_num)
{
	https_conn_err_t err = CONN_OK;
	esp_err_t err2;

	char *buffer = malloc(MAX_HTTP_RECV_BUFFER + 1);

	uint16_t url_len = strlen(download_devs_config->uri) + strlen(download_devs_config->password) + strlen(download_devs_config->username);
	char *url = malloc(url_len+5);
	memset((void*)url, 0, url_len);

	sprintf(url,"%.*s%s:%s@%s",8,download_devs_config->uri,download_devs_config->username,download_devs_config->password,download_devs_config->uri+8);

	printf("%s\n",url);



	esp_http_client_config_t config = {
		.url = url,
		.event_handler = _http_event_handler,
		.auth_type = HTTP_AUTH_TYPE_BASIC,
		.cert_pem =  Sys__GetCert(cert_num),
	};

	esp_http_client_handle_t client = esp_http_client_init(&config);

	if ((err2 = esp_http_client_open(client, 0)) != ESP_OK) {
		ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err2));
		free(buffer);
		free(url);
		return CONN_FAIL;
	}


	int content_length =  esp_http_client_fetch_headers(client);
	int total_read_len = 0, read_len;
	if (total_read_len < content_length && content_length <= MAX_HTTP_RECV_BUFFER) {
		read_len = esp_http_client_read(client, buffer, content_length);
		if (read_len <= 0) {
			ESP_LOGE(TAG, "Error read data");
		}
		buffer[read_len] = 0;
#if (DEEP_DEBUG_PRINTF_DEFAULT == 1)
		ESP_LOGD(TAG, "read_len = %d", read_len);
		ESP_LOG_BUFFER_HEXDUMP(__func__, buffer, MAX_HTTP_RECV_BUFFER, ESP_LOG_INFO);
#endif
		if( ESP_OK != FS_SaveFile(buffer , (size_t)read_len, MODEL_FILE)){
			err = FILE_NOT_SAVED;
		}
	}

	ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %d",
					esp_http_client_get_status_code(client),
					esp_http_client_get_content_length(client));


	free(buffer);
	free(url);

	esp_http_client_close(client);
	esp_http_client_cleanup(client);

	return err;
}


/*	Description:Updating certificates routine, using the passed certificate
 * 				The function downloads the new certificate and updates the not used certificate
 */
https_conn_err_t HttpsClient__UpdateCertificate(req_update_ca_cert_t *update_ca_cert, uint8_t cert_num){

	https_conn_err_t err = CONN_OK;
	esp_err_t err2;

		char *buffer = malloc(MAX_HTTP_RECV_BUFFER + 1);

		uint16_t url_len = strlen(update_ca_cert->uri) + strlen(update_ca_cert->password) + strlen(update_ca_cert->username);
		char *url = malloc(url_len+5);
		memset((void*)url, 0, url_len);

		sprintf(url,"%.*s%s:%s@%s",8,update_ca_cert->uri,update_ca_cert->username,update_ca_cert->password,update_ca_cert->uri+8);

		esp_http_client_config_t config = {
			.url = url,
			.event_handler = _http_event_handler,
			.auth_type = HTTP_AUTH_TYPE_BASIC,
			.cert_pem = Sys__GetCert(cert_num),
		};



		esp_http_client_handle_t client = esp_http_client_init(&config);
		if ((err2 = esp_http_client_open(client, 0)) != ESP_OK) {
			ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err2));
			free(buffer);
			free(url);
			return CONN_FAIL;
		}

		int content_length =  esp_http_client_fetch_headers(client);
		int total_read_len = 0, read_len;
		if (total_read_len < content_length && content_length <= MAX_HTTP_RECV_BUFFER) {
			read_len = esp_http_client_read(client, buffer, content_length);
			if (read_len <= 0) {
				ESP_LOGE(TAG, "Error read data");
			}
			buffer[read_len] = 0;
			ESP_LOGD(TAG, "read_len = %d", read_len);
			ESP_LOG_BUFFER_HEXDUMP(__func__, buffer, MAX_HTTP_RECV_BUFFER, ESP_LOG_INFO);

		//check the passed certificate and update the other one
			if(CERT_1 == cert_num)
			{
				if( ESP_OK != FS_SaveFile(buffer , (size_t)read_len, CERT2_SPIFFS)){
					err = FILE_NOT_SAVED;
				}

			}else if (CERT_2 == cert_num){
				if( ESP_OK != FS_SaveFile(buffer , (size_t)read_len, CERT1_SPIFFS)){
					err = FILE_NOT_SAVED;
				}

			}

		}

		ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %d",
						esp_http_client_get_status_code(client),
						esp_http_client_get_content_length(client));
		esp_http_client_close(client);
		esp_http_client_cleanup(client);


		free(buffer);
		free(url);

		return err;

}


//Description: Task called from cmdline to test carel server
void HTTPClient__TestTask(void *pvParameters)
{
    ESP_LOGI(TAG, "Connected to AP, begin http example");
    PollEngine__MBSuspend();
    https_carel_server_test();

    PollEngine__MBResume();
    vTaskDelete(NULL);
}

