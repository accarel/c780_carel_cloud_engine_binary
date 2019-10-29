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


esp_err_t FS_CheckFiles(void){
    int fsize;
    struct stat file_stat;
    esp_err_t err1, err2;

    if (stat(CERT1_SPIFFS, &file_stat) == -1){
    		printf("Cert1 in %s is not found\n",CERT1_SPIFFS);
    		err1 = ESP_FAIL;
		}else{
			fsize = (int)filesize(CERT1_SPIFFS);
			printf("Cert1 found in %s, size: %d \r\n ", CERT1_SPIFFS, fsize);
			err1 = ESP_OK;
		}

    if (stat(CERT1_SPIFFS, &file_stat) == -1){
        	printf("Cert2 in %s is not found\n",CERT1_SPIFFS);
        }else{
        	err2 = ESP_FAIL;
        	fsize = (int)filesize(CERT2_SPIFFS);
        	printf("Cert2 found in %s, size: %d \r\n", CERT2_SPIFFS, fsize);
        	err2 = ESP_OK;
        }

    if (stat(MODEL_FILE, &file_stat) == -1){
        	printf("Model File in %s is not found\n",MODEL_FILE);
        }else{
        	fsize = (int)filesize(MODEL_FILE);
        	printf("Model File found in %s, size: %d \r\n", MODEL_FILE, fsize);
        }

    if(ESP_OK == err1 && ESP_OK == err2){
    	return ESP_OK;
    }else{
    	return ESP_FAIL;
    }
}




void FS_DisplayFiles(void){
    int fsize;

    struct stat file_stat;

    if (stat(CERT1_SPIFFS, &file_stat) == -1){
		printf("%s is not found\n",CERT1_SPIFFS);
	}else{
		fsize = (int)filesize(CERT1_SPIFFS);
		printf("%s %d \r\n ", CERT1_SPIFFS, fsize);
	}

    if (stat(CERT1_SPIFFS, &file_stat) == -1){
		printf("%s is not found\n",CERT1_SPIFFS);
	}else{
		fsize = (int)filesize(CERT2_SPIFFS);
		printf("%s %d \r\n ", CERT2_SPIFFS, fsize);
	}

    if (stat(MODEL_FILE, &file_stat) == -1){
		printf("%s is not found\n",MODEL_FILE);
	}else{
		fsize = (int)filesize(MODEL_FILE);
		printf("%s %d \r\n ", MODEL_FILE, fsize);
	}

    if (stat(LOGIN_HTML, &file_stat) == -1){
		printf("%s is not found\n",LOGIN_HTML);
	}else{
		fsize = (int)filesize(LOGIN_HTML);
		printf("%s %d \r\n ", LOGIN_HTML, fsize);
	}

    if (stat(CHANGE_CRED_HTML, &file_stat) == -1){
		printf("%s is not found\n",CHANGE_CRED_HTML);
	}else{
		fsize = (int)filesize(CHANGE_CRED_HTML);
		printf("%s %d \r\n ", CHANGE_CRED_HTML, fsize);
	}

    if (stat(CONFIG_HTML, &file_stat) == -1){
		printf("%s is not found\n",CONFIG_HTML);
	}else{
		fsize = (int)filesize(CONFIG_HTML);
		printf("%s %d \r\n ", CONFIG_HTML, fsize);
	}

    if (stat(STYLE_CSS, &file_stat) == -1){
		printf("%s is not found\n",STYLE_CSS);
	}else{
		fsize = (int)filesize(STYLE_CSS);
		printf("%s %d \r\n ", STYLE_CSS, fsize);
	}

    if (stat(FAV_ICON, &file_stat) == -1){
		printf("%s is not found\n",FAV_ICON);
	}else{
		fsize = (int)filesize(FAV_ICON);
		printf("%s %d \r\n ", FAV_ICON, fsize);
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

		printf("File size: %ld bytes\n",size);

		data = malloc(size);

		printf("malloc OK \n");

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


char* FS_ReadCertificate(const char* filename){
		FILE *input_file_ptr;
		size_t sz_read;
		long size;
		char* data = NULL;

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

		data = (char*)malloc(size + 1);

		if(NULL != data){
			printf("File allocation OK\n");
		}else{
			printf("Problem in file allocation\n");
		}

		memset((void*)data,0,size);

		sz_read = fread((void*)data, sizeof(uint8_t), size, input_file_ptr);  // double


		if(sz_read != size)
			printf("Read ERROR!!!! \n");

		// close streaming
		fclose(input_file_ptr);

		data[size+1] = '\0';

		return data;
}




void* FS_ReadFile2(const char* filename, uint8_t* cert_ptr){
		FILE *input_file_ptr;
		size_t sz_read;
		long size;
		//void* data = NULL;

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

		sz_read = fread(cert_ptr, sizeof(uint8_t), size, input_file_ptr);  // double
		cert_ptr[size+1]=0;
		if(sz_read != size)
			printf("Read ERROR!!!! \n");

		// close streaming
		fclose(input_file_ptr);

		return cert_ptr;
}



esp_err_t FS_SaveFile(const char* file_to_save, size_t file_size, const char* filename){

		 FILE *file;

		file = fopen(filename, "w");
		if(NULL == file){
			printf("%s - File not found\n",filename);
			return ESP_FAIL;
		}else{
			printf("%s - File found\n",filename);
			fwrite(file_to_save , 1 , file_size , file );
			fclose(file);
			return ESP_OK;
		}

}



void* FS_CheckFile(const char* filename){
		FILE *input_file_ptr;
		size_t sz_read;
		long size;
		void* data;

		printf("Start check GME MODEL: %s\n",filename);

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

		printf("Size model Ok \n");

		data = (void *)malloc(size);

		memset(data,0,size);

		sz_read = fread(data, sizeof(uint8_t), size, input_file_ptr);  // double


		if(sz_read != size)
			printf("Read ERROR!!!! \n");

		// close streaming
		fclose(input_file_ptr);

		return data;
}


