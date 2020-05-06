/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "carel_configuration.h"

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"


#include "spiffs_manager.h"



static const char *TAG = "SPIFFS_MNG";


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




esp_err_t spiffs_init(void)
{
    esp_err_t ret_val;
    
    ret_val = ESP_FAIL;
    
    ESP_LOGI(TAG, "Initializing SPIFFS");
    
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = MAX_NUM_OF_FILES,
      .format_if_mount_failed = true
    };
    
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) 
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
            ret_val = ESP_FAIL;
        } 
        else if (ret == ESP_ERR_NOT_FOUND) 
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
            ret_val = ESP_ERR_NOT_FOUND;
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
            ret_val = ESP_FAIL;
        }
        return ret_val;
    }
    
    size_t total = 0, used = 0;
    
    ret = esp_spiffs_info(NULL, &total, &used);
    
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        ret_val = ESP_FAIL;
        return ret_val;
    } 
    else 
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    
    int fsize;
    fsize = (int)filesize(CERT_1_NAME);
    printf("%s %d \r\n ", CERT_1_NAME, fsize);

    fsize = (int)filesize(CERT_2_NAME);
    printf("%s %d \r\n ", CERT_2_NAME, fsize);

    fsize = (int)filesize(MODEL_NAME);
    printf("%s %d \r\n ", MODEL_NAME, fsize);


    
#if 0
    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) 
    {
        // Delete it if it exists
        unlink("/spiffs/foo.txt");
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) 
    {
        ESP_LOGE(TAG, "Rename failed");
        ret_val = ESP_FAIL;
        return ret_val;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) 
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        ret_val = ESP_FAIL;
        return ret_val;
    }
    
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");

#endif

    ret_val = ESP_OK;
    return ret_val;
}
