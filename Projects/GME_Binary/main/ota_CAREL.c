/**
 * @file   ota_CAREL.c
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to manage the update of the FW of the attached device
 *         via HTTPS, this is the platform independent part
 */

#include "nvm_CAREL.h"
#include "ota_IS.h"
#include "ota_CAREL.h"
#include "modbus_IS.h"
#include "https_client_CAREL.h"
#include "radio.h"

static const char *TAG = "OTA_CAREL";

/** @brief MODBUS_RX_BUFFER_SIZE this is the size allocated on the stack to store the incoming 
    frame DON'T CHANGE IT, is here to have a clear idea of the occupied size
*/
#define MODBUS_RX_BUFFER_SIZE  256

/**
 * @brief UpdateDevFirmware
 *        function that execute the modbus file transfer
 *        over the device attached to the GME.
 *
 * @param  C_BYTE *fw_chunk
 * @param  C_UINT16 ch_size
 * @param  C_UINT16 file_no
 * @param  C_UINT16 starting_reg
 *
 * @return C_RES
 */
C_RES UpdateDevFirmware(C_BYTE *fw_chunk, C_UINT16 ch_size, C_UINT16 file_no, C_UINT16 starting_reg){

	C_RES err = C_SUCCESS;
	
//	C_BYTE data_rx[MODBUS_RX_BUFFER_SIZE] = {0};
	
//	C_INT16 data_rx_len = 0;
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

	crc = CRC16(data_tx, packet_len - 2);
	data_tx[packet_len - 2] = (uint8_t)(crc & 0x00ff);
	data_tx[packet_len - 1] = (uint8_t)((crc >> 8) & 0x00ff);

    #ifdef __CCL_DEBUG_MODE
	printf("\nuart_transmit_bytes len = %d\n",packet_len);
		
	for(int idx=0; idx < packet_len; idx++){
		printf("%02X ",data_tx[idx]);
	}
	
	printf("\n\n");
    #endif

	err = app_file_transfer(data_tx, packet_len);
	printf("app_file_transfer err %d\n", err);

	free(data_tx);

	return err;

}

/**
 * @brief DEV_ota_task
 *        This task connect the GME in a secure way to a https server and
 *        download the fw update. Chunk by chunck download it to a device
 *        attached to the GME via modbus file transfer.
 *
 * @param  void * pvParameter
 *
 * @return none
 */
void DEV_ota_task(void * pvParameter){

	c_cborrequpddevfw * dev_fw_config = (c_cborrequpddevfw*)pvParameter;

	C_RES err = C_FAIL;
	uint8_t cert_num;
	c_http_client_config_t c_config;
	http_client_handle_t client;

	C_UINT16 url_len = strlen(dev_fw_config->uri) + strlen(dev_fw_config->pwd) + strlen(dev_fw_config->usr);
	C_CHAR *url = malloc(url_len + 5);
	if (url == NULL) {
		printf("cannot alloc url\n");
		OTADEVGroup(false);
		vTaskDelete(NULL);
	}

	memset((void*)url, 0, url_len);
	sprintf(url,"%.*s%s:%s@%s", 8, dev_fw_config->uri, dev_fw_config->usr,dev_fw_config->pwd, dev_fw_config->uri+8);
	
	c_config.url = url;
	c_config.username = dev_fw_config->usr;
	c_config.password = dev_fw_config->pwd;

	if(C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
		cert_num = CERT_1;

	client = http_client_init_IS(&c_config, cert_num);
	if ((err = http_client_open_IS(client, 0)) != C_SUCCESS) {
		
		#ifdef __CCL_DEBUG_MODE
		printf("%s Failed to open HTTP connection", TAG);
		#endif
		free(url);
		OTADEVGroup(false);
		vTaskDelete(NULL);
	}

	C_UINT16 file_number = dev_fw_config->fid;
	C_INT32  data_read_len = 0;
	C_UINT32 sent_data_per_file = 0;
	C_UINT16 starting_reg = 0;
	C_BYTE   *upgrade_data_buf = NULL;
	C_INT16  chunk_size = DEV_OTA_BUF_SIZE;
	
	C_BOOL new_file = C_FALSE;

	upgrade_data_buf = (C_BYTE *)malloc(DEV_OTA_BUF_SIZE);

	//TODO CPPCHECK da rivedere completamente dopo NULL check poi procede e non fa return e' sbagliato
	if (upgrade_data_buf == NULL)
	{
		free(url); //TODO CPPCHECK questa viene chiamata 2 volte vedi riga 142
		printf("cannot alloc upgrade_data_buf\n");
		OTADEVGroup(false);
		vTaskDelete(NULL);
	}
	memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(C_BYTE));

	C_INT16 content_length =  http_client_fetch_headers_IS(client);

    #ifdef __CCL_DEBUG_MODE
	printf("%s content_length = %d\n",TAG, content_length);
	#endif

	while(1){

		//chunk_size
		if(sent_data_per_file > MB_FILE_MAX_BYTES - DEV_OTA_BUF_SIZE){
			chunk_size = MB_FILE_MAX_BYTES - sent_data_per_file;
			new_file = true;
		}else{
			chunk_size = DEV_OTA_BUF_SIZE;
		}

		data_read_len = http_client_read_IS(client, (char*)upgrade_data_buf, chunk_size);
		
		if (data_read_len > 0) {
			err = UpdateDevFirmware(upgrade_data_buf, data_read_len, file_number, starting_reg);	//, content_length - sent_data_per_file
			if(err != C_SUCCESS) {
				free(url);
				free(upgrade_data_buf);
				OTADEVGroup(false);
				vTaskDelete(NULL);
			}

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

            #ifdef __CCL_DEBUG_MODE
			printf("%s Written image length %d\n", TAG, sent_data_per_file);
			#endif
		}
		else {
			printf("closing update\n");
			http_client_close_IS(client);
			http_client_cleanup_IS(client);

			uart_flush_input_IS(modbusPort);
			uart_flush_IS(modbusPort);

			if (data_read_len == 0) {
				err = UpdateDevFirmware(upgrade_data_buf, 0, file_number, starting_reg);
				if(err != C_SUCCESS) {
					free(url);
					free(upgrade_data_buf);
					OTADEVGroup(false);
					vTaskDelete(NULL);
				}

				#ifdef __CCL_DEBUG_MODE
				printf("%s %s\r\n", TAG, "Connection closed,all data received");
				#endif
				free(url);
				free(upgrade_data_buf);
				OTADEVGroup(true);
				vTaskDelete(NULL);
			}

			if (data_read_len < 0) {
				#ifdef __CCL_DEBUG_MODE
				printf("%s %s\r\n", TAG, "Error: SSL data read error");
				#endif
				free(url);
				free(upgrade_data_buf);
				OTADEVGroup(false);
				vTaskDelete(NULL);
			}
		}
	}
	free(url);
	free(upgrade_data_buf);
}


/**
 * @brief Model_ota_task
 *        This task download via OTA the model
 *        used by GME to start polling the modbus variables.
 *
 * @param  void * pvParameter
 *
 * @return none
 */
void Model_ota_task(void * pvParameter)
{
	c_cborreqdwldevsconfig * myCborUpdate = (c_cborreqdwldevsconfig*)pvParameter;
	C_RES err;
	uint8_t cert_num = CERT_1;

	// get current certificate number and download model
	if(C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
		cert_num = CERT_1;

	err = HttpsClient__DownloadFile(myCborUpdate, cert_num, MODEL_FILE);
	#ifdef __CCL_DEBUG_MODE
	printf("execute_download_devs_config err= %d \n",err);
	#endif
	if(CONN_OK == err){
		// model file has been saved, report it in nvm
		// save also corresponding cid and did
		// save also dev
		if( (C_SUCCESS == NVM__WriteU8Value(SET_DEVS_CONFIG_NVM, CONFIGURED)) &&
			(C_SUCCESS == NVM__WriteU32Value(MB_CID_NVM, myCborUpdate->cid)) &&
			(C_SUCCESS == NVM__WriteU32Value(MB_DID_NVM, myCborUpdate->did)) &&
			(C_SUCCESS == NVM__WriteU32Value(MB_DEV_NVM, myCborUpdate->dev)) ){
            #ifdef __CCL_DEBUG_MODE
			printf("MODEL FILE, CID, DID AND DEV SAVED\n");
            #endif
			err = C_SUCCESS;
		}else{
            #ifdef __CCL_DEBUG_MODE
			printf("CBOR MODEL FILE, CID, DID AND DEV NOT SAVED\n");
            #endif
			err = C_FAIL;
		}
	}else
		err = C_FAIL;

	err == C_SUCCESS ? CBOR_SendAsyncResponseDid(0, myCborUpdate->did) : CBOR_SendAsyncResponseDid(1, myCborUpdate->did);

	// reboot if everything went fine
	if ( err == C_SUCCESS )
		GME__Reboot();

	vTaskDelete(NULL);
}

/**
 * @brief CA_ota_task
 *        This task download via OTA the new certificate
 *        used to connect in a secure way into the Iot services.
 *
 * @param  void * pvParameter
 *
 * @return none
 */
void CA_ota_task(void * pvParameter)
{
	c_cborrequpdatecacert * myCborUpdate = (c_cborrequpdatecacert*)pvParameter;
	C_RES err;

	err = HttpsClient__UpdateCertificate(myCborUpdate);
	err == C_SUCCESS ? CBOR_SendAsyncResponse(0) : CBOR_SendAsyncResponse(1);

	if (err == C_SUCCESS)
		ESP_LOGI(TAG, "Certificate upgrade succeeded");
	else
		ESP_LOGE(TAG, "Certificate upgrade failed");

    #ifdef __CCL_DEBUG_MODE
	printf("execute_update_ca_cert err= %d \n",err);
    #endif
	// restart polling if needed
	uint8_t pe_status;
	NVM__ReadU8Value(PE_STATUS_NVM, &pe_status);
	if (pe_status == RUNNING)
		PollEngine_StartEngine_CAREL();	// restart polling

//	Modbus_Enable();

	vTaskDelete(NULL);
}

/**
 * @brief GME_ota_task
 *        This task download via OTA through an https
 *        server the GME firmware update.
 *		  Refer to the function "https_ota(...)" in ota_IS.c
 *		  for more details.
 *
 * @param  void * pvParameter
 *
 * @return none
 */
void GME_ota_task(void * pvParameter)
{
	c_cborrequpdgmefw * myCborUpdate = (c_cborrequpdgmefw*)pvParameter;
	c_http_client_config_t c_config;
	uint8_t cert_num;

    ESP_LOGI(TAG, "Starting OTA ...");
    /* Wait for the callback to set the CONNECTED_BIT in the
       event group.
    */
    Radio__WaitConnection();

    uint16_t url_len = strlen(myCborUpdate->uri) +
    		 	       strlen(myCborUpdate->usr) +
					   strlen(myCborUpdate->pwd);

    char *url = malloc(url_len+5);

	memset((void*)url, 0, url_len);
	sprintf(url,"%.*s%s:%s@%s",8,myCborUpdate->uri, myCborUpdate->usr, myCborUpdate->pwd, myCborUpdate->uri+8);

	c_config.url = url;
	c_config.username = myCborUpdate->usr;
	c_config.password = myCborUpdate->pwd;


	if(C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
		cert_num = CERT_1;

	c_config.cert_num = cert_num;

    C_RES ret = https_ota(&c_config);

    Modbus_Enable();

    if (ret == C_SUCCESS) {
    	ESP_LOGI(TAG, "Firmware Upgrades Succeeded");
    	//Send true to ota group to send the res and restart gme
    	free(url);
    	OTAGroup(true);
    	vTaskDelete(NULL);
    } else {
    	ESP_LOGE(TAG, "Firmware Upgrades Failed");
    	//Send false to ota group to send the res and continue working
    	free(url);
    	OTAGroup(false);
    	vTaskDelete(NULL);
    }
}
