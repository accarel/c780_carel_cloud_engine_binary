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
 *        Checks CRC of the file
 * 
 * @param file name
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_Check_File(C_CHAR* name)
{  /* TO BE implemented */
       	
	
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
       	
	
}

/**
 * @brief File_System_DefaultConfig_Load
 *        Loads default configuration to dedicated structure
 * 
 * @param void
 * @return C_SUCCESS or C_FAIL
 */
C_RES File_System_DefaultModel_Load(void)
{  /* TO BE implemented */
       	
	
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
       	
	
}




