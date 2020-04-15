/**
 * @file File_System_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the 
 *         file system.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILE_SYSTEM_IS_H
#define __FILE_SYSTEM_IS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"   
   
#ifdef INCLUDE_PLATFORM_DEPENDENT
//#include "file_system.h"
#endif
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/


/* Function prototypes -------------------------------------------------------*/
C_RES File_System_Init(void);
C_RES Set_Gateway_ID(void);
C_RES Get_Gateway_ID(C_SBYTE *s_id);

#ifdef __cplusplus
}
#endif

#endif
