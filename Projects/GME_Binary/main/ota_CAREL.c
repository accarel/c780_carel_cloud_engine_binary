/**
 * @file   ota_CAREL.c
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the update of the FW of the attached device 
 *         via HTTPS, this is the platform independent part
 */

#include "common.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "wifi.h"
#include "file_system.h"
#include "poll_engine.h"
#include "sys.h"

#include "https_client_IS.h"

#include "ota_IS.h"
#include "ota_CAREL.h"


static const char *TAG = "OTA_CAREL";


/** @brief MODBUS_RX_BUFFER_SIZE this is the size allocated on the stack to store the incoming 
    frame DON'T CHANGE IT, is here to have a clear idea of the occupied size
*/
#define MODBUS_RX_BUFFER_SIZE  256


C_RES UpdateDevFirmware(C_BYTE *fw_chunk, C_UINT16 ch_size, C_UINT16 file_no, C_UINT16 starting_reg){

	C_RES err = C_SUCCESS;
	
	C_BYTE data_rx[MODBUS_RX_BUFFER_SIZE] = {0};
	
	C_BYTE data_rx_len = 0;
	C_BYTE *data_tx;
	C_BYTE packet_len;

	//header vars
	C_UINT16 record_data_reg_no = 0;
	C_UINT16 crc;
	C_UINT16 i,j;


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

    #ifdef __CCL_DEBUG_MODE
	printf("\nuart_transmit_bytes len = %d\n",packet_len);
		
	for(int i=0; i<packet_len; i++){
		printf("%02X ",data_tx[i]);
	}
	
	printf("\n\n");
    #endif


	for(int i=0; i<packet_len; i++)
	{
		//TODO BILATO uart_write_bytes_IS(MB_PORTNUM, (const C_BYTE *) &data_tx[i], 1);
	}

	//TODO BILATO data_rx_len = uart_read_bytes_IS(MB_PORTNUM, data_rx, 256,  MB_RESPONSE_TIMEOUT(packet_len));


    #ifdef __CCL_DEBUG_MODE
	printf("uart_read_bytes len = %d\n",data_rx_len);

	for(int i=0; i<data_rx_len; i++){
		printf("%02X ",data_rx[i]);
	}
	printf("\n");
	#endif

	//TODO Check on incoming data not need but in the case is here

	/*if(data_rx_len != packet_len){

		printf("Received packet length doesn't match the transmitted packet length\n");
		err = C_FAIL;
	}else{
		if(data_rx[data_rx_len-1] != data_tx[packet_len-1] || data_rx[data_rx_len-2] != data_tx[packet_len-2]){
			printf("Received packet content doesn't match the transmitted packet content\n");
			err = C_FAIL;
		}else{
			err = C_SUCCESS;
		}
	}*/

	free(data_tx);

	return err;

}




C_RES OTA__DevFWUpdate(c_cborrequpddevfw *dev_fw_config){

	C_RES err, ret = C_FAIL;

	c_http_client_config_t c_config;
	http_client_handle_t client;

	C_UINT16 url_len = strlen(dev_fw_config->uri) + strlen(dev_fw_config->pwd) + strlen(dev_fw_config->usr);
	C_CHAR *url = malloc(url_len + 5);

	if (url == NULL)
	{
		return C_FAIL;
	}


	memset((void*)url, 0, url_len);
	sprintf(url,"%.*s%s:%s@%s", 8, dev_fw_config->uri, dev_fw_config->usr,dev_fw_config->pwd, dev_fw_config->uri+8);
	
	c_config.url = dev_fw_config->uri;
	c_config.username = dev_fw_config->usr;
	c_config.password = dev_fw_config->pwd;

	client = http_client_init_IS(&c_config, CERT_1);

	if ((err = http_client_open_IS(client, 0)) != C_SUCCESS) {
		
		#ifdef __CCL_DEBUG_MODE
		printf("%s Failed to open HTTP connection", TAG);
		#endif

		memset((void*)client, 0, sizeof(http_client_handle_t));

		client = http_client_init_IS(&c_config, CERT_2);

		if ((err = http_client_open_IS(client, 0)) != C_SUCCESS) {

            #ifdef __CCL_DEBUG_MODE
			printf("%s Failed to open HTTP connection", TAG);
			#endif
						
			free(url);
			return C_FAIL;
		}
	}

	C_UINT16 file_number = dev_fw_config->fid;
	C_INT32  data_read_len = 0;
	C_UINT32 sent_data_per_file = 0;
	C_UINT16 starting_reg = 0;
	C_BYTE   *upgrade_data_buf = NULL;
	C_INT16  chunk_size = DEV_OTA_BUF_SIZE;
	
	C_BOOL new_file = C_FALSE;

	upgrade_data_buf = (C_BYTE *)malloc(DEV_OTA_BUF_SIZE);

	if (upgrade_data_buf == NULL)
	{
		free(url);
		return C_FAIL;
	}

	memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(C_BYTE));

	C_INT16 content_length =  http_client_fetch_headers_IS(client);

    #ifdef __CCL_DEBUG_MODE
	printf("%s content_length = %d\n",TAG, content_length);
	#endif


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
			#ifdef __CCL_DEBUG_MODE 
			printf("%s %s\r\n", TAG, "Connection closed,all data received");
			#endif
			
			break;
		}
		
		if (data_read_len < 0) {
			#ifdef __CCL_DEBUG_MODE 
			printf("%s %s\r\n", TAG, "Error: SSL data read error");
			#endif
			break;
		}
		
		if (data_read_len > 0) {

            #ifdef __CCL_DEBUG_MODE 
			for(int i=0; i<data_read_len; i++){
				printf("%s DBUFF - %02X ",TAG, upgrade_data_buf[i]);
			}
			printf("\n");
			#endif

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
			}
			/*
			else
			{
				sent_data_per_file += data_read_len;
			}
			*/

            #ifdef __CCL_DEBUG_MODE
			printf("%s Written image length %d\n", TAG, sent_data_per_file);
			#endif
		}
	}

	http_client_close_IS(client);
	http_client_cleanup_IS(client);

	uart_flush_input_IS(MB_PORTNUM);
	uart_flush_IS(MB_PORTNUM);

//	ClearQueueMB();
//	mbc_master_resume();

	free(url);
	free(upgrade_data_buf);
	
	return err;
}

