/*
 * nvm.c
 *
 *  Created on: Jun 18, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#include "nvm.h"
#include "sys.h"
#include "data_types_IS.h"
#include "data_types_CAREL.h"

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "esp_spiffs.h"

static nvs_handle my_handle;

void NVM__Open(void){
	esp_err_t err ;

	    PRINTF_DEBUG_NVM("\n");
	    PRINTF_DEBUG_NVM("Opening Non-Volatile Storage (NVS) handle... ");
	    //nvs_handle my_handle;
	    err = nvs_open("storage", NVS_READWRITE, &my_handle);
	    if (err != ESP_OK) {
	        PRINTF_DEBUG_NVM("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	    } else {
	        PRINTF_DEBUG_NVM("Done\n");
	    }
}
void NVM__Close(void){
	nvs_close(my_handle);
}
#endif

C_INT16 NVM__ReadU8Value(const C_SCHAR* var, C_BYTE* val)
{
    C_INT16 err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT

    	NVM__Open();
        // Read
        PRINTF_DEBUG_NVM("Reading %s from NVS ... ",var);
        
        err = nvs_get_u8(my_handle, var, val);
        PRINTF_DEBUG_NVM("%d",*val);
        switch (err) {
            case ESP_OK:
                PRINTF_DEBUG_NVM("Done\n");
                PRINTF_DEBUG_NVM("%s = %d\n", var, *val);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_DEBUG_NVM("The value is not initialized yet!\n");
                break;
            default:
                PRINTF_DEBUG_NVM("Error (%s) reading!\n", esp_err_to_name(err));
        }

        NVM__Close();
#endif
    return err;
}

C_INT16 NVM__WriteU8Value(const C_SCHAR* var, C_BYTE val)
{
    C_INT16 err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
		NVM__Open();
        // Write
        //PRINTF_DEBUG_NVM("Updating %s in NVS ... ", var);
        err = nvs_set_u8(my_handle, var, val);
        //PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");
        //PRINTF_DEBUG_NVM("err = 0x%X\n",err);

        //Commit
        //PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
        esp_err_t err2 = nvs_commit(my_handle);
        //PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

        NVM__Close();
    PRINTF_DEBUG_NVM("\n");

#endif
return err;
}



C_INT16 NVM__ReadU32Value(const C_SCHAR* var, C_UINT32* val)
{
    C_INT16 err =C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT

    	NVM__Open();
        // Read
        PRINTF_DEBUG_NVM("Reading %s from NVS ... ",var);

        err = nvs_get_u32(my_handle, var, val);
        PRINTF_DEBUG_NVM("%d",*val);
        switch (err) {
            case ESP_OK:
                PRINTF_DEBUG_NVM("Done\n");
                PRINTF_DEBUG_NVM("%s = %d\n", var, *val);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                PRINTF_DEBUG_NVM("The value is not initialized yet!\n");
                break;
            default:
                PRINTF_DEBUG_NVM("Error (%s) reading!\n", esp_err_to_name(err));
        }

        NVM__Close();
#endif
    return err;
}

C_INT16 NVM__WriteU32Value(const C_SCHAR* var, C_UINT32 val)
{
	C_INT16 err = C_FAIL;
#ifdef INCLUDE_PLATFORM_DEPENDENT
    	NVM__Open();
        // Write
        //PRINTF_DEBUG_NVM("Updating %s in NVS ... ", var);
        err = nvs_set_u32(my_handle, var, val);

        //Commit
        //PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
        esp_err_t err2 = nvs_commit(my_handle);

        NVM__Close();
    PRINTF_DEBUG_NVM("\n");
#endif
	return err;
}


C_INT16 NVM__WriteString (const C_SCHAR* var, C_SCHAR* str)
{
    C_INT16 err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    	NVM__Open();
        //PRINTF_DEBUG_NVM("Updating %s in NVS ... ",var);
        err = nvs_set_str(my_handle, var, str);
        //PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done");
        //PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);

        //Commit
        //PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
        esp_err_t err2 = nvs_commit(my_handle);
        //PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

        NVM__Close();
#endif
    return err;
}

#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "http_server.h"
void test_write_config_to_nvm(void){
	html_config_param_t config = HTTPServer__GetCustomConfig();

	esp_err_t err ;
	 PRINTF_DEBUG_NVM("\n");
	    PRINTF_DEBUG_NVM("Opening Non-Volatile Storage (NVS) handle... ");
	    //nvs_handle my_handle;
	    err = nvs_open("storage", NVS_READWRITE, &my_handle);
	    if (err != ESP_OK) {
	        PRINTF_DEBUG_NVM("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	    } else {
	        PRINTF_DEBUG_NVM("Done\n");

	        PRINTF_DEBUG_NVM("Updating ... \n");
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_SSID,  config.ap_ssid, strlen(config.ap_ssid));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 					PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_PSWD,  config.ap_pswd, strlen(config.ap_pswd));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 					PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_IP,  config.ap_ip, strlen(config.ap_ip));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 						PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_DHCP_IP,  config.ap_dhcp_ip, strlen(config.ap_dhcp_ip));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 			PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_SSID,  config.sta_ssid, strlen(config.sta_ssid));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 				PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_ENCRYP,  config.sta_encryption, strlen(config.sta_encryption));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 		PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_PSWD,  config.sta_pswd, strlen(config.sta_pswd));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 				PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_STATIC_IP,  config.sta_static_ip, strlen(config.sta_static_ip));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 		PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_NETMASK,  config.sta_netmask, strlen(config.sta_netmask));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 			PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_GATEWAY_IP,  config.sta_gateway_ip, strlen(config.sta_gateway_ip));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 	PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_PRI_DNS,  config.sta_primary_dns, strlen(config.sta_primary_dns));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 		PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_SCND_DNS,  config.sta_secondary_dns, strlen(config.sta_secondary_dns));PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done"); 	PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);


	        //Commit
	        PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
	        esp_err_t err2 = nvs_commit(my_handle);
	        PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

	        NVM__Close();
	    }
}
#endif

C_INT16 NVM__ReadString(const C_SCHAR* var, C_SCHAR* str, size_t* len)
{
    C_INT16 err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    	NVM__Open();
    	nvs_get_str(my_handle, var, NULL, len);

    	char* temp = memmgr_alloc(*len);

    	err = nvs_get_str(my_handle, var, temp, len);



	    if (err != ESP_OK) {
	        PRINTF_DEBUG_NVM("Error (%s) Read %s for length %ul!\n", esp_err_to_name(err),str, *len);
	    } else {
	    	PRINTF_DEBUG_NVM("Reading %s, is %s  ....  length = %d \n",var ,temp, *len);
	    }

        NVM__Close();


        strcpy(str,temp);
        memmgr_free(temp);

#endif
    return err;
}


C_INT16 NVM__EraseKey(const C_SCHAR* var)
{
	C_INT16 err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	NVM__Open();

		err = nvs_erase_key(my_handle, var);
		PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

		//Commit
		PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
		esp_err_t err2 = nvs_commit(my_handle);
		PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

		NVM__Close();
#endif
	return err;
}


C_INT16 NVM__EraseAll(void)
{
	C_INT16 err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	NVM__Open();

		err = nvs_erase_all(my_handle);
		PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

		//Commit
		PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
		esp_err_t err2 = nvs_commit(my_handle);
		PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

		NVM__Close();

#endif
	return err;
}


C_INT16 NVM__ReadBlob(const C_SCHAR* var, void* vec, size_t* len)
{
    C_INT16 err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	        // Open
        PRINTF_DEBUG_NVM("\n");
        PRINTF_DEBUG_NVM("Opening Non-Volatile Storage (NVS) handle... ");
        //nvs_handle my_handle;
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            PRINTF_DEBUG_NVM("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        } else {
            PRINTF_DEBUG_NVM("Done\n");

            err = nvs_get_blob(my_handle, var, NULL, len);

            char* temp = memmgr_alloc(*len);

            err = nvs_get_blob(my_handle, var, temp, len);
            PRINTF_DEBUG_NVM("Reading %s, is  ....  length = %d \n",var , (int)*len);

            NVM__Close();

		   memcpy(vec,(void*)temp,*len);
		   memmgr_free(temp);

        }
#endif
    return err;
}


C_INT16 NVM__WriteBlob (const C_SCHAR* var, void* vec, size_t len)
{
    C_INT16 err = C_FAIL;

#ifdef INCLUDE_PLATFORM_DEPENDENT
    // Open
    PRINTF_DEBUG_NVM("\n");
    PRINTF_DEBUG_NVM("Opening Non-Volatile Storage (NVS) handle... ");
    //nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        PRINTF_DEBUG_NVM("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        PRINTF_DEBUG_NVM("Done\n");

        PRINTF_DEBUG_NVM("Updating %s in NVS ... ",var);
        err = nvs_set_blob(my_handle, var, vec, len);
        PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!" : "Done");
        PRINTF_DEBUG_NVM(" ..... Value = err = 0x%X\n",err);

            //Commit
        PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
        esp_err_t err = nvs_commit(my_handle);
        PRINTF_DEBUG_NVM((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
#endif
    return err;
}
