/**
 * @file GSM_Miscellanous_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief   functions implementations specific related to the managment of 
 *          real time clock.
 */

#include "data_types_IS.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GSM_MISCELLANOUS_IS_H
#define __GSM_MISCELLANOUS_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "CAREL_GLOBAL_DEF.h"

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

//this define enable the output of the communications errors
#define __DEBUG_GSM_MISCELLANEOUS_IS_LEV_1

//this define enable the output of others debug informations
#define __DEBUG_GSM_MISCELLANEOUS_IS_LEV_2

#endif



/* Exported types ------------------------------------------------------------*/ 



/* Exported constants --------------------------------------------------------*/
 typedef enum{
 	PWRKEY_ON   = 0,
 	PWRKEY_OFF  = 1
 }PWRKEY_Status_t;


/* Function prototypes -------------------------------------------------------*/
void GSM_Misc_SMS_Config_Received();

void GSM_Misc_Get_Communication_Status();

void GSM_Misc_Get_Gateway_ID(C_BYTE *s_id);


void GSM_Module_IO_Init(void);

void GSM_Module_Pon_Poff(C_BYTE set_status);


#ifdef __cplusplus
}
#endif

#endif
