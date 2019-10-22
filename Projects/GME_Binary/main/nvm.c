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

	    printf("\n");
	    printf("Opening Non-Volatile Storage (NVS) handle... ");
	    //nvs_handle my_handle;
	    err = nvs_open("storage", NVS_READWRITE, &my_handle);
	    if (err != ESP_OK) {
	        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	    } else {
	        printf("Done\n");
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
        printf("Reading %s from NVS ... ",var);
        
        err = nvs_get_u8(my_handle, var, val);
        printf("%d",*val);
        switch (err) {
            case ESP_OK:
                printf("Done\n");
                printf("%s = %d\n", var, *val);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default:
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        NVM__Close();
    return err;
}

esp_err_t NVM__WriteU8Value(const char* var, uint8_t val)
{
    esp_err_t err ;

    	NVM__Open();
        // Write
        //printf("Updating %s in NVS ... ", var);
        err = nvs_set_u8(my_handle, var, val);
        //printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        //printf("err = 0x%X\n",err);

        //Commit
        //printf("Committing updates in NVS ... ");
        esp_err_t err2 = nvs_commit(my_handle);
        //printf((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

        NVM__Close();
    printf("\n");

return err;
}


esp_err_t NVM__WriteString (const char* var, char* str)
{
    esp_err_t err ;


    	NVM__Open();
        //printf("Updating %s in NVS ... ",var);
        err = nvs_set_str(my_handle, var, str);
        //printf((err != ESP_OK) ? "Failed!" : "Done");
        //printf(" ..... Value = err = 0x%X\n",err);

        //Commit
        //printf("Committing updates in NVS ... ");
        esp_err_t err2 = nvs_commit(my_handle);
        //printf((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

        NVM__Close();
    return err;
}

#include "http_server.h"
void test_write_config_to_nvm(void){
	html_config_param_t config = HTTPServer__GetCustomConfig();

	esp_err_t err ;
	 printf("\n");
	    printf("Opening Non-Volatile Storage (NVS) handle... ");
	    //nvs_handle my_handle;
	    err = nvs_open("storage", NVS_READWRITE, &my_handle);
	    if (err != ESP_OK) {
	        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	    } else {
	        printf("Done\n");

	        printf("Updating ... \n");
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_SSID,  config.ap_ssid, strlen(config.ap_ssid));printf((err != ESP_OK) ? "Failed!" : "Done"); 					printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_PSWD,  config.ap_pswd, strlen(config.ap_pswd));printf((err != ESP_OK) ? "Failed!" : "Done"); 					printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_IP,  config.ap_ip, strlen(config.ap_ip));printf((err != ESP_OK) ? "Failed!" : "Done"); 						printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_AP_DHCP_IP,  config.ap_dhcp_ip, strlen(config.ap_dhcp_ip));printf((err != ESP_OK) ? "Failed!" : "Done"); 			printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_SSID,  config.sta_ssid, strlen(config.sta_ssid));printf((err != ESP_OK) ? "Failed!" : "Done"); 				printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_ENCRYP,  config.sta_encryption, strlen(config.sta_encryption));printf((err != ESP_OK) ? "Failed!" : "Done"); 		printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_PSWD,  config.sta_pswd, strlen(config.sta_pswd));printf((err != ESP_OK) ? "Failed!" : "Done"); 				printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_STATIC_IP,  config.sta_static_ip, strlen(config.sta_static_ip));printf((err != ESP_OK) ? "Failed!" : "Done"); 		printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_NETMASK,  config.sta_netmask, strlen(config.sta_netmask));printf((err != ESP_OK) ? "Failed!" : "Done"); 			printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_GATEWAY_IP,  config.sta_gateway_ip, strlen(config.sta_gateway_ip));printf((err != ESP_OK) ? "Failed!" : "Done"); 	printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_PRI_DNS,  config.sta_primary_dns, strlen(config.sta_primary_dns));printf((err != ESP_OK) ? "Failed!" : "Done"); 		printf(" ..... Value = err = 0x%X\n",err);
	        err = nvs_set_blob(my_handle, HTMLCONF_STA_SCND_DNS,  config.sta_secondary_dns, strlen(config.sta_secondary_dns));printf((err != ESP_OK) ? "Failed!" : "Done"); 	printf(" ..... Value = err = 0x%X\n",err);


	        //Commit
	        printf("Committing updates in NVS ... ");
	        esp_err_t err2 = nvs_commit(my_handle);
	        printf((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

	        NVM__Close();
	    }


}



esp_err_t NVM__ReadString(const char* var, char* str, size_t* len)
{
    esp_err_t err ;

// server_name should be str
    	NVM__Open();
    	nvs_get_str(my_handle, var, NULL, len);

    	char* temp = memmgr_alloc(*len);

    	err = nvs_get_str(my_handle, var, temp, len);



	    if (err != ESP_OK) {
	        printf("Error (%s) Read %s for length %ul!\n", esp_err_to_name(err),str, *len);
	    } else {
	    	printf("Reading %s, is %s  ....  length = %d \n",var ,temp, *len);
	    }

        NVM__Close();

        /*int i=0;
        for(i=0;i<*len;i++){
        	*str[i] = *server_name[i];
        }*/
        strcpy(str,temp);
        //printf("str %s, is %s  ....  length = %d \n",var ,str, *len);
        memmgr_free(temp);

    return err;
}


esp_err_t NVM__EraseKey(const char* var)
{
	esp_err_t err ;
	NVM__Open();

		err = nvs_erase_key(my_handle, var);
		printf((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

		//Commit
		printf("Committing updates in NVS ... ");
		esp_err_t err2 = nvs_commit(my_handle);
		printf((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

		NVM__Close();
	return err;
}



esp_err_t NVM__EraseAll(void)
{
	esp_err_t err ;
	NVM__Open();

		err = nvs_erase_all(my_handle);
		printf((err != ESP_OK) ? "Erasing Failed!\n" : "Erasing Done\n");

		//Commit
		printf("Committing updates in NVS ... ");
		esp_err_t err2 = nvs_commit(my_handle);
		printf((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

		NVM__Close();
	return err;
}














esp_err_t NVM__ReadBlob(const char* var, void* vec, size_t* len)
{
    esp_err_t err ;
    int i;
    //void* vec1;
    uint8_t conf[400];
    memset(conf,5,400*sizeof(uint8_t));
    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    //nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");

        err = nvs_get_blob(my_handle, var, vec, len);		//vec
        printf("Reading %s, is  ....  length = %d \n",var , (int)*len);

        //Commit
        //printf("Committing updates in NVS ... ");
        //esp_err_t err2 = nvs_commit(my_handle);
        //printf((err2 != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        //nvs_close(my_handle);
    }

    memcpy(conf,vec,*len);
    for(i=0;i<*len;i++){
    	//conf[i]= *vec;
    	//vec++;
    	printf("vec1[%d] = %d\n",i,conf[i]);
    }
    return err;
}


esp_err_t NVM__WriteBlob (const char* var, void* vec, size_t len)
{
    esp_err_t err ;
    int i;
    uint8_t conf[400] = {0};
    uint8_t conf1[400] = {0};
    uint8_t vec1[400] = {0};

    memcpy(conf,vec,len);
    printf("vec = \n");
    for(i=0;i<len;i++){

    	printf("write   %d\n",conf[i]);
    }

    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    //nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");

        printf("Updating %s in NVS ... ",var);
        err = nvs_set_blob(my_handle, var, vec, len);
        printf((err != ESP_OK) ? "Failed!" : "Done");
        printf(" ..... Value = err = 0x%X\n",err);

            //Commit
        printf("Committing updates in NVS ... ");
        esp_err_t err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        //nvs_close(my_handle);
    }

    /*NVM__ReadBlob(var, vec1, &len);
	//memcpy(conf1,vec1,len);
	printf("vec = \n");
	for(i=0;i<len;i++){

	printf("read   %d\n",vec1[i]);
	}*/

    /*printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    //nvs_handle my_handle;
    err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
      } else {
        printf("Done\n");


		err = nvs_get_blob(my_handle, var, vec1, &len);
		printf("Reading %s, is  ....  length = %d \n",var , len);

		printf("Committing updates in NVS ... ");
		esp_err_t err = nvs_commit(my_handle);
		printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

		// Close
		//nvs_close(my_handle);
		}


		memcpy(conf1,vec1,len);
		printf("vec = \n");
		for(i=0;i<len;i++){

		printf("read   %d\n",conf1[i]);
	}*/

    return err;
}













