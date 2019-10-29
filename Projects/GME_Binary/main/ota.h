/*
 * ota.h
 *
 *  Created on: Sep 24, 2019
 *      Author: ataayoub
 */

#ifndef MAIN_OTA_H_
#define MAIN_OTA_H_
#include"common.h"
#include"js_types.h"

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

	MAX_HEADER_BYTES,
};




void OTA__GMEInit(void);
esp_err_t OTA__DevFWInit(req_update_dev_fw_t *dev_fw_config);


#endif /* MAIN_OTA_H_ */
