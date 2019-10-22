
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
#include "https_client.h"
#include "driver/gpio.h"
#include "http_server.h"
#include "binary_model.h"


//Debugging
static int test2=0;
html_config_param_t test_config;


//Variables
static TaskHandle_t CLI__CmdLine = NULL;
static config_sm_t config_sm = CHECK_FOR_CONFIG;
static httpd_handle_t AP_http_server = NULL;
static uint8_t wifi_conf;
static uint8_t debounce_counter = 0;




//********************************************************
//					PUBLIC FUNCTIONS
//********************************************************

esp_err_t Sys__Init (void)
{
	/*esp_err_t err;
	err = nvs_flash_init();
	if (ESP_OK != err){
		printf("NVS PROPBLEM = 0x%X\n",err);
		return ESP_FAIL;
		}*/


    if (ESP_OK != nvs_flash_init()){
    	printf("NVS PROPBLEM\n");
        return ESP_FAIL;
        }


    if (ESP_OK != init_spiffs()){
    	printf("SPIFFS PROPBLEM\n");
        return ESP_FAIL;
    }


//    WiFi__ErasingConfig();
//    printf("\n\n ERASED	\n");

    //Initializing Factory Reset button
    gpio_pad_select_gpio(GPIO_NUM_0);
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_DEF_INPUT);

    //Initializing Memory Heap Manager
    memmgr_init();

    //Initializing CLI
    //initialize_console();
    //initialize_cmdline();

    //Lunch Command Line Task
    //xTaskCreate(cmdline_handler, "CLI__CmdLine", 2*CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, CLI__CmdLine );

    //Debugging purpose
    //memset(&test_config,0,sizeof(html_config_param_t));


    //return ESP_OK;
    return 1;
}



config_sm_t Sys_GetConfigSM(void){
    return config_sm;
}



void Sys_SetConfigSM(config_sm_t config_state){
    config_sm = config_state;
}



bool Sys__ResetCheck(void){
	// Debounce check using up/down counter every 10ms
	if (gpio_get_level(GPIO_NUM_0) == 0) {
		debounce_counter++;
		printf("RESET CHECK -- = %d\n",debounce_counter);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}else if(gpio_get_level(GPIO_NUM_0) == 1 && debounce_counter > 0){
		debounce_counter--;
		printf("RESET CHECK ++ = %d\n",debounce_counter);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	if((FACTORY_RESET_SEC*10) == debounce_counter){
		//Erase configuration flag form NVM
		NVM__EraseKey("wifi_conf");
		printf("RESET CHECK DONE = %d\n",debounce_counter);
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
 *
 *	TODO: Sys__Config states diagram
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
        			return GME_STRAT_SYS;
                }else{
                    config_sm = WAITING_FOR_HTML_CONF_PARAMETERS;
                }
            }
            break;
        
        case WAITING_FOR_HTML_CONF_PARAMETERS:
            if(test2 == 0){
            	printf("Sys__Config .... WAITING_FOR_HTML_CONF_PARAMETERS\n");
            	test2=10;
            }
            if(IsConfigReceived()){
            	printf("Configuration Received");
            	WiFi__WriteCustomConfigInNVM(HTTPServer__GetCustomConfig());
            	//test_config=WiFi__ReadCustomConfigFromNVM();

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



