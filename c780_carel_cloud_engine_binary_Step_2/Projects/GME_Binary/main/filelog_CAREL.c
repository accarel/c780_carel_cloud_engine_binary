/**
 * @file log_CAREl.h
 * @author  CAREL
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the read of LOG (STone way)
 */

/* Includes ------------------------------------------------------------------------ */

#include "CAREL_GLOBAL_DEF.h"
#include "sys_IS.h"
#include "modbus_IS.h"
#include "CBOR_CAREL.h"
#include "ota_CAREL.h"
#include "binary_model.h"
#include "filelog_CAREL.h"

#include "unlock_CAREL.h"

// prototipe
static logfile_sm_t FileLog_Full_SM(void);


enum SM_Read_File{
	SM_READ_INIT = 0,
	SM_READ_SBLOCK,
	SM_READ_CMP_HEADER,
	SM_READ_CHUNK,
	SM_READ_LAST_CHUNK,
	SM_READ_SEND,
	SM_READ_FINISH
};



static const char *TAG = "LOG";

//
// data from CBOR message
//
static C_UINT16 file_id 	= 0;
static C_UINT32 file_start  = 0;
static C_UINT32 file_len    = 0;

static unsigned char data_rx[260];

/**
 * @brief ReadDevFile
 * function useful to read a modbus file
 *
 * @param  C_UINT16 ch_size
 * @param  C_UINT16 file_no
 * @param  C_UINT16 starting_reg
 *
 * @return C_RES
 */
C_RES ReadDevFile(C_UINT16 ch_size, C_UINT16 file_no, C_UINT16 starting_reg){


	C_RES err = C_SUCCESS;

	C_BYTE *data_tx;
	C_BYTE packet_len;

	//header vars
	C_UINT16 crc;

	packet_len =  MAX_HEADER_BYTES + 2;				// + 2 for crc

	ch_size = ch_size/2 + ch_size%2;

	data_tx = malloc(packet_len);
	memset((void*)data_tx, 0, packet_len);

	data_tx[SLAVE_ADD] = 0x01;
	data_tx[FUNC_CODE] = 0x14;
	data_tx[REQ_DATA_LEN] = packet_len - 3 - 2;		//2 for func_code and req_data_len 	and	 2 for crc
	data_tx[REF_TYPE] = 0x06;

	data_tx[FILE_NO_LO] = (uint8_t)(file_no & 0x00FF);
	data_tx[FILE_NO_HI] = (uint8_t)((file_no >> 8) & 0x00FF);

	data_tx[RECORD_NO_LO] = (uint8_t)(starting_reg & 0x00FF);;
	data_tx[RECPRD_NO_HI] = (uint8_t)((starting_reg >> 8) & 0x00FF);

	data_tx[RECORD_LEN_LO] = (uint8_t)(ch_size & 0x00FF);;
	data_tx[RECORD_LEN_HI] = (uint8_t)((ch_size >> 8) & 0x00FF);


	crc = CRC16(data_tx, packet_len - 2);
	data_tx[packet_len - 2] = (uint8_t)(crc & 0x00ff);
	data_tx[packet_len - 1] = (uint8_t)((crc >> 8) & 0x00ff);

	err = app_file_read(data_tx, packet_len, data_rx);

	free(data_tx);

	return err;
}


/*
 *   TODO ...
 *
 */

#define NUM_OF_RETRY_READ   ( 2 )
#define SIZE_OF_READ_CHUNK  ( 200 )
#define SIZE_OF_COMP_HEADER ( 40  )


static logfile_sm_t log_file_sm = LOGFILE_INIT;

/**
 * @brief LOG__DEVInit
 * Create a state machine to manage read log from device
 * manage the mqtt message ( ex. read file full, read file range, abort operation)
 *
 * @param  void
 * @return void
 */
void Dev_LogFile_CAREL(void)
{
  logfile_sm_t st;

  switch(log_file_sm)
  {
       default:
       case  LOGFILE_INIT:
       {
    	   log_file_sm = LOGFILE_IDLE;
    	   break;
       }

       case LOGFILE_IDLE:
       {   // do nothing...is in idle mode!!!
    	   // we are waiting for a trigger from mqtt cbor message
    	   break;
       }

       case LOGFILE_FULL:
       {
#ifdef __DEBUG_CBOR_CAREL_LEV_1
	       printf("LOG OPERATION RUNNING...\n");
#endif
    	   st = FileLog_Full_SM();

    	   if(Dev_LogFile_GetSM() != st)
    		   Dev_LogFile_SetSM(st);

    	   break;
       }

       case LOGFILE_RANGE:
       {
    	   break;
       }

       case LOGFILE_ABORT:
       {

    	   Dev_LogFile_SetSM(LOGFILE_IDLE);
    	   break;
       }

       case LOGFILE_ERR_HEADER_NOTFOUND:
       {
    	   CBOR_SendAsyncResponse(ERROR_HEAD_NOTFOUND);
    	   Dev_LogFile_SetSM(LOGFILE_IDLE);
    	   break;
       }

       case LOGFILE_ERR_HEADER_VER:
       {
    	   CBOR_SendAsyncResponse(ERROR_HEAD_VERSION);
    	   Dev_LogFile_SetSM(LOGFILE_IDLE);
    	   break;
       }

       case LOGFILE_ERR_MODBUS:
       {
    	   CBOR_SendAsyncResponse(ERROR_MODBUS);
    	   Dev_LogFile_SetSM(LOGFILE_IDLE);
    	   break;
       }

       case LOGFILE_ERR_LOCK:
       {
    	   CBOR_SendAsyncResponse(ERROR_UNLOCK_FAIL);
    	   Dev_LogFile_SetSM(LOGFILE_IDLE);
    	   break;
       }

       case LOGFILE_FINISH:
       {
			#ifdef __DEBUG_CBOR_CAREL_LEV_1
    	   	   printf("DOWNLOAD OPERATION FINISH\n");
			#endif

    	   Dev_LogFile_SetSM(LOGFILE_INIT);
    	   break;
       }
  }
}





static C_UINT16 sm_full_file = SM_READ_INIT;
static index_data_t 	   index_data = { 0 };  //                                     <--|
static const index_data_t EmptyStruct = { 0 };  // used to reset the previously struct ---|

/**
 * @brief Reset_Full_SM
 * Reset all parameters used in the state machine FileLog_Full_SM
 * Put the state machien to the initial state.
 *
 * @param  void
 * @return void
 */
void Reset_Full_SM(void)
{
//	index_data.actual_chunk  = 0;
//	index_data.last_of_chunk = 0;
//	index_data.num_of_chunk = 0;
//	index_data.num_of_files = 0;
//	index_data.pt_of_chunk = 0;
//	index_data.pt_of_files = 0;
//	index_data.pt_reg = 0;
//	index_data.total_dim_file = 0;

	index_data = EmptyStruct;
	sm_full_file = SM_READ_INIT;
}

/**
 * @brief FileLog_Full_SM
 * Create a state machine to manage read log full
 *
 * @param  void
 * @return logfile_sm_t
 */
static logfile_sm_t FileLog_Full_SM(void)
{
	// return value for the main switch
	logfile_sm_t ret = Dev_LogFile_GetSM();

	// for modbus read
	C_RES err = C_FAIL;
    C_UINT16 retry = 0;

	C_UINT16 calc_crc = 0;

	// variable useful to manage the upload from device
	CompressionHeader_t DevHeader;
	filelog_info_t      actual_data = { 0 };

	switch(sm_full_file)
	{
		default:
	    case SM_READ_INIT:
	    {
	    	//sm_full_file = SM_READ_CMP_HEADER;
	    	sm_full_file = SM_READ_SBLOCK;
	    	ret = Dev_LogFile_GetSM();
	    	break;
	    }

	    case SM_READ_SBLOCK:
	    {
	    	if(unlock_feature_control() == C_SUCCESS)
	    	{
	    	  sm_full_file = SM_READ_CMP_HEADER;
	    	  ret = Dev_LogFile_GetSM();
	    	}
	    	else
	    	{
	    		Reset_Full_SM();
	    		ret = LOGFILE_ERR_LOCK;
	    	}

	    	break;
	    }

	    case SM_READ_CMP_HEADER:
	    {
			// read compression header
			// if something goes wrong, retry 10 times
			do{
			   err = ReadDevFile(sizeof(DevHeader), file_id, 0);

			   Sys__Delay(50);
			  }while((err == C_FAIL) && (retry++ <= NUM_OF_RETRY_READ));

			if(err == C_FAIL){

				Reset_Full_SM();
				ret = LOGFILE_ERR_MODBUS;
				break;
			}

			// extract only the useful information from the modbus
			// received message (40 byte of compression header)
			memcpy(&DevHeader.comp_header[0], &data_rx[4], sizeof(DevHeader));

			// controllo versione e CRC
			calc_crc = CRC16(&(DevHeader.comp_header[0]), sizeof(DevHeader)-2);

			if(DevHeader.data.Crc != calc_crc){
				Reset_Full_SM();
				ret = LOGFILE_ERR_HEADER_NOTFOUND;
				break;
			}

			if(DevHeader.data.Version != 0x0001){
				Reset_Full_SM();
				ret = LOGFILE_ERR_HEADER_VER;
				break;
			}

			// calcolo dimensione totale del file !!!
			index_data.total_dim_file = (DevHeader.data.CompressedSize + SIZE_OF_COMP_HEADER);

			// calcolation of number of chunk (200) and number of modbus files
			index_data.num_of_chunk = (index_data.total_dim_file) / 200;

			// calcolation of the remain data
			if(((index_data.total_dim_file) % 200) != 0)
			  index_data.last_of_chunk =  (index_data.total_dim_file) - (index_data.num_of_chunk * 200);

			// calc the number of file in case the LOG is greater than 20000 bytes
			index_data.num_of_files = (index_data.total_dim_file) / 20000;

			index_data.pt_reg = 0;  		// start from 0
			sm_full_file = SM_READ_CHUNK;
	    	break;
	    }

	    case SM_READ_CHUNK:
	    {
			if(index_data.pt_of_chunk < index_data.num_of_chunk)
			{
				index_data.actual_chunk = SIZE_OF_READ_CHUNK;

				// read compression header
				// if somenthing goes wrong, retry 10 times
				do{
				   err = ReadDevFile(SIZE_OF_READ_CHUNK, (file_id + index_data.pt_of_files), (index_data.pt_reg * SIZE_OF_READ_CHUNK/2));

				   Sys__Delay(50);
				  }while((err == C_FAIL) && (retry++ <= NUM_OF_RETRY_READ));

				if(err == C_FAIL){

					Reset_Full_SM();
					ret = LOGFILE_ERR_MODBUS;
					break;
				}

				index_data.pt_reg += 1;
				index_data.pt_of_chunk++;

				if(index_data.pt_of_chunk == 100)
				{
					// we have reach the 20000 byte of first file...go to the next!!!
					index_data.pt_of_files++;
					index_data.pt_reg = 0;
				}

				sm_full_file = SM_READ_SEND;
			}
			else
				sm_full_file = SM_READ_LAST_CHUNK;

	    	break;
	    }

	    case SM_READ_LAST_CHUNK:
		{
			index_data.actual_chunk = index_data.last_of_chunk;

			do{
			   err = ReadDevFile(index_data.last_of_chunk, (file_id + index_data.pt_of_files), index_data.pt_reg);

			   Sys__Delay(50);
			  }while((err == C_FAIL) && (retry++ <= NUM_OF_RETRY_READ));

			if(err == C_FAIL){

				Reset_Full_SM();
				ret = LOGFILE_ERR_MODBUS;
				break;
			}

			index_data.pt_reg += 1;
			index_data.pt_of_chunk++;

			sm_full_file = SM_READ_SEND;
			break;
		}

	    case SM_READ_SEND:
	    {
			memcpy(&actual_data.value[0], &data_rx[4], SIZE_OF_READ_CHUNK);  // start from [4] to cut the first part of modbus msg

	    	//it is in bytes and not registry
	    	actual_data.file_start = (index_data.pt_of_files * 20000) + ((index_data.pt_reg - 1)*SIZE_OF_READ_CHUNK);
	    	actual_data.file_chunk_len = index_data.actual_chunk;    // 200 or less if is the last chunk to send!!!
	    	actual_data.file_size = index_data.total_dim_file;

	    	// connessione??? da gestire
	    	CBOR_SendAsync_FileLog(actual_data);

	    	// check if file ended
	    	if(index_data.pt_of_chunk > index_data.num_of_chunk)
	    	{
	    		// end of file
	    		sm_full_file = SM_READ_FINISH;
	    	}else
	    	    sm_full_file = SM_READ_CHUNK;

	    	break;
	    }

	    case SM_READ_FINISH:
	    {
	    	Reset_Full_SM();
	    	ret = LOGFILE_FINISH;
	    	break;
	    }
	}
	return ret;
}




/**
 * @brief Dev_LogFile_SetSM
 *        set state machine Log Device
 *
 * @param  logfile_sm_t log_state_m
 * @return void
 */
void Dev_LogFile_SetSM(logfile_sm_t log_state_m){
	log_file_sm = log_state_m;
}

/**
 * @brief Dev_LogFile_GetSM
 *       return Log Device state machine
 *
 * @param  none
 * @return log_sm_t
 */
logfile_sm_t Dev_LogFile_GetSM(void){
    return log_file_sm;
}


/**
 * @brief RetriveFileLog_Info
 *        Retrieve the info useful to read the correct file
 *        via modbus. This function is call inside the CBOR handle (CBOR_ReqTopicParser)
 *
 *        ex. if we have to read full file we have only the file index
 *
 *        ex. if we have to read range file we have all the three parameters
 *            (file index, start and length)
 *
 * @param  c_cborreqfilelog data
 * @return void
 */
void RetrieveFileLog_Info(c_cborreqfilelog data)
{
	file_id    = data.fid;
	file_start = data.fle;
	file_len   = data.fst;
}








