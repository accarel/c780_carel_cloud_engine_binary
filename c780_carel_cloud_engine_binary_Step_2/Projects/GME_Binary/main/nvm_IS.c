#include "data_types_CAREL.h"
#include "nvm_IS.h"
#include "gme_types.h"

#include "unity.h"
#include "esp_flash_encrypt.h"
#include "esp_partition.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include <errno.h>

#include "mbedtls/aes.h"
static nvs_handle my_handle;


/**
 * @brief NVM_Init
 *        call a "nvs_flash_init" function,
 *        for more details see esp-idf manual.
 *
 * @param  none
 *
 * @return C_RES
 */
C_RES NVM_Init(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT

  #ifdef CONFIG_NVS_ENCRYPTION
	nvs_sec_cfg_t cfg;

	//check that the partition NVS and KEY NVS exists
	const esp_partition_t* key_part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS, NULL);
	assert(key_part && "partition table must have an NVS Key partition");

	const esp_partition_t* nvs_partition = esp_partition_find_first(
	            ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
	assert(nvs_partition && "partition table must have an NVS partition");

	// check the initialization of NVS KEY partition
	esp_err_t err = nvs_flash_read_security_cfg(key_part, &cfg);

	if(err == ESP_ERR_NVS_KEYS_NOT_INITIALIZED)
	{
		uint8_t value[4096] = {[0 ... 4095] = 0xff};
		TEST_ESP_OK(esp_partition_write(key_part, 0, value, sizeof(value)));

		TEST_ESP_ERR(nvs_flash_read_security_cfg(key_part, &cfg), ESP_ERR_NVS_KEYS_NOT_INITIALIZED);

		PRINTF_DEBUG_NVM("Generate the nvs key for Encrypt.");
		TEST_ESP_OK(nvs_flash_generate_keys(key_part, &cfg));

	}
	else
	{
		PRINTF_DEBUG_NVM("Already generated nvs key for Encrypt.");
		// Second time key_partition exists already
		ESP_ERROR_CHECK(err);
	}


	return nvs_flash_secure_init(&cfg);

  #else
        return nvs_flash_init();   // old initialization without encryption
  #endif
 #else
        return C_FAIL;				// implement your non volatile memory init
 #endif

}

/**
 * @brief NVM__Open
 *        call a "nvs_open" function,
 *        for more details see esp-idf manual.
 *
 * @param  none
 *
 * @return C_RES
 */
C_RES NVM__Open(void)
{
	PRINTF_DEBUG_NVM("Opening Non-Volatile Storage (NVS) handle... ");
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		PRINTF_DEBUG_NVM("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {
		PRINTF_DEBUG_NVM("Done\n");
    }
#endif
	return err;
}

/**
 * @brief NVM__Close
 *        call a "nvs_close" function,
 *        for more details see esp-idf manual.
 *
 * @param  none
 *
 * @return C_RES
 */
void NVM__Close(void){
	nvs_close(my_handle);
}

/**
 * @brief NVM__SetU8
 *        call a "nvs_set_u8(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  C_BYTE val
 *
 * @return C_RES
 */
C_RES NVM__SetU8(const C_CHAR* var, C_BYTE val){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	// Write

	if (ESP_OK == nvs_set_u8(my_handle, var, val))
	{
	  err = nvs_commit(my_handle);
	}
#endif
	return err;
}

/**
 * @brief NVM__SetU32
 *        call a "nvs_set_u32(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  C_UINT32 val
 *
 * @return C_RES
 */
C_RES NVM__SetU32(const C_CHAR* var, C_UINT32 val){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	// Write
	if (ESP_OK == nvs_set_u32(my_handle, var, val))
	{
	err = nvs_commit(my_handle);
	}
#endif
	return err;
}

/**
 * @brief NVM__SetString
 *        call a "nvs_set_str(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  char* str
 *
 * @return C_RES
 */
C_RES NVM__SetString(const C_CHAR* var, char* str){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	// Write
	if (ESP_OK == nvs_set_str(my_handle, var, str))
	{
	   err = nvs_commit(my_handle);
	}
#endif
	return err;
}

/**
 * @brief NVM__SetBlob
 *        call a "nvs_set_blob(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  void* vec
 * @param  size_t len
 *
 * @return C_RES
 */
C_RES NVM__SetBlob(const C_CHAR* var, void* vec, size_t len){
	C_RES err = C_FAIL;

	PRINTF_DEBUG_NVM("Updating %s in NVS ... ",var);

#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_set_blob(my_handle, var, vec, len);

	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done");
	PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);

	if (ESP_OK == err){
	  PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
	  err = nvs_commit(my_handle);
	  PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");
	}
#endif

	return err;
}

/**
 * @brief NVM__GetU8
 *        call a "nvs_get_u8(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  C_BYTE* val
 *
 * @return C_RES
 */
C_RES NVM__GetU8(const C_CHAR* var, C_BYTE* val){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_get_u8(my_handle, var, val);
	switch (err) {
	case ESP_OK:
		PRINTF_DEBUG_NVM("Done: %s = %d\n", var, *val);
		break;
	case ESP_ERR_NVS_NOT_FOUND:
		PRINTF_DEBUG_NVM("The value is not initialized yet!\n");
		break;
	default:
		PRINTF_DEBUG_NVM("Error (%s) reading!\n", esp_err_to_name(err));
	}
#endif
	return err;
}

/**
 * @brief NVM__GetU32
 *        call a "nvs_get_u32(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  C_UINT32* val
 *
 * @return C_RES
 */
C_RES NVM__GetU32(const C_CHAR* var, C_UINT32* val){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_get_u32(my_handle, var, val);
	switch (err) {
		case ESP_OK:
			PRINTF_DEBUG_NVM("Done: %s = %d\n", var, *val);
			break;
		case ESP_ERR_NVS_NOT_FOUND:
			PRINTF_DEBUG_NVM("The value is not initialized yet!\n");
			break;
		default:
			PRINTF_DEBUG_NVM("Error (%s) reading!\n", esp_err_to_name(err));
	}
#endif
	return err;
}

/**
 * @brief NVM__GetString
 *        call a "nvs_get_str(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  char* str
 * @param  size_t* len
 *
 * @return C_RES
 */
C_RES NVM__GetString(const C_CHAR* var, char* str, size_t* len){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT

	nvs_get_str(my_handle, var, NULL, len); //return value tested with len
	if (*len == 0){
		PRINTF_DEBUG_NVM("Nothing to allocate\n");
		return C_FAIL;
	}

	char *temp = malloc(*len);
	if (temp == NULL){
		PRINTF_DEBUG_NVM("No more space to allocate\n");
		return C_FAIL;
	}

	err = nvs_get_str(my_handle, var, temp, len);
	if (err == ESP_OK) {
		PRINTF_DEBUG_NVM("Reading %s, is %s  ....  length = %d \n",var ,temp, *len);
		strcpy(str,temp);
	}

	free(temp);
#endif
	return err;
}

/**
 * @brief NVM__GetBlob
 *        call a "nvs_get_blob(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 * @param  void* vec
 * @param  size_t* len
 *
 * @return C_RES
 */
C_RES NVM__GetBlob(const C_CHAR* var, void* vec, size_t* len){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT

	nvs_get_blob(my_handle, var, NULL, len); //return value tested with len
	if (*len == 0){
		PRINTF_DEBUG_NVM("Nothing to allocate\n");
		return C_FAIL;
	}

	char* temp = malloc(*len);
	if (temp == NULL){
		PRINTF_DEBUG_NVM("No more space to allocate");
		return C_FAIL;
	}

	err = nvs_get_blob(my_handle, var, temp, len);
	if (err == C_SUCCESS) {
		PRINTF_DEBUG_NVM("Reading %s, is  ....  length = %d \n",var , (int)*len);
		memcpy(vec,(void*)temp,*len);
	}
	free(temp);
#endif
	return err;

}

/**
 * @brief NVM__EraseK
 *        call a "nvs_erase_key(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  const C_CHAR* var
 *
 * @return C_RES
 */
C_RES NVM__EraseK(const C_CHAR* var){
	C_RES err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_erase_key(my_handle, var);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

	if (ESP_OK != err) return err;
	//TODO CPPCHECK prima non era gestito e err dopo viene sovrascritto

	//Commit
	PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
	err = nvs_commit(my_handle);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");
#endif
	return err;
}

/**
 * @brief NVM__Erase
 *        call a "nvs_erase_all(...)" function
 *        for more details see esp-idf manual.
 *
 * @param  none
 *
 * @return C_RES
 */
C_RES NVM__Erase(void){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_erase_all(my_handle);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

	if (ESP_OK != err) return err;
	//TODO CPPCHECK prima non era gestito e err dopo viene sovrascritto

	//Commit
	PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
	err = nvs_commit(my_handle);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");
#endif
	return err;
}
