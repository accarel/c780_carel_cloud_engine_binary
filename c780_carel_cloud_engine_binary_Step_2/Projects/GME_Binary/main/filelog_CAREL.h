/**
 * @file log_CAREl.h
 * @author  CAREL
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the LOG (STone way)
 */


#ifndef LOG_CAREl_H_
#define LOG_CAREl_H_

#define GUID_SIZE  		( 16 )
#define COMPHEADER_SIZE ( 40 )

#pragma pack(1)
typedef union VM_CompressionHeader
{
	uint8_t comp_header[COMPHEADER_SIZE];
	struct {
	uint16_t Version;
	uint8_t ProjectId[GUID_SIZE];
	uint32_t SizePayload;
	uint32_t CrcPayload;
	uint16_t CompressionType;
	uint16_t FileType;
	uint32_t CompressedSize;
	uint8_t Flags[2];
	uint16_t Spare;
	uint16_t Crc;
	}data;
} CompressionHeader_t;
#pragma pack()


#pragma pack(1)
typedef struct indx_data{
	uint32_t total_dim_file;
	uint8_t  actual_chunk;
	uint32_t num_of_chunk;
	uint32_t pt_of_chunk;
	uint32_t last_of_chunk;
	uint32_t num_of_files;
	uint32_t pt_of_files;
	uint32_t pt_reg;
	//char     data[200];   // TODO...remove, we use the final struct!!!
} index_data_t;
#pragma pack()

// by Mattia Bacchin from OS project
//
//typedef struct _CompressionHeader
//{
//unsigned short version;
//unsigned char projectId[16];
//unsigned long sizePayload;
//unsigned long crcPayload;
//unsigned short compressionType;
//unsigned short fileType;
//unsigned long compressedSize;
//unsigned char flags[2];
//unsigned short spare;
//unsigned short crc;
//} CompressionHeader;
//

#define MAX_FILE_LEN (200)

#pragma pack(1)
typedef struct filelog_info{
	C_UINT16 file_reg;				// register to read
	C_UINT32 file_size;				// total length
	C_UINT32 file_start;			// file start offset
	C_UINT32 file_chunk_len;			// chunk length
	C_BYTE  value[MAX_FILE_LEN];	// data
} filelog_info_t;
#pragma pack()


/**
 * @brief log_sm enum
 *        GME log state machine status
 */
typedef enum logfile_sm{
	LOGFILE_INIT = 0,
	LOGFILE_IDLE,
	LOGFILE_FULL,
	LOGFILE_RANGE,
	LOGFILE_ABORT,
	LOGFILE_ERR_HEADER_NOTFOUND,
	LOGFILE_ERR_HEADER_VER,
	LOGFILE_ERR_MODBUS,
	LOGFILE_ERR_LOCK,
	LOGFILE_FINISH
}logfile_sm_t;

void Dev_LogFile_CAREL(void);
void Dev_LogFile_SetSM(logfile_sm_t log_state_m);
logfile_sm_t Dev_LogFile_GetSM(void);

int GetActualMsgId(void);


void Reset_Full_SM(void);

#endif /* LOG_CAREl_H_ */
