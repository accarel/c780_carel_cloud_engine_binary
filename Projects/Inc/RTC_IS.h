/*
  File  : RTC_IS.h
  
  Scope : 
  functions implementations specific related to the managment of the
  real time clock.
            
  
  Note  : 
    
*/



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_IS_H
#define __RTC_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "carel_types.h"   
   
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_RES RTC_Init(void);

C_TIME RTC_Get_UTC_Current_Time(void);



#ifdef __cplusplus
}
#endif

#endif