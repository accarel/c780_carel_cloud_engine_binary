/**
 * @file File_System_CAREL.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the 
 *         file system.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILE_SYSTEM_CAREL_H
#define __FILE_SYSTEM_CAREL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"   
   
   
   
/* Exported types ------------------------------------------------------------*/ 




/* Exported constants --------------------------------------------------------*/



/* Function prototypes -------------------------------------------------------*/
C_INT32 FSC_filesize(const C_CHAR *fname);
C_RES FSC_file_checksum_check(C_CHAR *fname);


#ifdef __cplusplus
}
#endif

#endif