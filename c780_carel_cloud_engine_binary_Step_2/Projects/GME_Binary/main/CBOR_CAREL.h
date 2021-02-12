/**
 * @file CBOR_CAREL.h
 * @author Carel
 * @date 12 Sep 2019
 * @brief Carel CBOR glue logic
 * Functions performing encoding need a payload buffer to be passed as reference in order to be filled with required info.
 * This buffer needs to be previously statically allocated (with proper size)
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CBOR_CAREL_H
#define __CBOR_CAREL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "data_types_CAREL.h"
#include "CAREL_GLOBAL_DEF.h"
#include "tinycbor/cbor.h"
#include "binary_model.h"
#include "polling_CAREL.h"

#include "filelog_CAREL.h"

/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

 //this define enable the output of the errors
#define __DEBUG_CBOR_CAREL_LEV_1

//this define enable the output of others debug informations
#define __DEBUG_CBOR_CAREL_LEV_2

//this define enable the output of others debug informations
//this option enable the cbor packet dump so a lot of serial traffic
//#define __DEBUG_CBOR_CAREL_LEV_3

// the define __DEBUG_CBOR_CAREL_LEV_4 enable DEBUG_ENC/DEBUG_ADD/DEBUG_DEC output
//#define __DEBUG_CBOR_CAREL_LEV_4
#endif


/* Exported constants --------------------------------------------------------*/

#define CBORSTREAM_SIZE			1024

#define TAG_SIZE				3

#define RESPONSE_SIZE			256
#define ALIAS_SIZE				5
#define VAL_SIZE				10
#define A_SIZE					30
#define B_SIZE					30

#define REPORT_SLAVE_ID_SIZE	256
#define ADU_SIZE				512

#define HEADERREQ_LEN			55			// header of request has fixed size

enum CBOR_CmdResponse{
	INVALID_CMD = -1,
	SUCCESS_CMD,
	ERROR_CMD,
	ERROR_HEAD_NOTFOUND,
	ERROR_HEAD_VERSION,
	ERROR_MODBUS,
	ERROR_ALREADY_RUN,
	ERROR_UNLOCK_FAIL,
	ERROR_NO_ONGOING
};

typedef enum{
	mbR_COIL = 1,
	mbR_DI,
	mbR_HR,
	mbR_IR,
	mbW_COIL,
	mbW_HR,
	mbW_COILS = 15,
	mbW_HRS = 16
}mb_Functions_t;

typedef enum CloudtoGME_Commands_l{
	NO_COMMAND = 0,
	SET_GW_CONFIG = 1,
	REBOOT,
	SCAN_DEVICES,
	SET_LINES_CONFIG,
	SET_DEVS_CONFIG,
	READ_VALUES,
	WRITE_VALUES = 7,
	UPDATE_GME_FIRMWARE=10,
	UPDATE_DEV_FIRMWARE,
	FLUSH_VALUES,
	UPDATE_CA_CERTIFICATES,
	SEND_MB_ADU,
	SEND_MB_PASS_THROUGH,
	CHANGE_CREDENTIALS,
	START_ENGINE,
	STOP_ENGINE,
	FULL_DOWNLOAD,
	RANGE_DOWNLOAD,
	ABORT_DOWNLOAD,
	UPDATE_FILE = 50,
}cloud_req_commands_t;


#ifdef __DEBUG_CBOR_CAREL_LEV_4

#define DEBUG_ENC(err, a) if(err != CborNoError) printf("%s: %s %s, error %d\n", __func__, "cannot encode", a, err);
#define DEBUG_ADD(err, a) if(err != CborNoError) printf("%s: %s %s, error %d\n", __func__, "cannot add", a, err);
#define DEBUG_DEC(err, a) if(err != CborNoError) printf("%s: %s %s, error %d\n", __func__, "cannot decode", a, err);

#else

#define DEBUG_ENC(err, a)
#define DEBUG_ADD(err, a)
#define DEBUG_DEC(err, a)

#endif

/* Exported types ------------------------------------------------------------*/

/**
 * @brief C_CBORHREQ
 *
 * Header of a request/response
 * In case of a request, the last element is not populated
 */
#pragma pack(1)
typedef struct C_CBORHREQ{
	C_UINT16 ver;
	C_BYTE rto[RTO_SIZE];
	C_UINT16 cmd;
	C_INT16 res;
} c_cborhreq;
#pragma pack()

/**
 * @brief C_CBORREQRDWRVALUES
 *
 * Read/write values request (without header)
 */
#pragma pack(1)
typedef struct C_CBORREQRDWRVALUES{
	C_CHAR alias[ALIAS_SIZE];
	C_CHAR val[VAL_SIZE];
	C_UINT16 func;
	C_UINT16 addr;
	C_UINT16 dim;
	C_UINT16 pos;
	C_UINT16 len;
	C_CHAR a[A_SIZE];
	C_CHAR b[B_SIZE];
	flag_t flags;

}c_cborreqrdwrvalues;
#pragma pack()


/**
 * @brief C_CBORREQSETGWCONFIG
 *
 * Request set gw config
 */
#pragma pack(1)
typedef struct C_CBORREQSETGWCONFIG{
	C_UINT16 pva;		// values message will be sent every pva seconds
	C_UINT16 pst;		// status message will be sent every pst seconds
	C_UINT16 mka;		// mqtt keep alive interval
	C_UINT16 lss;		// low speed sampling period
	C_UINT16 hss;		// high speed sampling period
}c_cborreqsetgwconfig;
#pragma pack()

/**
 * @brief C_CBORREQDWLDEVSCONFIG
 *
 * Request set gw config
 */
#pragma pack(1)
typedef struct C_CBORDWLDEVSCFG{
	C_USERNAME usr;
	C_PASSWORD pwd;
	C_URI uri;
	C_UINT16 cid;
	C_UINT16 crc;
	C_UINT16 dev;
	C_UINT16 did;
	C_CHAR fil[FIL_SIZE];
}c_cborreqdwldevsconfig;
#pragma pack()



/**
 * @brief C_CBORREQUPDDEVFW
 *
 * Request
 */
#pragma pack(1)
typedef struct C_CBORREQUPDDEVFW{
	C_USERNAME usr;
	C_PASSWORD pwd;
	C_URI uri;
	C_UINT16 fid;
	C_UINT16 wet;
	C_UINT16 cid;
}c_cborrequpddevfw;
#pragma pack()

/**
 * @brief C_CBORREQLINESCONFIG
 *
 * Request set lines config
 */
#pragma pack(1)
typedef struct C_CBORLINESCFG{
	C_UINT32 baud;
	C_BYTE conn;
	C_UINT16 del;
}c_cborreqlinesconfig;
#pragma pack()

#pragma pack(1)
typedef struct C_CBORSENDMBADU{
	uint16_t sequence;
	uint8_t *adu;
	uint8_t adu_len;
}c_cbor_send_mb_adu;
#pragma pack()

/**
 * @brief C_CBORALARMS
 *
 * Alarms
 */
#pragma pack(1)
typedef struct C_CBORALARMS{
	C_BYTE aty;
	C_CHAR ali[ALIAS_SIZE];
	C_BYTE aco;
	C_TIME st;
	C_TIME et;
}c_cboralarms;
#pragma pack()


/*
 *
 *   STEP 2
 *
 * */

///*
// * @brief C_CBORREQFILELOG
// *
// * full log/file
// * */
#pragma pack(1)
typedef struct C_CBORREQFILELOG{
	uint16_t fid;
	C_UINT32 fst;
	C_UINT32 fle;
}c_cborreqfilelog;
#pragma pack()


/*
 * @brief C_CBORFILEVALUE
 *
 * VALUE log/file
 * */
#pragma pack(1)
typedef struct C_CBORFILEVALUE{
	C_UINT32 fsz;
	C_UINT32 fst;
	C_UINT32 fle;
	C_BYTE ans[400];
}c_cborfilevalue;
#pragma pack()




/*----------------------------------------------------------------------------------------*/
size_t CBOR_Alarms(C_CHAR* cbor_stream, c_cboralarms cbor_alarms);
void CBOR_SendAlarms(c_cboralarms cbor_alarms);
size_t CBOR_Hello(C_CHAR* cbor_stream);
void CBOR_SendHello(void);
size_t CBOR_Status(C_CHAR* cbor_stream);
void CBOR_SendStatus(void);
size_t CBOR_Values(C_CHAR* cbor_stream, C_UINT16 index, C_UINT16 number, C_INT16 frame);
void CBOR_SendFragmentedValues(C_UINT16 index, C_UINT16 number);
void CBOR_SendValues(C_UINT16 index, C_UINT16 number, C_INT16 frame);
void CBOR_SendMobile(void);
size_t CBOR_Mobile(C_CHAR* cbor_stream);
size_t CBOR_Connected(C_CHAR* cbor_stream, C_UINT16 cbor_status);

void CBOR_ResHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req, CborEncoder* encoder, CborEncoder* mapEncoder);
size_t CBOR_ResSimple(C_CHAR* cbor_response, c_cborhreq* cbor_req);
size_t CBOR_ResScanLine(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 device, C_BYTE* answer, C_UINT16 answer_len);
size_t CBOR_ResSendMbAdu(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 seq, C_BYTE* val, C_UINT16 val_len);
size_t CBOR_ResRdWrValues(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_CHAR* ali, C_CHAR* val);
size_t CBOR_ResSendMbPassThrough(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 cbor_pass);
size_t CBOR_ResSetDevsConfig(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 did);

CborError CBOR_ReqHeader(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborhreq* cbor_req);
CborError CBOR_ReqSetLinesConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqlinesconfig *set_line_cfg);
CborError CBOR_ReqSetDevsConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqdwldevsconfig* download_devs_config);
CborError CBOR_ReqRdWrValues(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqrdwrvalues* cbor_wv);
CborError CBOR_ReqSetGwConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqsetgwconfig* cbor_setgwconfig);
CborError CBOR_ReqScanLine(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16 *device);
CborError CBOR_ReqSendMbAdu(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16* seq, C_CHAR* adu);
CborError CBOR_ReqReboot(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16* cid);
CborError CBOR_ReqSendMbPassThrough(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16* cbor_pass);

// step 2
CborError CBOR_ReqFileLog(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqfilelog* RangeFileData);
C_INT32 CBOR_SendAsync_FileLog(filelog_info_t data);

#define CBOR_ReqRangeFile CBOR_ReqFileLog
#define CBOR_ReqFullFile  CBOR_ReqFileLog
typedef c_cborreqfilelog  c_cborreqfullfile;
typedef c_cborreqfilelog  c_cborreqrangefile;


void RetrieveFileLog_Info(c_cborreqfilelog data);

int CBOR_ReqTopicParser(C_CHAR* cbor_stream, C_UINT16 cbor_len);
CborError CBOR_DiscardElement(CborValue* recursed);
CborError CBOR_ExtractInt(CborValue* recursed, int64_t* read);

#define 	CBOR_ReqUpdateCaCertificate 	CBOR_ReqSetDevsConfig
#define 	CBOR_ReqChangeCredentials		CBOR_ReqSetDevsConfig
typedef 	c_cborreqdwldevsconfig			c_cborrequpdatecacert;
typedef		c_cborreqdwldevsconfig			c_cborreqchangecred;
typedef 	c_cborrequpddevfw				c_cborrequpdgmefw;
typedef		c_cborreqdwldevsconfig			c_cborrequpdatefile;
#define 	CBOR_ReqUpdateGMEFW 			CBOR_ReqUpdateDevFW
#define		CBOR_ReqUpdateFile				CBOR_ReqSetDevsConfig


C_RES execute_set_line_config(c_cborreqlinesconfig set_line_cfg);
C_RES execute_set_gw_config(c_cborreqsetgwconfig set_gw_config );
C_RES execute_change_cred(c_cborreqdwldevsconfig change_cred);
C_RES execute_scan_devices(C_BYTE* data_rx, C_UINT16 *add, C_INT16 * lnt);
C_RES execute_update_file(c_cborrequpdatefile *update_file);
C_RES parse_write_values(c_cborreqrdwrvalues cbor_wv);
C_RES parse_read_values(c_cborreqrdwrvalues* cbor_rv);

void CBOR_ReadDidFromNVM (void);
C_UINT16 CBOR_GetDid (void);

//long double read_values_conversion(hr_ir_low_high_poll_t *hr_to_read);
void Manage_Report_SlaveId_CAREL( C_CHAR * pucFrame, C_UINT16 * usLen);
void CBOR_SaveAsyncRequest(c_cborhreq cbor_req, C_UINT16 cid);
void CBOR_SendAsyncResponse(C_INT16 res);
void CBOR_SendAsyncResponseDid(C_INT16 res, C_UINT16 did);
#ifdef __cplusplus
}
#endif

#endif
