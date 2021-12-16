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
#include "gme_config.h"
#include "mobile.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_log.h"
static const char *TAG = "spiffs";
#endif

/* Functions Implementation --------------------------------------------------*/

#ifdef INCLUDE_PLATFORM_DEPENDENT

/**
 * @brief init_spiffs
 * initialize the SPI Flash File System remember max 5 files opened at the same time
 *
 * @param  none
 * @return C_SUCCESS / C_FAIL
 */
esp_err_t init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,   //the maximum number of files that could be opened at the same time
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
        P_COV_LN;
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        P_COV_LN;
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    P_COV_LN;
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

/**
 * @brief Set_Gateway_ID
 *        Sets the MAC address of the WiFi device at the same value of
 *        the default
 *
 * @param  none
 * @return C_SUCCESS
 */
C_RES Set_Gateway_ID(void)
{
  /* this function set the MAC address of the WiFi appliance
   * in this implementation we leave the original one coming
   * from the chip
   * */

    #ifdef INCLUDE_PLATFORM_DEPENDENT
		esp_err_t retval;
		uint8_t s_id_tmp[6];
		retval = esp_read_mac(&s_id_tmp[0], ESP_MAC_WIFI_STA);
		if (retval != ESP_OK) return C_FAIL;
		retval = esp_base_mac_addr_set(&s_id_tmp[0]);
		if (retval != ESP_OK) return C_FAIL;
    #endif

	return C_SUCCESS;
}



/**
 * @brief Get_Gateway_ID
 *        Gets MAC address of the WiFi device or IMEI of the 2G module
 *
 * @param C_BYTE* contaning the MAC or the IMEI mqtt portbroker
 * @return C_SUCCESS
 */
C_RES Get_Gateway_ID(C_SBYTE *s_id)
{ /* TO BE implemented */
	C_RES rv;

	rv = C_FAIL;

	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU)) {
		/* this function returns the MAC address of the WiFi appliance */

		#ifdef INCLUDE_PLATFORM_DEPENDENT
		uint8_t s_id_tmp[6];
		esp_err_t retval ;

		retval = esp_read_mac(&s_id_tmp[0], ESP_MAC_WIFI_STA);

		if (retval == ESP_OK) rv = C_SUCCESS;

		sprintf(s_id,"%02X%02X%02X%02X%02X%02X",
				s_id_tmp[0],
				s_id_tmp[1],
				s_id_tmp[2],
				s_id_tmp[3],
				s_id_tmp[4],
				s_id_tmp[5]
			   );
		#endif
		P_COV_LN;
	}
	else if (PLATFORM(PLATFORM_DETECTED_2G)) {
		/* this function returns the IMEI of the GSM module*/
		strcpy(s_id, Mobile__GetImeiCode());
		rv = C_SUCCESS;
		P_COV_LN;
	}


	return rv;
}
