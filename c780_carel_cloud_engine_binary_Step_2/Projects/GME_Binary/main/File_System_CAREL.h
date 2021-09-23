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
long FS_ReadFile(const char* filename, uint8_t* cert_ptr);
C_RES FS_SaveFile(const char* data_to_save, size_t data_size, const char* filename);
C_RES SaveCfgDefDataToNVM(void);

char* GetNtpServer(char* tmp_ntp_server);
char* GetMqttBroker(char* tmp_mqtt_broker);
char* GetMqttPort(char* tmp_mqtt_port);
char* GetMqttUser(char* tmp_mqtt_user);
char* GetMqttPassword(char* tmp_mqtt_password);
char* GetApnName(char* tmp_apn_name);
char* GetApnUserName(char* tmp_apn_username);
char* GetApnPassword(char* tmp_apn_password);


void decrypt_carel(char data[],int key, int size);
//void encrypt_carel(char data[],int key, int size);

int getSpiffsVersion();

#ifdef __cplusplus
}
#endif

#endif
