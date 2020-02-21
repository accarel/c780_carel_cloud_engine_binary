/**
 * @file File_System_CAREL.h
 * @author carel
 * @date 9 Sep 2019
 * @brief CAREL functions related to the managment of the file system.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILE_SYSTEM_CAREL_H
#define __FILE_SYSTEM_CAREL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"   
#include "File_System_IS.h"
#include <string.h>
#include "data_types_CAREL.h"

/* Function prototypes -------------------------------------------------------*/
long filesize(const char *fname);
C_RES FS_CheckFiles(void);
void FS_DisplayFiles(void);
long FS_ReadFile(const char* filename, uint8_t* cert_ptr);
C_RES FS_SaveFile(const char* file_to_save, size_t file_size, const char* filename);

C_RES FS_GetCfgData(C_BYTE file);
C_RES FS_SaveCfgData(C_BYTE file);

void encrypt(char data[],int key);
void dencrypt(char data[],int key);

#ifdef __cplusplus
}
#endif

#endif
