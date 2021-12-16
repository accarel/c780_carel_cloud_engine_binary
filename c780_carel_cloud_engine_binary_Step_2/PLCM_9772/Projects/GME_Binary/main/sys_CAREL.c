/**
 * @file   sys_CAREL.c
 * @author carel
 * @date 20 Nov 2019
 * @brief   contain some routine that manage global resource like NVM, certificates, and so on
 *        
 */ 
 
#include "data_types_CAREL.h" 
#include "sys_CAREL.h"
#include "sys_IS.h"
#include "File_System_CAREL.h"
#include "common.h"

#include "nvm_IS.h"

static char certificates[CERT_MAX_NUMBER][CERT_MAX_SIZE] = {0};

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
    	PRINTF_DEBUG_SYS("NVS PROBLEM\n");
    	P_COV_LN;
        return C_FAIL;
    }

    if (C_SUCCESS != File_System_Init()){
    	PRINTF_DEBUG_SYS("SPIFFS PROBLEM\n");
    	P_COV_LN;
        return C_FAIL;
    }

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
	FS_ReadFile(CERT1_SPIFFS, (uint8_t*)certificates[0]);
	FS_ReadFile(CERT2_SPIFFS, (uint8_t*)certificates[1]);
}



/**
 * @brief Sys__GetCert
 *      Get certificatoins pointer 
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

