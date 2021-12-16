// Copyright 2017-2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <esp_https_ota.h>
#include <esp_log.h>

#include "gme_https_ota.h"
#include "CAREL_GLOBAL_DEF.h"
#include "sys_IS.h"
#include "ota_IS.h"
#include "modbus_IS.h"

#define IMAGE_HEADER_SIZE sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t) + 1
#define DEFAULT_OTA_BUF_SIZE IMAGE_HEADER_SIZE
static const char *TAG = "esp_https_ota";


#define OTA_CHUNK (1024)
#define MAX_CLIENT_INIT_RETRY (5)

static uint32_t file_size = 0;
static uint32_t dim_tot = 0;     // only for debug

typedef enum {
    ESP_HTTPS_OTA_INIT,
    ESP_HTTPS_OTA_BEGIN,
    ESP_HTTPS_OTA_IN_PROGRESS,
    ESP_HTTPS_OTA_SUCCESS,
} esp_https_ota_state;

struct esp_https_ota_handle {
    esp_ota_handle_t update_handle;
    const esp_partition_t *update_partition;
    esp_http_client_handle_t http_client;
    char *ota_upgrade_buf;
    size_t ota_upgrade_buf_size;
    int binary_file_len;
    esp_https_ota_state state;
};

typedef struct esp_https_ota_handle esp_https_ota_t;

static bool process_again(int status_code)
{
    switch (status_code) {
        case HttpStatus_MovedPermanently:
        case HttpStatus_Found:
        case HttpStatus_Unauthorized:
            return true;
        default:
            return false;
    }
    return false;
}

static esp_err_t CAREL_http_handle_response_code(esp_http_client_handle_t http_client, int status_code)
{
    esp_err_t err;
    if (status_code == HttpStatus_MovedPermanently || status_code == HttpStatus_Found) {
        err = esp_http_client_set_redirection(http_client);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "URL redirection Failed");
            return err;
        }
    } else if (status_code == HttpStatus_Unauthorized) {
        esp_http_client_add_auth(http_client);
    }
    
    char upgrade_data_buf[DEFAULT_OTA_BUF_SIZE];
    if (process_again(status_code)) {
        while (1) {
            int data_read = esp_http_client_read(http_client, upgrade_data_buf, DEFAULT_OTA_BUF_SIZE);
            if (data_read < 0) {
                ESP_LOGE(TAG, "Error: SSL data read error");
                return ESP_FAIL;
            } else if (data_read == 0) {
                return ESP_OK;
            }
        }
    }
    return ESP_OK;
}

static esp_err_t CAREL_http_connect(esp_http_client_handle_t http_client)
{
    esp_err_t err = ESP_FAIL;
    int status_code;
    do {
        err = esp_http_client_open(http_client, 0);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
            return err;
        }

        file_size = esp_http_client_fetch_headers(http_client);
        dim_tot = file_size;


        status_code = esp_http_client_get_status_code(http_client);
        if (CAREL_http_handle_response_code(http_client, status_code) != ESP_OK) {
            return ESP_FAIL;
        }
    } while (process_again(status_code));
    return err;
}

static void CAREL_http_cleanup(esp_http_client_handle_t client)
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

static esp_err_t CAREL_ota_write(esp_https_ota_t *https_ota_handle, const void *buffer, size_t buf_len)
{
    if (buffer == NULL || https_ota_handle == NULL) {
        return ESP_FAIL;
    }
    esp_err_t err = esp_ota_write(https_ota_handle->update_handle, buffer, buf_len);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error: esp_ota_write failed! err=0x%d", err);
    } else {
        https_ota_handle->binary_file_len += buf_len;
        ESP_LOGD(TAG, "Written image length %d", https_ota_handle->binary_file_len);
        err = ESP_ERR_HTTPS_OTA_IN_PROGRESS;
    }
    return err;
}


/*
 *  start the OTA procedure
 *
 * */
esp_err_t gme_https_ota_begin(CAREL_https_ota_config_t *ota_config, esp_https_ota_handle_t *handle)
{
    esp_err_t err;

    if (handle == NULL || ota_config == NULL || ota_config->http_config == NULL) {
        ESP_LOGE(TAG, "esp_https_ota_begin: Invalid argument");
        if (handle) {
            *handle = NULL;
        }
        return ESP_ERR_INVALID_ARG;
    }

#if !CONFIG_OTA_ALLOW_HTTP
    if (!ota_config->http_config->cert_pem) {
        ESP_LOGE(TAG, "Server certificate not found in esp_http_client config");
        *handle = NULL;
        return ESP_ERR_INVALID_ARG;
    }
#endif

    esp_https_ota_t *https_ota_handle = calloc(1, sizeof(esp_https_ota_t));
    if (!https_ota_handle) {
        ESP_LOGE(TAG, "Couldn't allocate memory to upgrade data buffer");
        *handle = NULL;
        return ESP_ERR_NO_MEM;
    }
    
    /* Initiate HTTP Connection */
    https_ota_handle->http_client = esp_http_client_init(ota_config->http_config);
    if (https_ota_handle->http_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialise HTTP connection");
        err = ESP_FAIL;
        goto failure;
    }

    err = CAREL_http_connect(https_ota_handle->http_client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to establish HTTP connection");
        goto http_cleanup;
    }

    //
    // close the connection
    //
    esp_http_client_close(https_ota_handle->http_client);
    esp_http_client_cleanup(https_ota_handle->http_client);

    https_ota_handle->update_partition = NULL;
    ESP_LOGI(TAG, "Starting OTA...");
    https_ota_handle->update_partition = esp_ota_get_next_update_partition(NULL);
    if (https_ota_handle->update_partition == NULL) {
        ESP_LOGE(TAG, "Passive OTA partition not found");
        err = ESP_FAIL;
        goto http_cleanup;
    }
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
        https_ota_handle->update_partition->subtype, https_ota_handle->update_partition->address);

    const int alloc_size = (ota_config->http_config->buffer_size > DEFAULT_OTA_BUF_SIZE) ?
                            ota_config->http_config->buffer_size : DEFAULT_OTA_BUF_SIZE;
    https_ota_handle->ota_upgrade_buf = (char *)malloc(alloc_size);
    if (!https_ota_handle->ota_upgrade_buf) {
        ESP_LOGE(TAG, "Couldn't allocate memory to upgrade data buffer");
        err = ESP_ERR_NO_MEM;
        goto http_cleanup;
    }
    https_ota_handle->ota_upgrade_buf_size = alloc_size;

    https_ota_handle->binary_file_len = 0;
    *handle = (esp_https_ota_handle_t)https_ota_handle;
    https_ota_handle->state = ESP_HTTPS_OTA_BEGIN;
    return ESP_OK;

http_cleanup:
    CAREL_http_cleanup(https_ota_handle->http_client);
failure:
    free(https_ota_handle);
    *handle = NULL;
    return err;
}




esp_err_t gme_https_ota_perform(esp_https_ota_handle_t https_ota_handle, CAREL_https_ota_config_t *ota_config)
{
	char  myrange[50];
	static C_INT32 int_range_start  = 0;
	static C_INT32 int_range_stop = 0;
	static C_INT32 chunk_len = 0;

	static C_BYTE client_init_retry;
	static C_BYTE is_connected = 0;

#ifdef __DEBUG_OTA_GME
	static C_INT32 total_dbg = 0;
#endif

    esp_https_ota_t *handle = (esp_https_ota_t *)https_ota_handle;
    if (handle == NULL) {
        ESP_LOGE(TAG, "esp_https_ota_perform: Invalid argument");
        return ESP_ERR_INVALID_ARG;
    }
    if (handle->state < ESP_HTTPS_OTA_BEGIN) {
        ESP_LOGE(TAG, "esp_https_ota_perform: Invalid state");
        return ESP_FAIL;
    }

    esp_err_t err;
    int data_read;
    switch (handle->state) {
        case ESP_HTTPS_OTA_BEGIN:
        {
            err = esp_ota_begin(handle->update_partition, OTA_SIZE_UNKNOWN, &handle->update_handle);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
                return err;
            }
            handle->state = ESP_HTTPS_OTA_IN_PROGRESS;

            int_range_start = 0;
            int_range_stop  = (OTA_CHUNK - 1);
            chunk_len = OTA_CHUNK;

			#ifdef __DEBUG_OTA_GME
            total_dbg = file_size;
			#endif

            return ESP_ERR_HTTPS_OTA_IN_PROGRESS;
        }

        case ESP_HTTPS_OTA_IN_PROGRESS:
        {
    		// loop header request
    		if (!is_connected)
    		{
    			client_init_retry = 0;

                do {
                	handle->http_client = esp_http_client_init(ota_config->http_config);

    			   if (handle->http_client != NULL)
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
    					   Modbus_Enable();

    					   //ok no way Internet or server off line
    					   OTAGroup(false);
    					   vTaskDelete(NULL);
						   #ifdef __DEBUG_OTA_GME
						   printf("ok no way Internet or server off line \r\n");
						   #endif

    					   handle->state = ESP_HTTPS_OTA_IN_PROGRESS;
    					   return ESP_FAIL;
    			       }
    				   Sys__Delay(100);
    			   }
    			}while (is_connected==0);
    		}

    		sprintf(myrange, "bytes=%d%s%d\r\n",int_range_start,"-", int_range_stop);
    		esp_http_client_set_method(handle->http_client, HTTP_METHOD_GET);
    		esp_http_client_delete_header(handle->http_client, "Range");        //delete previous range
    		esp_http_client_set_header(handle->http_client, "Range", myrange);  //set new range

    		do
			{
				err = esp_http_client_perform(handle->http_client);

				if (err != C_SUCCESS)
				{
					client_init_retry++;

					if (client_init_retry > MAX_CLIENT_INIT_RETRY)
					{
					  Modbus_Enable();

					  //ok no way Internet or server off line
					  OTAGroup(false);
					  vTaskDelete(NULL);

					  #ifdef __DEBUG_OTA_GME
					  printf("ok no way Internet or server off line \r\n");
					  #endif

					  handle->state = ESP_HTTPS_OTA_IN_PROGRESS;
					  return ESP_FAIL;
					}
					Sys__Delay(100);
				}
    		}while(err != C_SUCCESS);

    		// request the chunk of data (1024 bytes or less if is the last packet)
    		esp_http_client_fetch_headers(handle->http_client);
		    data_read = esp_http_client_read(handle->http_client, handle->ota_upgrade_buf, chunk_len);

    		file_size -= data_read;

#ifdef __DEBUG_OTA_GME
    		printf("lunghezza letta %d totale %d / %d \r\n", data_read, file_size, total_dbg);
#endif
    		// check if we have a lot of chunk or we are at the end
    		chunk_len = (((file_size) >= OTA_CHUNK)  ?  OTA_CHUNK :   file_size);
    		int_range_start += OTA_CHUNK;
    		int_range_stop += chunk_len;

    		dim_tot -= data_read;

            if (dim_tot == 0) {
            	CAREL_ota_write(handle, (const void *)handle->ota_upgrade_buf, data_read);
                ESP_LOGI(TAG, "Connection closed, all data received");
            } else if (data_read < 0) {		
                ESP_LOGE(TAG, "Error: SSL data read error");
                return ESP_FAIL;
            } else if (data_read > 0) {
                return CAREL_ota_write(handle, (const void *)handle->ota_upgrade_buf, data_read);
            }
	        else if(dim_tot > 0)
            {
			   ESP_LOGI(TAG,"the file is not finish, connection error!!!\r\n");
			   break;
            }	
			
            handle->state = ESP_HTTPS_OTA_SUCCESS;
            break;
        }
        default:
            ESP_LOGE(TAG, "Invalid ESP HTTPS OTA State");
            return ESP_FAIL;
            break;
    }
    return ESP_OK;
}

esp_err_t gme_https_ota_finish(esp_https_ota_handle_t https_ota_handle)
{
    esp_https_ota_t *handle = (esp_https_ota_t *)https_ota_handle;
    if (handle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (handle->state < ESP_HTTPS_OTA_BEGIN) {
        return ESP_FAIL;
    }

    esp_err_t err = ESP_OK;
    switch (handle->state) {
        case ESP_HTTPS_OTA_SUCCESS:
        case ESP_HTTPS_OTA_IN_PROGRESS:
            err = esp_ota_end(handle->update_handle);
            /* falls through */
        case ESP_HTTPS_OTA_BEGIN:
            if (handle->ota_upgrade_buf) {
                free(handle->ota_upgrade_buf);
            }
            if (handle->http_client) {
                CAREL_http_cleanup(handle->http_client);
            }
            break;
        default:
            ESP_LOGE(TAG, "Invalid ESP HTTPS OTA State");
            break;
    }

    if ((err == ESP_OK) && (handle->state == ESP_HTTPS_OTA_SUCCESS)) {
        esp_err_t err = esp_ota_set_boot_partition(handle->update_partition);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_ota_set_boot_partition failed! err=0x%d", err);
        }
    }
    free(handle);
    return err;
}

int esp_https_ota_get_image_len_read(esp_https_ota_handle_t https_ota_handle)
{
    esp_https_ota_t *handle = (esp_https_ota_t *)https_ota_handle;
    if (handle == NULL) {
        return -1;
    }
    if (handle->state < ESP_HTTPS_OTA_IN_PROGRESS) {
        return -1;
    }
    return handle->binary_file_len;
}

esp_err_t gme_https_ota(const esp_http_client_config_t *config)
{
    if (!config) {
        ESP_LOGE(TAG, "esp_http_client config not found");
        return ESP_ERR_INVALID_ARG;
    }    

    CAREL_https_ota_config_t ota_config = {
        .http_config = config,
    };

    esp_https_ota_handle_t https_ota_handle = NULL;
    esp_err_t err = gme_https_ota_begin(&ota_config, &https_ota_handle);
    if (https_ota_handle == NULL) {
        return ESP_FAIL;
    }

    while (1) {				
        err = gme_https_ota_perform(https_ota_handle, &ota_config);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
    }

    esp_err_t ota_finish_err = gme_https_ota_finish(https_ota_handle);
    if (err != ESP_OK) {
        /* If there was an error in esp_https_ota_perform(),
           then it is given more precedence than error in esp_https_ota_finish()
         */
        return err;
    } else if (ota_finish_err != ESP_OK) {
        return ota_finish_err;
    }
    return ESP_OK;
}
