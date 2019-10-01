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
      

/* Defines  ------------------------------------------------------------------*/
#define CRC_RESET_ENGINE 1
#define CRC_RETURN_CRC 2
#define CRC_ACCUMULATE 3

	  	  	  
	 
/* Functions Implementation --------------------------------------------------*/

/**
 * @brief FSC_filesize
 *        return the size in bytes of the given file 
 * 
 * @param none
 * @return the size in bytes of the file or -1 if file not found or I/O error 
 */
C_INT32 FSC_filesize(C_CHAR *fname)
{
  C_INT32 sz = -1;

  FILE* fp = fopen((char *)fname, "r");
  if (fp != NULL)
  {

    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
  }

  return sz;
}


/**
 * @brief FSC_CRC16_on_the_fly
 *        compute the CRC16 of the straem of data passed  
 * 
 * @param action CRC_RESET_ENGINE reset the CRC calculation
 *               CRC_RETURN_CRC   return the computed CRC 
 *               CRC_ACCUMULATE   compute the CRC byte by byte
 *
 * @return the CRC value returned is valid only if you use 
 *         action=CRC_RETURN_CRC in other case 0xFFFF is returned
 *       
 */
C_UINT16 FSC_CRC16_on_the_fly(C_BYTE action, C_BYTE nData)
{
	C_BYTE nTemp;
	static C_UINT16 wCRCWord;


	switch (action) {
   	    case CRC_RESET_ENGINE :
   	    	wCRCWord = 0xFFFF;
			return 0xFFFF;
   	    	
 	    case CRC_RETURN_CRC:
   	    	return wCRCWord;
   	    	   	    		   	    	
 	    case CRC_ACCUMULATE:
   	    	nTemp = nData ^ wCRCWord;
		    wCRCWord >>= 8;
		    wCRCWord ^= CRCTABLE[nTemp];
   	    	return 0xFFFF;
	     	
	    }

    return 0xFFFF;	    
}




/**
 * @brief FSC_file_checksum_check
 *        check if the checksum stored at the end of a file is right  
 * 
 * @param the file name to check
 * @return C_SUCCESS / C_FAIL
 */

C_RES FSC_file_checksum_check(C_CHAR *fname)
{
  C_INT32 sz = -1;
  C_INT32 fsize;
  C_INT32 count;
  C_BYTE value;
  C_UINT16 crc_value;
  C_RES retval;

  retval = C_FAIL;
  
  fsize = FSC_filesize(fname);

  /* file not exist */
  if (fsize == -1)
  {
	  return retval;
  }


  FILE* fp = fopen((char *)fname, "rb");
  
  value = 0;
  
  if (fp != NULL)
  {
    crc_value = FSC_CRC16_on_the_fly(CRC_RESET_ENGINE, value);

    for (count=0; count < fsize; count++)
	{		
		fread(&value,sizeof(C_BYTE),1,fp);		
		crc_value = FSC_CRC16_on_the_fly(CRC_ACCUMULATE, value);
	}
	
	
	crc_value = FSC_CRC16_on_the_fly(CRC_RETURN_CRC, value);
		
	if (0x0000 == crc_value)
	{
		retval = C_SUCCESS;
	}
	else
	{
		retval = C_FAIL;
	}
			
    fclose(fp);
  }

  return retval;	
}



/**
 * @brief File_System_Check_File
 *        Performs the action needed to validate a file
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Check_File(C_CHAR* name)
{  /* TO BE implemented */
	return 1;
	
}

/**
 * @brief File_System_Config_Load
 *        Loads info from configuration file to dedicated structure
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Config_Load(C_CHAR* name)
{  /* TO BE implemented */
       	return 1;
	
}

/**
 * @brief File_System_DefaultConfig_Load
 *        Loads default configuration to dedicated structure
 * 
 * @param void
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_DefaultConfig_Load(void)
{  /* TO BE implemented */
	return 1;      	
	
}

/**
 * @brief File_System_Cert_Check
 *        Checks certificate validity
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Cert_Check(C_CHAR* name)
{  /* TO BE implemented */
	return 1;       	
	
}

/**
 * @brief File_System_Cert_Load
 *        Loads certificate info to dedicated structure
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Cert_Load(C_CHAR* name)
{  /* TO BE implemented */
	return 1;   	
	
}

/**
 * @brief File_System_Cert_Store
 *        Saves certificate to file system
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Cert_Store(C_CHAR* name)
{  /* TO BE implemented */
	return 1;      	
	
}

/**
 * @brief File_System_Model_Check
 *        Checks model file
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Model_Check(C_CHAR* name)
{  /* TO BE implemented */
	return 1;       	
	
}

/**
 * @brief File_System_Model_Load
 *        Loads info in model file to dedicated structure
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Model_Load(C_CHAR* name)
{  /* TO BE implemented */
	return 1;       	
	
}

/**
 * @brief File_System_DefaultModel_Load
 *        Loads default model to dedicated structure
 * 
 * @param void
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_DefaultModel_Load(void)
{  /* TO BE implemented */
	return 1;       	
	
}

/**
 * @brief File_System_Model_Store
 *        Saves model to file system
 * 
 * @param void
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Model_Store(void)
{  /* TO BE implemented */
	return 1;      	
	
}

/**
 * @brief Get_RS485_baudrate
 *        Gets RS485 baud rate 
 * 
 * @param C_UINT32 contaning baud rate
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_RS485_BaudRate(C_UINT32* rs485_baud_rate)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_Device_Address
 *        Gets RS485 slave device address
 * 
 * @param void
 * @return C_UINT16 contaning device address 
 */
C_UINT16 Get_Device_Address(void)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_Guid
 *        Gets guid from model file
 * 
 * @param void
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_Guid(C_BYTE* Guid)
{  /* TO BE implemented */
	
	return 1;
}

/**
 * @brief Get_RS485_TTL
 *        Gets RS485 port to be used (TTL/3-way RS485) 
 * 
 * @param C_UINT32 contaning 0 to use 3-way connector and 1 for TTL connector
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_RS485_TTL(C_BYTE* rs485_port)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_RS485_StopBits
 *        Gets RS485 number of stop bits 
 * 
 * @param C_BYTE contaning the number of stop bits
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_RS485_StopBits(C_BYTE* rs485_stop_bits)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_RS485_Parity
 *        Gets RS485 parity
 * 
 * @param C_BYTE contaning the parity
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_RS485_Parity(C_BYTE* rs485_parity)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_MQTT_broker
 *        Gets MQTT broker URI
 * 
 * @param C_URI* contaning the mqtt broker
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_MQTT_broker(C_URI* mqtt_broker)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_MQTT_Port
 *        Gets MQTT broker port
 * 
 * @param C_UINT16* contaning the mqtt portbroker
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_MQTT_Port(C_UINT16* mqtt_port)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_Gateway_ID
 *        Gets MAC address of the WiFi device or IMEI of the 2G module
 * 
 * @param C_BYTE* contaning the MAC or the IMEI mqtt portbroker
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_Gateway_ID(C_GATEWAY_ID *s_id)
{ /* TO BE implemented */

#if (NETWORK_INTERFACE == WIFI_INTERFACE)
  /* this function returns the MAC address of the WiFi appliance */

#elif (NETWORK_INTERFACE == GSM_INTERFACE)
  /* this function returns the IMEI of the GSM module*/

#endif
	return 1;
}

/**
 * @brief Get_DNS
 *        Gets DNS server address
 * 
 * @param C_IPV4* contaning the address of the DNS server
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_DNS(C_IPV4* dns_address)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_NTP_server
 *        Gets NTP server name
 * 
 * @param C_URI* contaning the URI of the NTP server
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_NTP_server(C_URI* ntp_server)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_NTP_port
 *        Gets NTP port
 * 
 * @param C_UINT16* contaning the port to be used with the NTP server
 * @return C_SUCCESS or C_FAIL
 */
C_RES Get_NTP_port(C_UINT16* ntp_port)
{  /* TO BE implemented */
	return 1;
}

/**
 * @brief Get_GSM_APN
 *        Gets APN for GSM connection
 * 
 * @param C_CHAR* contaning the apn to be used for GSM connection
 * @return C_SUCCESS or C_FAIL
 */

C_RES Get_GSM_APN(C_CHAR* apn)
{  /* TO BE implemented */
	return 1;
}

