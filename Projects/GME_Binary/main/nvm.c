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

esp_err_t NVM__ReadU8Value(const char* var, uint8_t* val)
{
    esp_err_t err ;

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
    return err;
}

esp_err_t NVM__WriteU8Value(const char* var, uint8_t val)
{
    esp_err_t err ;

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

return err;
}



esp_err_t NVM__ReadU32Value(const char* var, uint32_t* val)
{
    esp_err_t err ;

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
    return err;
}

esp_err_t NVM__WriteU32Value(const char* var, uint32_t val)
{
    esp_err_t err ;

    	NVM__Open();
        // Write
        //PRINTF_DEBUG_NVM("Updating %s in NVS ... ", var);
        err = nvs_set_u32(my_handle, var, val);

        //Commit
        //PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
        esp_err_t err2 = nvs_commit(my_handle);

        NVM__Close();
    PRINTF_DEBUG_NVM("\n");

return err;
}





esp_err_t NVM__WriteString (const char* var, char* str)
{
    esp_err_t err ;


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
    return err;
}

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



esp_err_t NVM__ReadString(const char* var, char* str, size_t* len)
{
    esp_err_t err ;

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

    return err;
}


esp_err_t NVM__EraseKey(const char* var)
{
	esp_err_t err ;
	NVM__Open();

		err = nvs_erase_key(my_handle, var);
		PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

		//Commit
		PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
		esp_err_t err2 = nvs_commit(my_handle);
		PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

		NVM__Close();
	return err;
}



esp_err_t NVM__EraseAll(void)
{
	esp_err_t err ;
	NVM__Open();

		err = nvs_erase_all(my_handle);
		PRINTF_DEBUG_NVM((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

		//Commit
		PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
		esp_err_t err2 = nvs_commit(my_handle);
		PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

		NVM__Close();
	return err;
}





/*
esp_err_t NVM__ReadBlob(const char* var, void* vec, size_t* len)
{
    esp_err_t err ;
    int i;
    //void* vec1;
    uint8_t conf[400];
    memset(conf,5,400*sizeof(uint8_t));
    // Open
    PRINTF_DEBUG_NVM("\n");
    PRINTF_DEBUG_NVM("Opening Non-Volatile Storage (NVS) handle... ");
    //nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        PRINTF_DEBUG_NVM("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        PRINTF_DEBUG_NVM("Done\n");

        err = nvs_get_blob(my_handle, var, vec, len);		//vec
        PRINTF_DEBUG_NVM("Reading %s, is  ....  length = %d \n",var , (int)*len);

        //Commit
        //PRINTF_DEBUG_NVM("Committing updates in NVS ... ");
        //esp_err_t err2 = nvs_commit(my_handle);
        //PRINTF_DEBUG_NVM((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        //nvs_close(my_handle);
    }

    memcpy(conf,vec,*len);
    for(i=0;i<*len;i++){
    	//conf[i]= *vec;
    	//vec++;
    	PRINTF_DEBUG_NVM("vec1[%d] = %d\n",i,conf[i]);
    }
    return err;
}

*/



esp_err_t NVM__ReadBlob(const char* var, void* vec, size_t* len)
{
    esp_err_t err ;

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

    return err;
}












esp_err_t NVM__WriteBlob (const char* var, void* vec, size_t len)
{
    esp_err_t err ;

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

    return err;
}













