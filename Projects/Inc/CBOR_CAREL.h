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
#define REPLYTO_SIZE			32

#define CMD_SCAN_LINE_RES		TODO
#define CMD_SEND_MB_ADU			TODO
#define CMD_SET_DEVS_CONFIG		TODO
#define CMD_SET_LINES_CONFIG		TODO
#define CMD_WRITE_VALUES		TODO
#define CMD_READ_VALUES			TODO

const char cannot_encode[]="cannot encode";
const char cannot_add[]="cannot add";

#define CAREL_DEBUG

#ifdef CAREL_DEBUG
#define DEBUG_ENC(err, a) if (err != CborNoError) printf("%s: %s %s, error %d\n", __func__, cannot_encode, a, err);
#define DEBUG_ADD(err, a) if (err != CborNoError) printf("%s: %s %s, error %d\n", __func__, cannot_add, a, err);
#else
#define DEBUG_ENC(err, a)
#define DEBUG_ADD(err, a)
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
	C_BYTE replyTo[REPLYTO_SIZE];
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
	C_BYTE replyTo[REPLYTO_SIZE];
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
void CBOR_Status(C_CHAR* cbor_stream);
void CBOR_Values(C_CHAR* cbor_stream);
void CBOR_Mobile(C_CHAR* cbor_stream);

void CBOR_Response(C_CHAR* cbor_stream, c_cborhreq* cbor_req, C_INT16 result);
void CBOR_Header(C_CHAR* cbor_stream);
void CBOR_WriteValues(C_CHAR* cbor_stream);
void CBOR_ScanLine(C_CHAR* cbor_stream);
void CBOR_MbAdu(C_CHAR* cbor_stream);
void CBOR_SetDevsConfig(C_CHAR* cbor_stream);
void CBOR_SetLinesConfig(C_CHAR* cbor_stream);
#ifdef __cplusplus
}
#endif

#endif
