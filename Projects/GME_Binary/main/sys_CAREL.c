/**
 * @file   sys_CAREL.c
 * @author carel
 * @date 20 Nov 2019
 * @brief ... TODO
 *        
 */ 
 
#include "data_types_CAREL.h" 
#include "sys_CAREL.h"
#include "sys_IS.h"
#include "File_System_IS.h"
#include "https_client_CAREL.h"

#include "nvm.h"
#include "wifi.h"
#include "common.h"
#include "cmd_custom.h"

#include "driver/gpio.h"

#include "http_server.h"

#include "binary_model.h"



//Debugging
static int test2=0;
html_config_param_t test_config;


//Variables
//static TaskHandle_t CLI__CmdLine = NULL;				// if we want the console...esp32 library...?!?! 

static config_sm_t config_sm = CHECK_FOR_CONFIG;
static httpd_handle_t AP_http_server = NULL;
static uint8_t wifi_conf;

static char certificates[CERT_MAX_NUMBRER][CERT_MAX_SIZE] = {0};


//********************************************************
//					PUBLIC FUNCTIONS
//********************************************************



/**
 * @brief Sys_Init
 *      check the integrity and inizialize the non volatile storage   
 *      inizialize the file system   
 *
 * @param  none
 * @return C_RES
 */
C_RES Sys__Init (void)						 
{
    if (C_SUCCESS != nvs_flash_init()){
    	printf("NVS PROPBLEM\n");
        return C_FAIL;
        }


    if (C_SUCCESS != File_System_Init()){
    	printf("SPIFFS PROPBLEM\n");
        return C_FAIL;
    }

    //Initializing Factory Reset button
    //gpio_pad_select_gpio(GPIO_NUM_0);
    //gpio_set_direction(GPIO_NUM_0, GPIO_MODE_DEF_INPUT);


    //Initializing CLI
    //initialize_console();
    //initialize_cmdline();

    //Lunch Command Line Task
    //xTaskCreate(cmdline_handler, "CLI__CmdLine", 2*CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, CLI__CmdLine );

    return C_SUCCESS;
}


/**
 * @brief Sys_GetConfigSM
 *       return state machine config status   
 *
 * @param  none
 * @return C_RES
 */
config_sm_t Sys_GetConfigSM(void){
    return config_sm;
}

/**
 * @brief Sys_SetConfigSM
 *      set state machine config status      
 *
 * @param  none
 * @return C_RES
 */
void Sys_SetConfigSM(config_sm_t config_state){
    config_sm = config_state;
}



/**
 * @brief Sys__Config
 *   Configure the WiFi interface:
 * @Param:
 * 		sm: state variable
 * @return:
 * 		gme_sm_t: SM__Start state
 *
 * @Description:
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

            if(C_SUCCESS == NVM__ReadU8Value("wifi_conf", &wifi_conf) && CONFIGURED == wifi_conf){
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

                if(C_SUCCESS == NVM__WriteU8Value("wifi_conf", (uint8_t)CONFIGURED)){
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


/**
 * @brief Sys__CertAlloc
 *      Certificatoins static allocation 
 *
 *
 * @param  none
 * @return C_RES
 */

void Sys__CertAlloc(void)
{
	if(0 == FS_ReadFile(CERT1_SPIFFS, (uint8_t*)certificates[0])){
		GME__Reboot();
	}
	if(0 == FS_ReadFile(CERT2_SPIFFS, (uint8_t*)certificates[1])){
		GME__Reboot();
	}
}



/**
 * @brief Sys__GetCert
 *      Get certificatoins pointer 
 *
 *
 * @param  none
 * @return C_RES
 */
C_SBYTE* Sys__GetCert(C_BYTE cert_num){
	if(cert_num == CERT_1|| cert_num == CERT_2){
		return certificates[cert_num];
	}else{
		return certificates[CERT_1];
	}
}





