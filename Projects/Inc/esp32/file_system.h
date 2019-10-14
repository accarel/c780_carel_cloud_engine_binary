/*
 * file_system.h
 *
 *  Created on: Jun 19, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#ifndef MAIN_FILE_SYSTEM_H_
#define MAIN_FILE_SYSTEM_H_

//#include "common.h"
#include "esp_err.h"
#include <esp_log.h>
#include "memmgr.h"

#define MAX_NUM_OF_FILES    6

#define MODEL_FILE  	"/spiffs/model.bin"
#define CERT_1 			"/spiffs/cert1.crt"
#define CERT_2 			"/spiffs/cert2.crt"

/* file names
 * this is the named stored by the script that build the ffs
 * take care to upper/lower case
 * */




long filesize(const char *fname);
esp_err_t init_spiffs(void);
void FS_DisplayFiles(void);
void* FS_ReadFile(const char* filename);
void* FS_ReadFile2(const char* filename);
esp_err_t FS_SaveFile(const char* file_to_save, size_t file_size, const char* filename);

#endif /* MAIN_FILE_SYSTEM_H_ */
