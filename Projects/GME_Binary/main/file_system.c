/*
 * filesystem.c
 *
 *  Created on: Jun 19, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */


#include "file_system.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"

static const char *TAG = "spiffs";



long filesize(const char *fname)
{
  long sz = -1;

  FILE* fp = fopen(fname, "r");
  if (fp != NULL)
  {

    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
  }

  return sz;
}


esp_err_t init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,   // This decides the maximum number of files that can be created on the storage
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    


    return ESP_OK;
}





void FS_DisplayFiles(void){
    int fsize;
    //char read[1024] = {0};
    //const char* filepath = "/spiffs";
    //FILE *fd = NULL;
    struct stat file_stat;

    if (stat(CERT_1_NAME, &file_stat) == -1){
    		printf("%s is not found\n",CERT_1_NAME);
		}else{
			fsize = (int)filesize(CERT_1_NAME);
			printf("%s %d \r\n ", CERT_1_NAME, fsize);
			/*fd = fopen(CERT_1_NAME, "r");
			if(!fd){
				printf("Opening %s is failed\n",CERT_1_NAME);
			}else{
				fread(read, fsize, 1 ,fd);
				printf("%s :\n%s",CERT_1_NAME,read);
			}
				fclose(fd);*/

		}

    if (stat(CERT_1_NAME, &file_stat) == -1){
        	printf("%s is not found\n",CERT_1_NAME);
        }else{
        	//fd = fopen(CERT_1_NAME, "r");
        	fsize = (int)filesize(CERT_2_NAME);
        	printf("%s %d \r\n ", CERT_2_NAME, fsize);
        }

    if (stat(MODEL_NAME, &file_stat) == -1){
        	printf("%s is not found\n",MODEL_NAME);
        }else{
        	//fd = fopen(CERT_1_NAME, "r");
        	fsize = (int)filesize(MODEL_NAME);
        	printf("%s %d \r\n ", MODEL_NAME, fsize);
        }

}

void* FS_ReadFile(const char* filename){
		FILE *input_file_ptr;
		size_t sz_read;
		long size;
		void* data = NULL;

		printf("Reading from SPIFFS : %s\n",filename);

		input_file_ptr = fopen(filename, "rb");

		if (input_file_ptr == NULL)
		{
			printf("Unable to open file! \n");
			return NULL;
		}

		printf("Read File ok \n");

		//calculation model dimension

		//Set file position pointer at the end of the file
		fseek(input_file_ptr, 0L, SEEK_END);
		//tell me actual file position number (the number is in bytes)
		size = ftell(input_file_ptr);
		//point the file position at the beginning of the file
		rewind(input_file_ptr);

		printf("Size model Ok, %ld \n",size);

		data = memmgr_alloc(size);

		if(NULL != data){
			printf("File allocation OK\n");
		}else{
			printf("Problem in file allocation\n");
		}

		memset(data,0,size);

		sz_read = fread(data, sizeof(uint8_t), size, input_file_ptr);  // double


		if(sz_read != size)
			printf("Read ERROR!!!! \n");

		// close streaming
		fclose(input_file_ptr);

		return data;
}
