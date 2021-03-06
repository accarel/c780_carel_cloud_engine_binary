/**
 * @file RTC_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
           real time clock.
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_IS_H
#define __RTC_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_IS.h"
#include "data_types_CAREL.h"
   

 /* ========================================================================== */
 /* typedefs and defines                                                       */
 /* ========================================================================== */
 #ifdef __CCL_DEBUG_MODE

 //this define enable the output of the communications errors
 #define __DEBUG_RTC_IS_LEV_1

 //this define enable the output of others debug informations
 #define __DEBUG_RTC_IS_LEV_2

 #endif






/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_RES RTC_Init(C_URI ntp_server, C_UINT16 ntp_port);
C_RES RTC_Sync(void);
C_TIME RTC_Get_UTC_Current_Time(void);

C_TIME RTC_Get_UTC_Boot_Time(void);
void RTC_Set_UTC_Boot_Time(void);

C_TIME RTC_Get_UTC_MQTTConnect_Time(void);
void RTC_Set_UTC_MQTTConnect_Time(void);

#ifdef __cplusplus
}
#endif

#endif
