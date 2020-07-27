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

/**
 * @brief encrypt
 *        coding a block of data to be placed in SPIFFS
 *
 * @param data[]the data, key the encryption key
 * @return none
 */
void encrypt(char data[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(data);++i)
    {
    	data[i] = data[i] - key;
    }
}

/**
 * @brief decrypt
 *        decode a block of data placed in SPIFFS
 *
 * @param data[]the data, key the decryption key
 * @return none
 */
void decrypt(char data[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(data);++i)
    {
    	data[i] = data[i] + key;
    }
}

/**
 * @brief filesize
 *        return the size of a file
 *
 * @param fname the fully qualified file name
 * @return the size of the file
 */
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


/**
 * @brief FS_CheckFile
 *        check is a file exist or not
 *
 * @param fname the fully qualified file name
 * @return C_SUCCESS/C_FAIL
 */
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

/**
 * @brief FS_ReadFile
 *
 *
 * @param filename  the fully qualified file name
 * @param file_ptr  a pointer to the file handle
 * @return the file size
 */
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
			return 0;
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


/**
 * @brief FS_SaveFile
 *        save a data block to a file
 * @param data_to_save data to be stored
 * @param data_size
 * @param filename  the fully qualified file name
 * @return the file size
 */
C_RES FS_SaveFile(const char* data_to_save, size_t data_size, const char* filename){

		 FILE *file;

		file = fopen(filename, "w");
		if(NULL == file){
			PRINTF_DEBUG("%s - File not found\n",filename);
			return C_FAIL;
		}else{
			PRINTF_DEBUG("%s - File found\n",filename);
			fwrite(data_to_save , 1 , data_size , file );
			fclose(file);
			return C_SUCCESS;
		}

}


/**
 * @brief Check_spiffs_compatibility
 *		  USefull to check the spiffs version inside the device
 *
 * @param  none
 * @return C_SUCCESS/C_FAIL
 */

C_RES Check_spiffs_compatibility(void)
{
	int fw_ver;

	int spf_ver;
    int act_spf_ver;

	uint8_t saved_spiff_ver[2] = { 0,0 };
	uint8_t spiff_ver[2] = { 0,0 };

	C_RES err = C_FAIL;

	FILE *file_ptr_1 = NULL;

	fw_ver = atoi(GW_FW_REV);

	spf_ver = atoi(GW_SPIFFS_REV);

	uint8_t *dati_1 = (uint8_t *)malloc(sizeof(uint16_t));
	file_ptr_1 = fopen(CFG_DEF, "rb");
	fread(dati_1, sizeof(uint8_t), sizeof(uint16_t), file_ptr_1);
	fclose(file_ptr_1);

	saved_spiff_ver[1] = *dati_1;
	saved_spiff_ver[0] = *(dati_1+1);

	act_spf_ver =  (int)(saved_spiff_ver[0] << 8) | (saved_spiff_ver[1]);

	free(dati_1);

    if(act_spf_ver == spf_ver)
	{
    	err = C_SUCCESS;
    }
    else
    {
    	// bisogna capire se la versione può supportare lo spiffs presente
    	err = C_FAIL;
    }
    return err;
}



/**
 * @brief SaveCfgDefDataToNVM
 *        save some data in Non Volatile Memory
 * @param none
 * @return C_SUCCESS/C_FAIL
 */
C_RES SaveCfgDefDataToNVM(void) {
	C_RES err = C_SUCCESS;

	FILE *file_ptr_1 = NULL;

	//Check_spiffs_compatibility();
    /*  to added a spiffs check control before save the data in the NVM memory  */
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
	err |= NVM__WriteString(APN_NAME, CfgData.apn_name);
	err |= NVM__WriteString(APN_USERNAME, CfgData.apn_user);
	err |= NVM__WriteString(APN_PASSWORD, CfgData.apn_password);

	if (err == C_SUCCESS)
		err = NVM__WriteU8Value(CFG_DEF_NVM, 1);

	return err;
}




/**
 * @brief GetNtpServer
 *        retrieve the NTP URL from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_ntp_server buffer to store the URL
 * @return the same buffer pointer
 */
char* GetNtpServer(char* tmp_ntp_server){
	size_t len = 0;

	if (C_FAIL == NVM__ReadString(NTP_SERVER, tmp_ntp_server, &len))
	{
	   *tmp_ntp_server='\0'; //return empty string
	}

	return tmp_ntp_server;
}

/**
 * @brief GetMqttBroker
 *        retrieve the MQTT broker URL from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_mqtt_broker buffer to store the value
 * @return the same buffer pointer
 */
char* GetMqttBroker(char* tmp_mqtt_broker){
	size_t len = 0;

	if (C_FAIL== NVM__ReadString(MQTT_BROKER, tmp_mqtt_broker, &len))
	{
		*tmp_mqtt_broker = '\x0';//return empty string
	}
	return tmp_mqtt_broker;
}

/**
 * @brief GetMqttPort
 *        retrieve the MQTT Port from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_mqtt_port bbuffer to store the value
 * @return the same buffer pointer
 */
char* GetMqttPort(char* tmp_mqtt_port){
	size_t len = 0;

	if (C_FAIL == NVM__ReadString(MQTT_PORT, tmp_mqtt_port, &len))
	{
		*tmp_mqtt_port='\x0'; //return empty string
	}
	return tmp_mqtt_port;
}



/**
 * @brief GetMqttUser
 *        retrieve the MQTT user from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_mqtt_port buffer to store the value
 * @return the same buffer pointer
 */
char* GetMqttUser(char* tmp_mqtt_user){
	size_t len = 0;
	if (C_FAIL == NVM__ReadString(MQTT_USER, tmp_mqtt_user, &len))
	{
		*tmp_mqtt_user='\0'; //return empty string
	}
	return tmp_mqtt_user;
}

/**
 * @brief GetMqttPassword
 *        retrieve the MQTT pwd from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_mqtt_password buffer to store the value
 * @return the same buffer pointer
 */
char* GetMqttPassword(char* tmp_mqtt_password){
	size_t len = 0;
	if (C_FAIL == NVM__ReadString(MQTT_PASSWORD, tmp_mqtt_password, &len))
	{
		*tmp_mqtt_password='\0';  //return empty string
	}
	return tmp_mqtt_password;
}

/**
 * @brief GetApnName
 *        retrieve the APN name from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_apn_name buffer to store the value
 * @return the same buffer pointer
 */
char* GetApnName(char* tmp_apn_name){
	size_t len = 0;
	if (C_FAIL ==  NVM__ReadString(APN_NAME, tmp_apn_name, &len))
	{
		*tmp_apn_name='\0';		//return empty string
	}
	return tmp_apn_name;
}

/**
 * @brief GetApnUserName
 *        retrieve the APN user name from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_apn_username buffer to store the value
 * @return the same buffer pointer
 */
char* GetApnUserName(char* tmp_apn_username){
	size_t len = 0;
	if (C_FAIL == NVM__ReadString(APN_USERNAME, tmp_apn_username, &len))
	{
		*tmp_apn_username='\0';
	}
	return tmp_apn_username;
}


/**
 * @brief GetApnPassword
 *        retrieve the APN pwd from the Non Volatile Memory
 *        null string in case of reading error
 * @param tmp_apn_password buffer to store the value
 * @return the same buffer pointer
 */
char* GetApnPassword(char* tmp_apn_password){
	size_t len = 0;
	if (C_FAIL == NVM__ReadString(APN_PASSWORD, tmp_apn_password, &len))
	{
		*tmp_apn_password='\0';
	}
	return tmp_apn_password;
}
