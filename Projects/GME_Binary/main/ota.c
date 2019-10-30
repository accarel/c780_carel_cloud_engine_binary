/*
 * ota.c
 *
 *  Created on: Sep 24, 2019
 *      Author: ataayoub
 */


/* OTA example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "common.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_partition.h"
#include "ota.h"
#include "wifi.h"

#include "file_system.h"

#include "poll_engine.h"
#include "sys.h"

/*The maximum allowed packet's size to be sent through modbus is 253 bytes,
*	so 253(total) - 9(write file cmd header) - 2(CRC) = 242
*/
#define DEV_OTA_BUF_SIZE 240
#define MB_FILE_MAX_BYTES 20000

static const char *TAG = "OTA";
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





void GME_ota_task(void * pvParameter)
{
#if 0
    ESP_LOGI(TAG, "Starting OTA ...");
    /* Wait for the callback to set the CONNECTED_BIT in the
       event group.
    */
    WiFi__WaitConnection();


    esp_http_client_config_t config = {
        .url = JSON__Get_GME_FWurl(),
        .cert_pem = Sys__GetCert(CERT_1),
		.event_handler = _http_event_handler,
		.auth_type = HTTP_AUTH_TYPE_BASIC,
    };

    printf("url : %s\n",config.url);

    //This function manage the ota operation, the checksum validation and the ota partition switch
    esp_err_t ret = esp_https_ota(&config);

    if (ret != ESP_OK) {
    	//Try the second Cert
		config.cert_pem = Sys__GetCert(CERT_2);
		ret = esp_https_ota(&config);
		if (ret == ESP_OK){
			ESP_LOGI(TAG, "Firmware Upgrades Succeeded 2");
			//Send true to ota group to send the res and restart gme
			JSON__OTAGroup(true);
		}else{
			ESP_LOGE(TAG, "Firmware Upgrades Failed");
			//Send false to ota group to send the res and continue working
			JSON__OTAGroup(false);
			vTaskDelete(NULL);
		}

    } else {

    	ESP_LOGI(TAG, "Firmware Upgrades Succeeded 1");
    	//Send true to ota group to send the res and restart gme
    	JSON__OTAGroup(true);
    }
#endif
}



void OTA__GMEInit(void)
{
    xTaskCreate(&GME_ota_task, "GME_ota_task", 8192, NULL, 5, NULL);
}




esp_err_t UpdateDevFirmware(uint8_t *fw_chunk, uint16_t ch_size, uint16_t file_no, uint16_t starting_reg){

	esp_err_t err = ESP_OK;
	uint8_t data_rx[256] = {0};
	uint8_t data_rx_len = 0;
	uint8_t *data_tx;
	uint8_t packet_len;

	//header vars
	uint16_t record_data_reg_no = 0;
	uint16_t crc;
	uint16_t i,j;


	ch_size = ch_size/2 + ch_size%2;
	record_data_reg_no = ch_size * 2;
	packet_len = record_data_reg_no + MAX_HEADER_BYTES + 2;		// + 2 for crc

	data_tx = malloc(packet_len);
	memset((void*)data_tx, 0, packet_len);

	data_tx[SLAVE_ADD] = 0x01;
	data_tx[FUNC_CODE] = 0x15;
	data_tx[REQ_DATA_LEN] = packet_len - 3 - 2;		//2 for func_code and req_data_len 	and	 2 for crc
	data_tx[REF_TYPE] = 0x06;

	data_tx[FILE_NO_LO] = (uint8_t)(file_no & 0x00FF);
	data_tx[FILE_NO_HI] = (uint8_t)((file_no >> 8) & 0x00FF);

	data_tx[RECORD_NO_LO] = (uint8_t)(starting_reg & 0x00FF);;
	data_tx[RECPRD_NO_HI] = (uint8_t)((starting_reg >> 8) & 0x00FF);

	data_tx[RECORD_LEN_LO] = (uint8_t)(ch_size & 0x00FF);;
	data_tx[RECORD_LEN_HI] = (uint8_t)((ch_size >> 8) & 0x00FF);


	for(i = MAX_HEADER_BYTES, j = 0; i<packet_len-2; i+=2, j+=2){
		data_tx[i] = fw_chunk[j];
		data_tx[i+1] = fw_chunk[j+1];
	}

//	crc = CRC16(data_tx, packet_len - 2);
//
//	data_tx[packet_len - 2] = (uint8_t)(crc & 0x00ff);
//	data_tx[packet_len - 1] = (uint8_t)((crc >> 8) & 0x00ff);
//
//	ClearQueueMB();

	printf("\nuart_transmit_bytes len = %d\n",packet_len);
	for(int i=0; i<packet_len; i++){
		printf("%02X ",data_tx[i]);
	}
	printf("\n\n");

	for(int i=0; i<packet_len; i++)
	{
		uart_write_bytes(MB_PORTNUM, (const char *) &data_tx[i], 1);
	}

	data_rx_len = uart_read_bytes(MB_PORTNUM, data_rx, 256,  MB_RESPONSE_TIMEOUT(packet_len));

	printf("uart_read_bytes len = %d\n",data_rx_len);

	for(int i=0; i<data_rx_len; i++){
		printf("%02X ",data_rx[i]);
	}
	printf("\n");

	//TODO Check on incoming data

	/*if(data_rx_len != packet_len){

		printf("Received packet length doesn't match the transmitted packet length\n");
		err = ESP_FAIL;
	}else{
		if(data_rx[data_rx_len-1] != data_tx[packet_len-1] || data_rx[data_rx_len-2] != data_tx[packet_len-2]){
			printf("Received packet content doesn't match the transmitted packet content\n");
			err = ESP_FAIL;
		}else{
			err = ESP_OK;
		}
	}*/

	free(data_tx);

	return err;

}





esp_err_t OTA__DevFWInit(req_update_dev_fw_t *dev_fw_config){

	esp_err_t err, ret = ESP_FAIL;


	uint16_t url_len = strlen(dev_fw_config->uri) + strlen(dev_fw_config->password) + strlen(dev_fw_config->username);
	char *url = malloc(url_len + 5);
	memset((void*)url, 0, url_len);

	sprintf(url,"%.*s%s:%s@%s",8,dev_fw_config->uri,dev_fw_config->username,dev_fw_config->password,dev_fw_config->uri+8);

	esp_http_client_config_t config = {
		.url = url,
		.event_handler = _http_event_handler,
		.auth_type = HTTP_AUTH_TYPE_BASIC,
		.cert_pem = Sys__GetCert(CERT_1),
	};

	esp_http_client_handle_t client = esp_http_client_init(&config);

	if ((err = esp_http_client_open(client, 0)) != ESP_OK) {
		ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));

		config.cert_pem = Sys__GetCert(CERT_2);

		memset((void*)client, 0, sizeof(esp_http_client_handle_t));
		client = esp_http_client_init(&config);

		if ((err = esp_http_client_open(client, 0)) != ESP_OK) {

			ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
			free(url);
			return ESP_FAIL;
		}
	}

	uint16_t file_number = dev_fw_config->file_index;
	int32_t data_read_len = 0;
	uint32_t sent_data_per_file = 0;
	uint16_t starting_reg = 0;
	uint8_t *upgrade_data_buf = NULL;
	int chunk_size = DEV_OTA_BUF_SIZE;
	bool new_file=false;

	upgrade_data_buf = (uint8_t *)malloc(DEV_OTA_BUF_SIZE);
	memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(uint8_t));

	int content_length =  esp_http_client_fetch_headers(client);

	printf("content_length = %d\n",content_length);

//	mbc_master_suspend();
	while(1){

		//chunk_size
		if(sent_data_per_file > MB_FILE_MAX_BYTES - DEV_OTA_BUF_SIZE){
			chunk_size = MB_FILE_MAX_BYTES - sent_data_per_file;
			new_file = true;
		}else{
			chunk_size = DEV_OTA_BUF_SIZE;
		}

		data_read_len = esp_http_client_read(client, (char*)upgrade_data_buf, chunk_size);

		if (data_read_len == 0) {
			err = UpdateDevFirmware(upgrade_data_buf, 0, file_number, starting_reg);
			ESP_LOGI(TAG, "Connection closed,all data received");
			break;
		}
		if (data_read_len < 0) {
			ESP_LOGE(TAG, "Error: SSL data read error");
			break;
		}
		if (data_read_len > 0) {

			for(int i=0; i<data_read_len; i++){
				printf("%02X ",upgrade_data_buf[i]);
			}
			printf("\n");

			err = UpdateDevFirmware(upgrade_data_buf, data_read_len, file_number, starting_reg);	//, content_length - sent_data_per_file

			if(sent_data_per_file > MB_FILE_MAX_BYTES - DEV_OTA_BUF_SIZE){
				file_number++;
				starting_reg = 0;
			}else{
				sent_data_per_file += data_read_len;
				starting_reg = sent_data_per_file/2;
			}

			if(new_file == true){
				sent_data_per_file = 0;
				new_file=false;
			}/*else{
				sent_data_per_file += data_read_len;
			}*/

			printf("Written image length %d\n", sent_data_per_file);
		}
	}

	esp_http_client_close(client);
	esp_http_client_cleanup(client);

	uart_flush_input(MB_PORTNUM);
	uart_flush(MB_PORTNUM);

//	ClearQueueMB();
//	mbc_master_resume();

	free(url);
	free(upgrade_data_buf);
	return err;
}



