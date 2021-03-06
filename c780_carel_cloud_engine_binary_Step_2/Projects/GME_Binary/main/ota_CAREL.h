/**
 * @file   ota_CAREL.h
 * @author carel
 * @date   30 Oct 2019
 * @brief  functions to managment the update of the FW of the attached device 
 *         via HTTPS, this is the platform independent part
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OTA_CAREL_
#define __OTA_CAREL_

/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

 //this define enable the output of the errors
//#define __DEBUG_OTA_CAREL_LEV_1

//#define __DEBUG_OTA_CAREL_LEV_2


#endif
/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */


/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */
#pragma pack(1)
typedef struct mb_write_file_record_s {
	uint8_t 	func_code;
	uint8_t 	req_data_len;
	uint8_t 	ref_type;
	uint16_t 	file_number;
	uint16_t	record_number;
	uint16_t	record_length;
}mb_write_file_record_t;
#pragma pack()


enum{
	SLAVE_ADD = 0,
	FUNC_CODE,
	REQ_DATA_LEN,
	REF_TYPE,

	FILE_NO_HI,
	FILE_NO_LO,

	RECPRD_NO_HI,
	RECORD_NO_LO,

	RECORD_LEN_HI,
	RECORD_LEN_LO,

	MAX_HEADER_BYTES
};


#define OTA_IDLE 0
#define OTA_RUNNING 1


/** @brief  DEV_OTA_BUF_SIZE
*   The maximum allowed packet's size to be sent through modbus is 253 bytes,
*	so 253(total) - 9(write file cmd header) - 2(CRC) = 242
* 
*   @note  this amount of bytes will be allocate on the stack. 
*          the packet chunck is a little bit small than the maximum and rounded to 200
*          because the CAREL MFT file is 20K so 100 blocks
*
*/
#define DEV_OTA_BUF_SIZE  200
//238

/** @brief MB_FILE_MAX_BYTES this define is the size of a modbus file 
 *        in the implemented way, this does't mean that we allocate this 
 *        amount of bytes 
*/
#define MB_FILE_MAX_BYTES 20000

/* ========================================================================== */
/* Functions prototypes                                                       */
/* ========================================================================== */
void dbg_Set_OTA_Status(C_BYTE status);
void dbg_Set_OTA_Trasf_len(C_UINT32 len);
C_BYTE dbg_Get_OTA_Status(void);
C_UINT32 dbg_Get_OTA_Content_Lenght(void);

C_RES OTA__DevFWUpdate(c_cborrequpddevfw *dev_fw_config);

//void DEV_ota_task(void * pvParameter);

void DEV_ota_range_task(void * pvParameter);

C_RES UpdateDevFirmware(C_BYTE *fw_chunk, C_UINT16 ch_size, C_UINT16 file_no, C_UINT16 starting_reg);

void GME_ota_task(void * pvParameter);
void CA_ota_task(void * pvParameter);
void Model_ota_task(void * pvParameter);

#endif /* __OTA_CAREL_ */
