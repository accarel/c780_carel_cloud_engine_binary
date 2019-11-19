/**
 * @file File_System_IS.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         file system.
 *         Note  : 
 *         we assume that the stdio.h function are supported 
 *         ie. fopen/fclose/fread/fwrite/fseek 
 */






/* Includes ------------------------------------------------------------------*/
#include "File_System_IS.h"   

#ifdef INCLUDE_PLATFORM_DEPENDENT
//#include "file_system.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_log.h"
static const char *TAG = "spiffs";
#endif

/* Functions Implementation --------------------------------------------------*/

#ifdef INCLUDE_PLATFORM_DEPENDENT
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
#endif

/**
 * @brief File_System_Init
 *        Initialize the file system
 *
 * @param none
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Init(void)
{  /* TO BE implemented */

	C_RES err = C_FAIL;
	/* call the routine to initialize the file system */
	#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = init_spiffs();
	#endif
	return err;
}
