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
#include "WebDebug.h"
#include "sys_IS.h"
#include "unlock_CAREL.h"


static const char *TAG = "OTA_CAREL";

/** @brief MODBUS_RX_BUFFER_SIZE this is the size allocated on the stack to store the incoming 
    frame DON'T CHANGE IT, is here to have a clear idea of the occupied size
*/
#define MODBUS_RX_BUFFER_SIZE  256

#define LENGTH_OF_CHUNK    (200)

#define MAX_CLIENT_INIT_RETRY 3

/*
 * @brief dbg_ota_status
 *
 * */
C_BYTE   dbg_ota_status;
C_UINT32 content_length;
C_UINT32 dbg_traferred_lenght;


void dbg_Set_OTA_Status(C_BYTE status){
	dbg_ota_status = status;
}

void dbg_Set_OTA_Trasf_len(C_UINT32 len){
	dbg_traferred_lenght = len;
}


C_BYTE dbg_Get_OTA_Status(void){
	return dbg_ota_status;
}

C_UINT32 dbg_Get_OTA_Content_Lenght(void){
	return content_length;
}




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

    #ifdef __DEBUG_OTA_CAREL_LEV_2
	PRINTF_DEBUG("\nuart_transmit_bytes len = %d\n",packet_len);
		
	for(int idx=0; idx < packet_len; idx++){
		PRINTF_DEBUG("%02X ",data_tx[idx]);
	}
	
	PRINTF_DEBUG("\n\n");
    #endif

	err = app_file_transfer(data_tx, packet_len);

    #ifdef __DEBUG_OTA_CAREL_LEV_2
	PRINTF_DEBUG("app_file_transfer err %d\n", err);
    #endif

	free(data_tx);

	return err;

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

	#ifdef __DEBUG_OTA_CAREL_LEV_1
	PRINTF_DEBUG("execute_download_devs_config err= %d \n",err);
	#endif
	if(CONN_OK == err){
		// model file has been saved, report it in nvm
		// save also corresponding cid and did
		// save also dev
		if( (C_SUCCESS == NVM__WriteU8Value(SET_DEVS_CONFIG_NVM, CONFIGURED)) &&
			(C_SUCCESS == NVM__WriteU32Value(MB_CID_NVM, myCborUpdate->cid)) &&
			(C_SUCCESS == NVM__WriteU32Value(MB_DID_NVM, myCborUpdate->did)) &&
			(C_SUCCESS == NVM__WriteU32Value(MB_DEV_NVM, myCborUpdate->dev)) ){
            #ifdef __DEBUG_OTA_CAREL_LEV_1
			PRINTF_DEBUG("MODEL FILE, CID, DID AND DEV SAVED\n");
            #endif
			P_COV_LN;
			err = C_SUCCESS;
		}else{
            #ifdef __DEBUG_OTA_CAREL_LEV_1
			PRINTF_DEBUG("CBOR MODEL FILE, CID, DID AND DEV NOT SAVED\n");
            #endif
			P_COV_LN;
			err = C_FAIL;
		}
	}else
		err = C_FAIL;

	err == C_SUCCESS ? CBOR_SendAsyncResponseDid(0, myCborUpdate->did, ASYNC_DEVCONF) : CBOR_SendAsyncResponseDid(1, myCborUpdate->did, ASYNC_DEVCONF);

	// reboot if everything went fine
	if ( err == C_SUCCESS )
	{
		GME__Reboot();
		P_COV_LN;
	}

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
	c_cborrequpdatecacert mySavedUpdate;

	strcpy(mySavedUpdate.usr, myCborUpdate->usr);
	strcpy(mySavedUpdate.pwd, myCborUpdate->pwd);
	strcpy(mySavedUpdate.uri, myCborUpdate->uri);
	mySavedUpdate.crc = myCborUpdate->crc;

	err = HttpsClient__UpdateCertificate(&mySavedUpdate);
	err == C_SUCCESS ? CBOR_SendAsyncResponse(0, ASYNC_CA) : CBOR_SendAsyncResponse(1, ASYNC_CA);

    #ifdef __DEBUG_OTA_CAREL_LEV_1
	if (err == C_SUCCESS)
		ESP_LOGI(TAG, "Certificate upgrade succeeded");
	else
		ESP_LOGE(TAG, "Certificate upgrade failed");

	PRINTF_DEBUG("execute_update_ca_cert err= %d \n",err);
    #endif
	// restart polling if needed
	uint8_t pe_status;
	NVM__ReadU8Value(PE_STATUS_NVM, &pe_status);
	if (pe_status == RUNNING)
	{
		PollEngine_StartEngine_CAREL();	// restart polling
		P_COV_LN;
	}

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

#ifdef __DEBUG_OTA_CAREL_LEV_1
      ESP_LOGI(TAG, "Starting OTA ...");
#endif

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


	if (C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
		cert_num = CERT_1;

	c_config.cert_num = cert_num;

    C_RES ret = https_ota(&c_config);

    Modbus_Enable();

    if (ret == C_SUCCESS) {
#ifdef __DEBUG_OTA_CAREL_LEV_1
    	ESP_LOGI(TAG, "Firmware Upgrades Succeeded");
#endif
    	//Send true to ota group to send the res and restart gme
    	free(url);
    	OTAGroup(true);
    	vTaskDelete(NULL);
    	P_COV_LN;
    } else {
#ifdef __DEBUG_OTA_CAREL_LEV_1
    	ESP_LOGE(TAG, "Firmware Upgrades Failed");
#endif
    	//Send false to ota group to send the res and continue working
    	free(url);
    	OTAGroup(false);
    	vTaskDelete(NULL);
    	P_COV_LN;
    }
}


void DEV_ota_range_task(void * pvParameter){

	c_cborrequpddevfw * dev_fw_config = (c_cborrequpddevfw*)pvParameter;

	C_RES err = C_FAIL;
	uint8_t cert_num;
	c_http_client_config_t c_config;
	http_client_handle_t client;

	static C_INT32 num_of_chunk = 0;
	static C_INT32 int_range_start  = 0;
	static C_INT32 int_range_stop = 0;
	static C_INT32 last_chunk_len = 0;
    char  myrange[50];
	
	C_BYTE client_init_retry;

    C_BYTE is_connected = 0;


    if(unlock_feature_control() != C_SUCCESS){
#ifdef __DEBUG_OTA_CAREL_LEV_1
    	PRINTF_DEBUG("cannot update device\n");
#endif
		OTADEVGroup(false);
		P_COV_LN;
		vTaskDelete(NULL);
    }


	C_UINT16 url_len = strlen(dev_fw_config->uri) + strlen(dev_fw_config->pwd) + strlen(dev_fw_config->usr);
	C_CHAR *url = malloc(url_len + 5);
	if (url == NULL) {
#ifdef __DEBUG_OTA_CAREL_LEV_1
		PRINTF_DEBUG("cannot alloc url\n");
#endif
		OTADEVGroup(false);
		P_COV_LN;
		vTaskDelete(NULL);
	}

	memset((void*)url, 0, url_len);

	sprintf(url,"%.*s%s:%s@%s", 8, dev_fw_config->uri, dev_fw_config->usr,dev_fw_config->pwd, dev_fw_config->uri+8);

	//to test http path use this
	//sprintf(url,"%s","http://test:password@bilato.ddns.net/49");

#ifdef __DEBUG_OTA_CAREL_LEV_1
	PRINTF_DEBUG("\r\n%s\r\n", url);
#endif

	c_config.url = url;
	c_config.username = dev_fw_config->usr;
	c_config.password = dev_fw_config->pwd;

	if(C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
	{
		cert_num = CERT_1;
		P_COV_LN;
	}


    #ifdef __DEBUG_OTA_CAREL_LEV_1
    PRINTF_DEBUG("%s OTA START **************************\r\n", TAG);
    #endif

	client = http_client_init_IS(&c_config, cert_num);

	if ((err = http_client_open_IS(client, 0)) != C_SUCCESS) {

		#ifdef __DEBUG_OTA_CAREL_LEV_1
		PRINTF_DEBUG("%s DEV_ota_range_task Failed to open HTTP connection", TAG);
		#endif
		free(url);
		OTADEVGroup(false);
		P_COV_LN;
		vTaskDelete(NULL);
	}

	C_UINT16 file_number = dev_fw_config->fid;
	C_UINT16 file_number_inc = 0;
	C_UINT16 file_number_inc_old = 0;
	C_INT32  data_read_len = 0;
	C_UINT32 sent_data_per_file = 0;
	C_UINT32 file_total_lenght = 0;
	C_UINT16 starting_reg = 0;
	C_BYTE   *upgrade_data_buf = NULL;
	C_INT16  chunk_size = DEV_OTA_BUF_SIZE;

	C_BOOL new_file = C_FALSE;

	upgrade_data_buf = (C_BYTE *)malloc(DEV_OTA_BUF_SIZE);
	
	if (upgrade_data_buf == NULL)
	{
		free(url); //TODO CPPCHECK questa viene chiamata 2 volte vedi riga 142
		PRINTF_DEBUG("cannot alloc upgrade_data_buf\n");
		OTADEVGroup(false);
		P_COV_LN;
		vTaskDelete(NULL);
	}
	
	memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(C_BYTE));

	C_INT32 content_length =  http_client_fetch_headers_IS(client);

	num_of_chunk = content_length / LENGTH_OF_CHUNK;

	if ((content_length % LENGTH_OF_CHUNK) != 0)
		num_of_chunk +=1;

	http_client_close_IS(client);
	http_client_cleanup_IS(client);

	int_range_start = 0;
	int_range_stop  = (LENGTH_OF_CHUNK - 1);

	// calculate last chunk dimension
    last_chunk_len = content_length - ((num_of_chunk-1)*LENGTH_OF_CHUNK);

    client_init_retry = 0;

    // start packet (to reset the device state mqachine)
	memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(C_BYTE));

	err = UpdateDevFirmware(upgrade_data_buf, 0, file_number, 0x0000);

	if(err != C_SUCCESS) {
        #ifdef __DEBUG_OTA_CAREL_LEV_1
        PRINTF_DEBUG("DEV_ota_range_task UpdateDevFirmware error - 2 ABORTED! \n");
        #endif
		free(url);
		free(upgrade_data_buf);
		OTADEVGroup(false);
		P_COV_LN;
		vTaskDelete(NULL);
	}

	Sys__Delay(3000);

	while (num_of_chunk > 0)
	{
		memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(C_BYTE));

		// loop header request
		if (!is_connected)
		{
			client_init_retry = 0;
		
            do {
   		       client = http_client_init_IS(&c_config, cert_num);

			   if (client != NULL)
			   {
			     is_connected = 1;
				 client_init_retry = 0;
			   }
			   else
			   {			   
				   client_init_retry++;
				   is_connected = 0;
				   
				   if (client_init_retry > MAX_CLIENT_INIT_RETRY)
				   {
                       #ifdef __DEBUG_OTA_CAREL_LEV_1
                       PRINTF_DEBUG("%s MAX_CLIENT_INIT_RETRY", TAG);
                       #endif
					   //ok no way Internet or server off line 					   
				       free(url);
				       free(upgrade_data_buf);
				       OTADEVGroup(false);
				       P_COV_LN;
				       vTaskDelete(NULL);					   
			       }		
				   Sys__Delay(100);				   
			   }
			   
			}while (is_connected==0);
						
		}


		sprintf(myrange, "bytes=%d%s%d\r\n",int_range_start,"-", int_range_stop);
		esp_http_client_set_method(client, HTTP_METHOD_GET);		
		esp_http_client_delete_header(client, "Range");        //delete previous range
		esp_http_client_set_header(client, "Range", myrange);  //set new range


        client_init_retry = 0;
        do
		{
			err = esp_http_client_perform(client);
		
  		    if (err != C_SUCCESS)
			{
				client_init_retry++;
				#ifdef __DEBUG_OTA_CAREL_LEV_1
			    PRINTF_DEBUG("%s DEV_ota_range_task esp_http_client_perform Failed to open HTTP connection", TAG);
		        #endif	
				
			    if (client_init_retry > MAX_CLIENT_INIT_RETRY)
				{
                  #ifdef __DEBUG_OTA_CAREL_LEV_1
                  PRINTF_DEBUG("%s MAX_CLIENT_INIT_RETRY", TAG);
                  #endif
				  //ok no way Internet or server off line 		          
			  
				  free(url);
				  free(upgrade_data_buf);
				  OTADEVGroup(false);
				  P_COV_LN;
				  vTaskDelete(NULL);					   
			    }			
				Sys__Delay(100);
			}
			
		}while(err != C_SUCCESS);

   	    C_INT32 cl =  http_client_fetch_headers_IS(client);

		if (num_of_chunk > 1)
	      data_read_len = esp_http_client_read(client, (char*)upgrade_data_buf, DEV_OTA_BUF_SIZE);
		else
		  data_read_len = esp_http_client_read(client, (char*)upgrade_data_buf, last_chunk_len);



	    if (data_read_len > 0)
	    {
			  #ifdef __DEBUG_OTA_CAREL_LEV_1
              PRINTF_DEBUG("\n%s data_read_len=%d of num_of_chunk=%d -> %d - %d \n",TAG, data_read_len, num_of_chunk, int_range_start, int_range_stop);

              C_UINT16 ciclo;

              if (data_read_len > 0)
              {
                 for (ciclo=0; ciclo < data_read_len; ciclo++)
                 {
	               PRINTF_DEBUG("%X ", upgrade_data_buf[ciclo]);
                 }
                 PRINTF_DEBUG("\r\n");
              }

              #endif

           //BILATO cancellare
           //err = C_SUCCESS;
           err = UpdateDevFirmware(upgrade_data_buf, data_read_len, (file_number+file_number_inc), starting_reg);	//, content_length - sent_data_per_file

			if (err != C_SUCCESS) {
				// modbus write error
                #ifdef __DEBUG_OTA_CAREL_LEV_1
                PRINTF_DEBUG("DEV_ota_range_task UpdateDevFirmware error - 1 ABORTED! \n");
                #endif

				free(url);
				free(upgrade_data_buf);
				OTADEVGroup(false);
				P_COV_LN;
				vTaskDelete(NULL);
			}

			sent_data_per_file = sent_data_per_file + data_read_len;
			file_total_lenght += data_read_len;

			if (sent_data_per_file >= MB_FILE_MAX_BYTES)
			{
		    	file_number_inc++;
		    	starting_reg = 0;
		    	sent_data_per_file = 0;
                #ifdef __CCL_DEBUG_MODE_1
                PRINTF_DEBUG("%s file_number_inc %d\n", TAG, file_number_inc);
                #endif
		    }
		    else
		    {
		    	starting_reg = sent_data_per_file/2;
		    }

            #ifdef __CCL_DEBUG_MODE_1
			PRINTF_DEBUG("%s file_number_inc %d  Written image length %d\n", TAG, file_number_inc, sent_data_per_file);
			#endif
	    }
		else
		{						
			// *** manage HTTP exception ***			
			http_client_close_IS(client);
			http_client_cleanup_IS(client);
			is_connected = 0;
			
			uart_flush_input_IS(modbusPort);
			uart_flush_IS(modbusPort);

			if (data_read_len < 0)
			{
				//we try to retry
				#ifdef __DEBUG_OTA_CAREL_LEV_1
				PRINTF_DEBUG("%s %s\r\n", TAG, "Error: SSL data read error");
				#endif			
			}

			if ((data_read_len == 0) && (num_of_chunk > 1))
			{
				//is a network error probably or server trouble 
                #ifdef __DEBUG_OTA_CAREL_LEV_1
                PRINTF_DEBUG("\n data_read_len==0   mum_of_chunk > 1) \n");
                #endif
			}
			else{
				//end of file SECONDO ME QUESTO E' un bug perchè se passiamo di qui non chiudiamo il file modbus
				
			    #ifdef __DEBUG_OTA_CAREL_LEV_1
                PRINTF_DEBUG("%s CLOSING update - Written file_total_lenght %d\n", TAG, file_total_lenght);
                #endif
				free(url);
				free(upgrade_data_buf);
				OTADEVGroup(false);
				P_COV_LN;
				vTaskDelete(NULL);
			}
		}


      if (is_connected ==1) 
	  {
		 //if connected I go forward if not I try to reconnect and download the same chunk
	     num_of_chunk--;

	     int_range_start += LENGTH_OF_CHUNK;

	     if(num_of_chunk > 1)
	       int_range_stop += LENGTH_OF_CHUNK;
	     else
		   int_range_stop += last_chunk_len; 	  // last chunk to download
	  }
	   
	}

	if (num_of_chunk == 0)
	{
		// all chunk sended...send a 0 file to end modbus trasmition
		http_client_close_IS(client);
		http_client_cleanup_IS(client);

		uart_flush_input_IS(modbusPort);
		uart_flush_IS(modbusPort);

		memset((void*)upgrade_data_buf, 0, DEV_OTA_BUF_SIZE * sizeof(C_BYTE));

		err = UpdateDevFirmware(upgrade_data_buf, 0, file_number, 0x0001); // 0  +file_number_inc

		if(err != C_SUCCESS) {
            #ifdef __DEBUG_OTA_CAREL_LEV_1
            PRINTF_DEBUG("DEV_ota_range_task UpdateDevFirmware error - 2 ABORTED! \n");
            #endif
			free(url);
			free(upgrade_data_buf);
			OTADEVGroup(false);
			P_COV_LN;
			vTaskDelete(NULL);
		}

		#ifdef __DEBUG_OTA_CAREL_LEV_1
		PRINTF_DEBUG("%s %s\r\n", TAG, "Connection closed,all data received");
		#endif
		free(url);
		free(upgrade_data_buf);
		OTADEVGroup(true);
		P_COV_LN;
		is_connected = 0;
		vTaskDelete(NULL);
	}


    #ifdef __DEBUG_OTA_CAREL_LEV_1
    PRINTF_DEBUG("%s %s\r\n", TAG, "Connection closed and exit from routine");
    #endif

        
	http_client_close_IS(client);
	http_client_cleanup_IS(client);
    

	free(url);
	free(upgrade_data_buf);

    #ifdef __DEBUG_OTA_CAREL_LEV_1
    PRINTF_DEBUG("%s End of download %d\n",TAG, num_of_chunk);
    #endif
}
