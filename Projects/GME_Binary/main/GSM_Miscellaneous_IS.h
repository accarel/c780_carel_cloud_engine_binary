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


   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
void GSM_Misc_SMS_Config_Received();

void GSM_Misc_Get_Communication_Status();

void GSM_Misc_Get_Gateway_ID(C_BYTE *s_id);


#ifdef __cplusplus
}
#endif

#endif
