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

#include "common.h"


#define MAX_NUM_OF_FILES    6


/* file names
 * this is the named stored by the script that build the ffs
 * take care to upper/lower case
 * */
#define CERT_1_NAME "/spiffs/cert1.crt"
#define CERT_2_NAME "/spiffs/cert2.crt"



long filesize(const char *fname);
esp_err_t init_spiffs(void);
void FS_DisplayFiles(void);
void* FS_ReadFile(const char* filename);



#endif /* MAIN_FILE_SYSTEM_H_ */
