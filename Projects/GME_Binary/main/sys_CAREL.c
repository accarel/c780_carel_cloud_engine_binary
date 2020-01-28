/**
 * @file   sys_CAREL.c
 * @author carel
 * @date 20 Nov 2019
 * @brief ... TODO
 *        
 */ 
 
#include "data_types_CAREL.h" 
#include "sys_CAREL.h"
#include "File_System_CAREL.h"
#include "common.h"
#include "cmd_custom.h"
#include "nvm_IS.h"

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
    if (C_SUCCESS != NVM_Init()){
    	printf("NVS PROBLEM\n");
        return C_FAIL;
        }


    if (C_SUCCESS != File_System_Init()){
    	printf("SPIFFS PROBLEM\n");
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





