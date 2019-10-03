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
#include "cbor.h"

/* Exported constants --------------------------------------------------------*/

#define CBORSTREAM_SIZE			1024

#define TAG_SIZE				3
#define REPLYTO_SIZE			48
#define RESPONSE_SIZE			80

#define CMD_SCAN_LINE_RES		TODO
#define CMD_SEND_MB_ADU			TODO
#define CMD_SET_DEVS_CONFIG		TODO
#define CMD_SET_LINES_CONFIG		TODO
#define CMD_WRITE_VALUES		TODO
#define CMD_READ_VALUES			TODO

#define HEADERREQ_LEN			55			// header of request has fixed size

#define	INVALID_CMD				-1

typedef enum CloudtoGME_Commands_l{
	SET_GW_CONFIG = 1,
	REBOOT,
	SCAN_DEVICES,
	SET_LINES_CONFIG,
	DOWNLOAD_DEVS_CONFIG,
	READ_VALUES,
	WRITE_VALUES = 9,
	UPDATE_GME_FIRMWARE,
	UPDATE_DEV_FIRMWARE,
	FLUSH_VALUES,
	UPDATE_CA_CERTIFICATES,
	SEND_MB_ADU,
	CHANGE_CREDENTIALS,
	START_ENGINE,
	STOP_ENGINE,
}cloud_req_commands_t;


#define CAREL_DEBUG

const char cannot_encode[]="cannot encode";
const char cannot_add[]="cannot add";
const char cannot_decode[]="cannot decode";

#ifdef CAREL_DEBUG
#define DEBUG_ENC(err, a) if (err != CborNoError) printf("%s: %s %s, error %d\n", __func__, cannot_encode, a, err);
#define DEBUG_ADD(err, a) if (err != CborNoError) printf("%s: %s %s, error %d\n", __func__, cannot_add, a, err);
#define DEBUG_DEC(err, a) if (err != CborNoError) printf("%s: %s %s, error %d\n", __func__, cannot_decode, a, err);
#else
#define DEBUG_ENC(err, a)
#define DEBUG_ADD(err, a)
#define DEBUG_DEC(err, a)
#endif

/* Exported types ------------------------------------------------------------*/ 

/**
 * @brief C_CBORHREQ
 *
 * Header of a request
 */
#pragma pack(1)
typedef struct C_CBORHREQ{
	C_UINT16 ver;
	C_BYTE rto[REPLYTO_SIZE];
	C_UINT16 cmd;	
} c_cborhreq;
#pragma pack()
/**
 * @brief C_CBORHRES
 *
 * Header of a response
 */
#pragma pack(1)
typedef struct C_CBORHRES{
	C_UINT16 ver;
	C_BYTE rto[REPLYTO_SIZE];
	C_UINT16 cmd;
	C_INT16 res;
}c_cborhres;
#pragma pack()
/**
 * @brief C_CBORHRES
 *
 * Response to a write values (without header)
 */
#pragma pack(1)
typedef struct C_CBORRESWRITEVALUES{
	int todo;
}c_cborreswritevalues;
#pragma pack()

/*----------------------------------------------------------------------------------------*/
void CBOR_Alarms(C_UINT16 alias, C_TIME tstart, C_TIME tstop, C_BYTE alarm_issue, C_CHAR* cbor_stream);
size_t CBOR_Hello(C_CHAR* cbor_stream);
size_t CBOR_Status(C_CHAR* cbor_stream);
void CBOR_Values(C_CHAR* cbor_stream);
size_t CBOR_Mobile(C_CHAR* cbor_stream);

CborError CBOR_ReqHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req, CborValue* it, CborValue* recursed);
CborError CBOR_ReqSetLinesConfig(CborValue* recursed, C_UINT32* new_baud_rate, C_BYTE* new_connector);

int CBOR_ReqTopicParser(C_CHAR* cbor_stream, C_UINT16 cbor_len);

void CBOR_Response(C_CHAR* cbor_stream, c_cborhreq* cbor_req, C_INT16 result);
void CBOR_Header(C_CHAR* cbor_stream);
void CBOR_WriteValues(C_CHAR* cbor_stream);
void CBOR_ScanLine(C_CHAR* cbor_stream);
void CBOR_MbAdu(C_CHAR* cbor_stream);
void CBOR_SetDevsConfig(C_CHAR* cbor_stream);
void CBOR_SetLinesConfig(C_CHAR* cbor_stream);

void CBOR_ResHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req, CborEncoder* encoder, CborEncoder* mapEncoder);
size_t CBOR_ResSimple(C_CHAR* cbor_stream, c_cborhreq* cbor_req, C_INT16 res);


#ifdef __cplusplus
}
#endif

#endif
