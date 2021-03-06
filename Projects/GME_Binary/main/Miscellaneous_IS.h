/**
 * @file Miscellanous_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief   functions implementations specific related to the managment of HW
 *          
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MISCELLANOUS_IS_H
#define __MISCELLANOUS_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"      
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/

void GSM_Misc_SMS_Config_Received(void);
void GSM_Misc_Get_Communication_Status(void);

#ifdef __cplusplus
}
#endif

#endif
