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
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/

#ifdef INCLUDE_PLATFORM_DEPENDENT
/*-------------------------------
 * FILES DIRECTORY
 *-----------------------------*/
#define MODEL_FILE  	"/spiffs/model.bin"
#define CERT_1 			"/spiffs/cert1.crt"
#define CERT_2 			"/spiffs/cert2.crt"
#endif

/* Function prototypes -------------------------------------------------------*/
C_RES File_System_Init(void);


#ifdef __cplusplus
}
#endif

#endif