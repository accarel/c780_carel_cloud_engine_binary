/**
 * @file File_System_CAREL.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  contains functions related to the file system      
 *         
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "data_types_CAREL.h"
#include "File_System_CAREL.h"
#include "gme_config.h"
#include "binary_model.h"
#include "nvm_CAREL.h"

void encrypt(char data[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(data);++i)
    {
    	data[i] = data[i] - key;
    }
}


void decrypt(char data[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(data);++i)
    {
    	data[i] = data[i] + key;
    }
}

long filesize(const char *fname)
{
  long sz = -1;

  FILE* fp = fopen(fname, "r");
  if (fp != NULL)
  {

    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
  }

  return sz;
}

C_RES FS_CheckFile(char* filename){
	C_RES err = C_FAIL;
	struct stat file_stat;
	int fsize;

	if (stat(filename, &file_stat) == -1) {
		PRINTF_DEBUG("File in %s is not found\n", filename);
	}
	else {
		fsize = (int)filesize(filename);
		PRINTF_DEBUG("Found file in %s, size: %d \r\n", filename, fsize);
		err = C_SUCCESS;
	}
	return err;
}

C_RES FS_CheckFiles(void){

	C_RES err = C_SUCCESS;

	err |= FS_CheckFile(CERT1_SPIFFS);
	err |= FS_CheckFile(CERT2_SPIFFS);
	err |= FS_CheckFile(CFG_DEF);

   	return err;
}

long FS_ReadFile(const char* filename, uint8_t* file_ptr){
		FILE *input_file_ptr;
		size_t sz_read;
		long size;
		//void* data = NULL;

		PRINTF_DEBUG("Reading from SPIFFS : %s\n",filename);

		input_file_ptr = fopen(filename, "rb");

		if (input_file_ptr == NULL)
		{
			PRINTF_DEBUG("Unable to open file! \n");
			return NULL;
		}

		PRINTF_DEBUG("Read File %s ok \n", filename);

		//calculation model dimension

		//Set file position pointer at the end of the file
		fseek(input_file_ptr, 0L, SEEK_END);
		//tell me actual file position number (the number is in bytes)
		size = ftell(input_file_ptr);
		//point the file position at the beginning of the file
		rewind(input_file_ptr);

		PRINTF_DEBUG("Size of file %s Ok: %ld \n", filename, size);

		sz_read = fread(file_ptr, sizeof(uint8_t), size, input_file_ptr);  // double
		file_ptr[size+1]=0;
		if(sz_read != size)
			PRINTF_DEBUG("Read ERROR!!!! \n");

		// close streaming
		fclose(input_file_ptr);

		return size;
}



C_RES FS_SaveFile(const char* file_to_save, size_t file_size, const char* filename){

		 FILE *file;

		file = fopen(filename, "w");
		if(NULL == file){
			PRINTF_DEBUG("%s - File not found\n",filename);
			return C_FAIL;
		}else{
			PRINTF_DEBUG("%s - File found\n",filename);
			fwrite(file_to_save , 1 , file_size , file );
			fclose(file);
			return C_SUCCESS;
		}

}

C_RES SaveCfgDefDataToNVM(void) {

	C_UINT16 calcCrc, spiffsCrc;
	C_RES err = C_SUCCESS;

	FILE *file_ptr_1 = NULL;
	uint8_t *dati_1 = (uint8_t *)malloc(sizeof(cfg_data_t));

	file_ptr_1 = fopen(CFG_DEF, "rb");

	fread(dati_1, sizeof(uint8_t), sizeof(cfg_data_t), file_ptr_1);

	fclose(file_ptr_1);

	pCfgData = (struct cfg_data*)dati_1;

	CfgData = *pCfgData;

	decrypt(CfgData.mqtt_pssw , CfgData.enc_key);
	decrypt(CfgData.mqtt_user , CfgData.enc_key);

	free(dati_1);

	err |= NVM__WriteString(MQTT_USER, CfgData.mqtt_user);
	err |= NVM__WriteString(MQTT_PASSWORD, CfgData.mqtt_pssw);
	err |= NVM__WriteString(MQTT_BROKER, CfgData.mqtt_broker);
	err |= NVM__WriteString(MQTT_PORT, CfgData.mqtt_port);
	err |= NVM__WriteString(NTP_SERVER, CfgData.ntp_server);

	if (err == C_SUCCESS)
		err = NVM__WriteU8Value(CFG_DEF_NVM, 1);

	return err;
}


char* GetNtpServer(char* tmp_ntp_server){
	size_t len = 0;
	NVM__ReadString(NTP_SERVER, tmp_ntp_server, &len);
	return tmp_ntp_server;
}


char* GetMqttBroker(char* tmp_mqtt_broker){
	size_t len = 0;
	NVM__ReadString(MQTT_BROKER, tmp_mqtt_broker, &len);
	return tmp_mqtt_broker;
}

char* GetMqttPort(char* tmp_mqtt_port){
	size_t len = 0;
	NVM__ReadString(MQTT_PORT, tmp_mqtt_port, &len);
	return tmp_mqtt_port;
}

char* GetMqttUser(char* tmp_mqtt_user){
	size_t len = 0;
	NVM__ReadString(MQTT_USER, tmp_mqtt_user, &len);
	return tmp_mqtt_user;
}

char* GetMqttPassword(char* tmp_mqtt_password){
	size_t len = 0;
	NVM__ReadString(MQTT_PASSWORD, tmp_mqtt_password, &len);
	return tmp_mqtt_password;
}
