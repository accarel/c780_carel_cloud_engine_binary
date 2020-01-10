#include "data_types_CAREL.h"
#include "nvm_IS.h"
#include "gme_types.h"

static nvs_handle my_handle;

C_RES NVM_Init(void)
{
#ifdef INCLUDE_PLATFORM_DEPENDENT
	return nvs_flash_init();
#endif
}

C_RES NVM__Open(void)
{
	PRINTF_DEBUG("Opening Non-Volatile Storage (NVS) handle... ");
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
	    PRINTF_DEBUG("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {
	    PRINTF_DEBUG("Done\n");
    }
#endif
	return err;
}

void NVM__Close(void){
	nvs_close(my_handle);
}

C_RES NVM__SetU8(const C_CHAR* var, C_BYTE val){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	// Write
	err = nvs_set_u8(my_handle, var, val);
	//Commit
	err = nvs_commit(my_handle);
#endif
	return err;
}

C_RES NVM__SetU32(const C_CHAR* var, C_UINT32 val){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	// Write
	err = nvs_set_u32(my_handle, var, val);
	//Commit
	err = nvs_commit(my_handle);
#endif
	return err;
}

C_RES NVM__SetString(const C_CHAR* var, char* str){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	// Write
	err = nvs_set_str(my_handle, var, str);
	//Commit
	err = nvs_commit(my_handle);
#endif
	return err;
}

C_RES NVM__SetBlob(const C_CHAR* var, void* vec, size_t len){
	C_RES err = C_FAIL;

	PRINTF_DEBUG("Updating %s in NVS ... ",var);
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_set_blob(my_handle, var, vec, len);
	PRINTF_DEBUG((err != ESP_OK) ? "Failed!" : "Done");
	PRINTF_DEBUG(" ..... Value = err = 0x%X\n",err);

	//Commit
	PRINTF_DEBUG("Committing updates in NVS ... ");
	err = nvs_commit(my_handle);
	PRINTF_DEBUG((err != ESP_OK) ? "Failed!\n" : "Done\n");
#endif

	return err;
}

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

C_RES NVM__GetString(const C_CHAR* var, char* str, size_t* len){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_get_str(my_handle, var, NULL, len);
	if (*len == 0){
		PRINTF_DEBUG("Nothing to allocate\n");
		return C_FAIL;
	}
	char* temp = malloc(*len);
	if (temp == NULL){
		PRINTF_DEBUG("No more space to allocate\n");
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

C_RES NVM__GetBlob(const C_CHAR* var, void* vec, size_t* len){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_get_blob(my_handle, var, NULL, len);
	if (*len == 0){
		PRINTF_DEBUG("Nothing to allocate\n");
		return C_FAIL;
	}
	char* temp = malloc(*len);
	if (temp == NULL){
		PRINTF_DEBUG("No more space to allocate");
		return C_FAIL;
	}

	err = nvs_get_blob(my_handle, var, temp, len);
	if (err == C_SUCCESS) {
		PRINTF_DEBUG("Reading %s, is  ....  length = %d \n",var , (int)*len);
		memcpy(vec,(void*)temp,*len);
	}
	free(temp);
#endif
	return err;

}

C_RES NVM__EraseK(const C_CHAR* var){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_erase_key(my_handle, var);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

	//Commit
	PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
	err = nvs_commit(my_handle);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");
#endif
	return err;
}

C_RES NVM__Erase(void){
	C_RES err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
	err = nvs_erase_all(my_handle);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

	//Commit
	PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
	err = nvs_commit(my_handle);
	PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");
#endif
	return err;
}
