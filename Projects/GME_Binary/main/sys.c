
/*
 * mode_config.c
 *
 *  Created on: Jun 13, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */


#include "sys.h"
#include "nvm.h"
#include"wifi.h"
#include "common.h"
#include "cmd_custom.h"
#include "file_system.h"
#include "https_client_CAREL.h"
#include "driver/gpio.h"
#include "http_server.h"
#include "binary_model.h"
#include "utilities.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"

#include "data_types_CAREL.h"

//Debugging
static int test2=0;
html_config_param_t test_config;


//Variables
static TaskHandle_t CLI__CmdLine = NULL;
static config_sm_t config_sm = CHECK_FOR_CONFIG;
static httpd_handle_t AP_http_server = NULL;
static uint8_t wifi_conf;
static uint32_t factory_reset_debounce_counter = 0;
static uint32_t config_reset_debounce_counter = 0;
static char certificates[CERT_MAX_NUMBRER][CERT_MAX_SIZE] = {0};


//********************************************************
//					PUBLIC FUNCTIONS
//********************************************************

esp_err_t Sys__Init (void)
{
    if (ESP_OK != nvs_flash_init()){
    	printf("NVS PROPBLEM\n");
        return C_FAIL;
        }


    if (ESP_OK != init_spiffs()){      // File_system_init();
    	printf("SPIFFS PROPBLEM\n");
        return C_FAIL;
    }


//    WiFi__ErasingConfig();
//    printf("\n\n ERASED	\n");

    //Initializing Factory Reset button
    //gpio_pad_select_gpio(GPIO_NUM_0);
    //gpio_set_direction(GPIO_NUM_0, GPIO_MODE_DEF_INPUT);


    //Initializing CLI
    //initialize_console();
    //initialize_cmdline();

    //Lunch Command Line Task
    //xTaskCreate(cmdline_handler, "CLI__CmdLine", 2*CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, CLI__CmdLine );

    //Debugging purpose
    //memset(&test_config,0,sizeof(html_config_param_t));


    //return ESP_OK;
    return C_SUCCESS;
}



config_sm_t Sys_GetConfigSM(void){
    return config_sm;
}



void Sys_SetConfigSM(config_sm_t config_state){
    config_sm = config_state;
}



bool Sys__ResetCheck(void){
	// Debounce check using up/down counter every 10ms
	if (gpio_get_level(CONFIG_RESET_BUTTON) == 0) {
		config_reset_debounce_counter++;
		PRINTF_DEBUG("CONFIG RESET CHECK -- = %d\n",config_reset_debounce_counter);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}else if(gpio_get_level(CONFIG_RESET_BUTTON) == 1 && config_reset_debounce_counter > 0){
		config_reset_debounce_counter--;
		PRINTF_DEBUG("CONFIG RESET CHECK ++ = %d\n",config_reset_debounce_counter);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	if((CONFIG_RESET_SEC * 10) == config_reset_debounce_counter){
		//Erase configuration flag form NVM
		WiFi__ErasingConfig();
		unlink(MODEL_FILE);
		PRINTF_DEBUG("CONFIG RESET CHECK DONE = %d\n",config_reset_debounce_counter);
		return true;
	}
	return false;
}


bool Sys__FirmwareFactoryReset(void){
	// Debounce check using up/down counter every 10ms
	if (gpio_get_level(FACTORY_RESET_BUTTON) == 0) {
		factory_reset_debounce_counter++;
		PRINTF_DEBUG("FIRMWARE RESET CHECK -- = %d\n",factory_reset_debounce_counter);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}else if(gpio_get_level(FACTORY_RESET_BUTTON) == 1 && factory_reset_debounce_counter > 0){
		factory_reset_debounce_counter--;
		PRINTF_DEBUG("FIRMWARE RESET CHECK ++ = %d\n",factory_reset_debounce_counter);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	if((FACTORY_RESET_SEC * 10) == factory_reset_debounce_counter){
		//Point boot partition on /factory part.
		Sys__SetFactoryBootPartition();
		printf("GME Configuration has bees reseted\n");
		PRINTF_DEBUG("FIRMWARE RESET CHECK DONE = %d\n",factory_reset_debounce_counter);
		return true;
	}
	return false;
}


/*Configure the WiFi interface:
 * Param:
 * 		sm: state variable
 * return:
 * 		gme_sm_t: SM__Start state
 *
 * Description:
 * 	Situation 1:
 * 	Starts with checking the NVM if exists old configuration. If it finds old config, it will read them from the NVM
 * 	and configure the system directly, starts the wifi then run the whole system in base of the configuration found.
 *
 *	Situation 2:
 * 	If it finds nothing, it will start the http_server and the wifi with the default configuration (check gme_config.h),
 * 	then attends for the html configuration. After receiving them, it writes them in NVM and does a reboot. After the reboot,
 * 	the system will be found in situation 1.
 *
 * 	Note: 	1) The HTTP_Server is launched as Task, so it can be accessed after the configuration, during the system running, in case
 * 				we wanted to re-configure the wifi.
 * 			2) In case of the gateway is configured as AP Mode, the varies functionalities don't start and the system will attend the
 * 				new configuration.
 *
 * */


gme_sm_t Sys__Config (config_sm_t sm)
{
    config_sm = sm;
    while(1)
    {
        switch(config_sm)
        {
        case CHECK_FOR_CONFIG:
            printf("Sys__Config .... CHECK_FOR_CONFIG\n");
            /*Check in NVM*/

            if(ESP_OK == NVM__ReadU8Value("wifi_conf", &wifi_conf) && CONFIGURED == wifi_conf){
            	WiFi__ReadCustomConfigFromNVM();
                config_sm = CONFIGURE_GME;
            }else{
                config_sm = SET_DEFAULT_CONFIG;
            }
            break;
        
        case SET_DEFAULT_CONFIG:
            printf("Sys__Config .... SET_DEFAULT_CONFIG\n");
            if(WiFi__SetDefaultConfig()){
                config_sm = START_WIFI;
            }
            printf("Sys__Config .... SET_DEFAULT_CONFIG  END\n");
            break;
        
        case START_WIFI:
            printf("Sys__Config .... START_WIFI\n");
            if (WiFi__StartWiFi()){
                ESP_ERROR_CHECK(HTTPServer__StartFileServer(&AP_http_server, "/spiffs"));

                if(CONFIGURED == wifi_conf && APSTA_MODE == WiFi__GetCustomConfig().gateway_mode){
        			return GME_WAITING_FOR_INTERNET;
                }else{
                    config_sm = WAITING_FOR_HTML_CONF_PARAMETERS;
                }
            }
            break;
        
        case WAITING_FOR_HTML_CONF_PARAMETERS:
            if(test2 == 0){
            	printf("\nGateway Mode = %d, Wifi Conf has %d config\n\n",WiFi__GetCustomConfig().gateway_mode,wifi_conf);
            	printf("Sys__Config .... WAITING_FOR_HTML_CONF_PARAMETERS\n");
            	test2=10;
            }
            if(IsConfigReceived()){
            	printf("Configuration Received");
            	WiFi__WriteCustomConfigInNVM(HTTPServer__GetCustomConfig());

                if(ESP_OK == NVM__WriteU8Value("wifi_conf", (uint8_t)CONFIGURED)){
                	config_sm = CONFIGURE_GME;
                }
                if(wifi_conf == CONFIGURED){			//for future implementation
                	wifi_conf = TO_RECONFIGURE;
                }
            }
            break;

        case CONFIGURE_GME:
            printf("Sys__Config .... CONFIGURE_GME\n");
            if(CONFIGURED == wifi_conf){

            	WiFi__SetCustomConfig(WiFi__GetCustomConfig());
                config_sm = START_WIFI;
            }else{
                return GME_REBOOT;
            }
            break;


        default:
            break;
        }

        //If the factory reset button has been pressed for X time (look gme_config.h)
        if(true == Sys__ResetCheck())
        {
        	printf("RESET CHECK DONE SYS\n");
        	return GME_REBOOT;
        }
    }
}



//Certificatoins static allocation
void Sys__CertAlloc(void){
	if(NULL == FS_ReadFile2(CERT1_SPIFFS, (uint8_t*)certificates[0])){
		GME__Reboot();
	}
	if(NULL == FS_ReadFile2(CERT2_SPIFFS, (uint8_t*)certificates[1])){
		GME__Reboot();
	}
}

//Get certificatoins pointer
char* Sys__GetCert(uint8_t cert_num){
	if(cert_num == CERT_1|| cert_num == CERT_2){
		return certificates[cert_num];
	}else{
		return certificates[CERT_1];;
	}
}




//Change boot partition to factory
esp_err_t Sys__SetFactoryBootPartition(void){
	esp_err_t err;

	const esp_partition_t *factory = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);

	err = esp_ota_set_boot_partition(factory);

	if (err != ESP_OK) {
		printf("SetFactoryBootPartition failed!  err=0x%d", err);
		return err;
	}else{
		 printf("SetFactoryBootPartition Succeeded!");
		return ESP_OK;
	}
}
