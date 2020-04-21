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


C_RES FS_CheckFiles(void){
    int fsize;
    struct stat file_stat;
    C_RES err1, err2;

    if (stat(CERT1_SPIFFS, &file_stat) == -1)
    {
      PRINTF_DEBUG("Cert1 in %s is not found\n", CERT1_SPIFFS);
      err1 = C_FAIL;
	}
    else
    {
	  fsize = (int)filesize(CERT1_SPIFFS);
	  PRINTF_DEBUG("Cert1 found in %s, size: %d \r\n", CERT1_SPIFFS, fsize);
	  err1 = C_SUCCESS;
	}

    if (stat(CERT2_SPIFFS, &file_stat) == -1)
    {
      PRINTF_DEBUG("Cert2 in %s is not found\n", CERT2_SPIFFS);
    }else
    {
      err2 = C_FAIL;
      fsize = (int)filesize(CERT2_SPIFFS);
      PRINTF_DEBUG("Cert2 found in %s, size: %d \r\n", CERT2_SPIFFS, fsize);
      err2 = C_SUCCESS;
    }
// check on model file sould be removed... no result is gathered
    if (stat(MODEL_FILE, &file_stat) == -1){
      PRINTF_DEBUG("Model File in %s is not found\n", MODEL_FILE);
    }else{
      fsize = (int)filesize(MODEL_FILE);
      PRINTF_DEBUG("Model File found in %s, size: %d \r\n", MODEL_FILE, fsize);
      }

   long lung;
	lung = filesize(CFG_DEF);

	PRINTF_DEBUG("lunghezza file dati %d \r\n", (int)lung);

	// check if data default files is present
	if (stat(CFG_DEF, &file_stat) == -1){

		PRINTF_DEBUG("Data File in %s is not found\n", CFG_DEF);

	}else{
		fsize = (int)filesize(CFG_DEF);

		// reading from file system the data configuration	(non-changeble)
		FS_GetCfgData(FILE_CFG_DEF);

		PRINTF_DEBUG("Data File found in %s, size: %d \r\n", CFG_DEF, fsize);
	}

	if (stat(CFG_USR, &file_stat) == -1){

		PRINTF_DEBUG("Data usr File in %s is not found\n", CFG_USR);

	}else{
		fsize = (int)filesize(CFG_USR);
		// reading from file system the data configuration	(changeble)

		FS_GetCfgData(FILE_CFG_USR);

		PRINTF_DEBUG("Data usr File found in %s, size: %d \r\n", CFG_USR, fsize);
	}



    if(C_SUCCESS == err1 || C_SUCCESS == err2){
    	return C_SUCCESS;
    }else{
    	return C_FAIL;
    }
}




void FS_DisplayFiles(void){
    int fsize;

    struct stat file_stat;

    if (stat(CERT1_SPIFFS, &file_stat) == -1){
    	PRINTF_DEBUG("%s is not found\n",CERT1_SPIFFS);
	}else{
		fsize = (int)filesize(CERT1_SPIFFS);
		PRINTF_DEBUG("%s %d \r\n ", CERT1_SPIFFS, fsize);
	}

    if (stat(CERT1_SPIFFS, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",CERT1_SPIFFS);
	}else{
		fsize = (int)filesize(CERT2_SPIFFS);
		PRINTF_DEBUG("%s %d \r\n ", CERT2_SPIFFS, fsize);
	}

    if (stat(MODEL_FILE, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",MODEL_FILE);
	}else{
		fsize = (int)filesize(MODEL_FILE);
		PRINTF_DEBUG("%s %d \r\n ", MODEL_FILE, fsize);
	}

    if (stat(LOGIN_HTML, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",LOGIN_HTML);
	}else{
		fsize = (int)filesize(LOGIN_HTML);
		PRINTF_DEBUG("%s %d \r\n ", LOGIN_HTML, fsize);
	}

    if (stat(CHANGE_CRED_HTML, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",CHANGE_CRED_HTML);
	}else{
		fsize = (int)filesize(CHANGE_CRED_HTML);
		PRINTF_DEBUG("%s %d \r\n ", CHANGE_CRED_HTML, fsize);
	}

    if (stat(CONFIG_HTML, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",CONFIG_HTML);
	}else{
		fsize = (int)filesize(CONFIG_HTML);
		PRINTF_DEBUG("%s %d \r\n ", CONFIG_HTML, fsize);
	}

    if (stat(STYLE_CSS, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",STYLE_CSS);
	}else{
		fsize = (int)filesize(STYLE_CSS);
		PRINTF_DEBUG("%s %d \r\n ", STYLE_CSS, fsize);
	}

    if (stat(FAV_ICON, &file_stat) == -1){
		PRINTF_DEBUG("%s is not found\n",FAV_ICON);
	}else{
		fsize = (int)filesize(FAV_ICON);
		PRINTF_DEBUG("%s %d \r\n ", FAV_ICON, fsize);
	}
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


C_RES FS_GetCfgData(C_BYTE file)
{
	C_UINT16 calcCrc, spiffsCrc;

	FILE *file_ptr_1 = NULL;
	FILE *file_ptr_2 = NULL;

	uint8_t *dati_1 = (uint8_t *)malloc(sizeof(cfg_data_t));
	uint8_t *dati_2 = (uint8_t *)malloc(sizeof(cfg_data_t));

	if(file == FILE_CFG_DEF)
	{
	   file_ptr_1 = fopen(CFG_DEF, "rb");

	   fread(dati_1, sizeof(uint8_t), sizeof(cfg_data_t), file_ptr_1);

	   fclose(file_ptr_1);
	}
	else if(file == FILE_CFG_USR)
	{
	  file_ptr_2 = fopen(CFG_USR, "rb");

	  fread(dati_2, sizeof(uint8_t), sizeof(cfg_data_t), file_ptr_2);

	  fclose(file_ptr_2);
	}

	if(file == FILE_CFG_DEF)
	  pCfgData = (struct cfg_data*)dati_1;
	else if(file == FILE_CFG_USR)
	  pCfgDataUsr = (struct cfg_data*)dati_2;

	if(file == FILE_CFG_DEF)
	{
		CfgData = *pCfgData;

		decrypt(CfgData.mqtt_pssw , CfgData.enc_key);
		decrypt(CfgData.mqtt_user , CfgData.enc_key);
	}
	else if(file == FILE_CFG_USR)
	{
	   CfgDataUsr = *pCfgDataUsr;

	   //controllo del crc
	   calcCrc = CRC16(&CfgDataUsr ,sizeof(cfg_data_t)-2);

	   spiffsCrc = ((C_UINT16)CfgDataUsr.crc[1]<<8) | (CfgDataUsr.crc[0]);

	   if(calcCrc != spiffsCrc)
		 return C_FAIL;

	   decrypt(CfgDataUsr.mqtt_pssw , CfgDataUsr.enc_key);
	   decrypt(CfgDataUsr.mqtt_user , CfgDataUsr.enc_key);
	}

	free(dati_1);
	free(dati_2);

	return C_SUCCESS;
}


C_RES FS_SaveCfgData(C_BYTE file)
{
	FILE * file_ptr;

	if(!file)
	{
	  file_ptr = fopen(CFG_DEF, "w");
	  fwrite(&CfgData, sizeof(uint8_t), sizeof(cfg_data_t), file_ptr);
	}
	else
	{
  	  file_ptr = fopen(CFG_USR, "w");
  	  fwrite(&CfgDataUsr, sizeof(uint8_t), sizeof(cfg_data_t), file_ptr);
	}

	fclose(file_ptr);

	return C_SUCCESS;
}

