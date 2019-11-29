/**
 * @file CBOR_CAREL.c
 * @author Carel
 * @date 12 Sep 2019
 * @brief Carel CBOR glue logic
 * Functions performing encoding need a payload buffer to be passed as reference in order to be filled with required info.
 * This buffer needs to be previously statically allocated (with proper size)
 *
 */
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

#include "modbus_IS.h"

#include "CBOR_CAREL.h"
#include "File_System_CAREL.h"
#include "tinycbor/cbor.h"
#include "Miscellaneous_IS.h"
#include "RTC_IS.h"
#include "polling_CAREL.h"
#include "polling_IS.h"
#include "MQTT_Interface_IS.h"
#include "MQTT_Interface_CAREL.h"
#include "nvm.h"
#include "binary_model.h"
#include "https_client_CAREL.h"
#include "ota_CAREL.h"
#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "mb_m.h"
#endif

/* Exported types ------------------------------------------------------------*/
extern  CHAR     ucMBSlaveID[256];
extern  USHORT   usMBSlaveIDLen;


/* Exported constants --------------------------------------------------------*/

C_CHAR txbuff[1000];



/**
 * @brief CBOR_SendAlarms
 *
 * Prepares and sends an MQTT message containing a CBOR encoded message with alarms
 *
 * @param Pointer to a struct containing alarms info
  * @return void
 */
void CBOR_SendAlarms(c_cboralarms cbor_alarms)
{
	size_t len = CBOR_Alarms(txbuff, cbor_alarms);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/alarms"), (C_SBYTE*)txbuff, len, 0, 0);
}

/**
 * @brief CBOR_Alarms
 *
 * Prepares CBOR encoded message containing information on alarms occurred in the interval from st till et
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Structure containing alarms info
 * @return void
 */
size_t CBOR_Alarms(C_CHAR* cbor_stream, c_cboralarms cbor_alarms)
{
	CborEncoder encoder, mapEncoder;
	size_t len;
	int err;

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "alarms create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode aty - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "aty");
	err |= cbor_encode_uint(&mapEncoder, cbor_alarms.aty);
	DEBUG_ADD(err, "aty");

	// encode ali - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "ali");
	err |= cbor_encode_text_stringz(&mapEncoder, (char*)cbor_alarms.ali);
	DEBUG_ADD(err, "ali");

	// encode aco - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "aco");
	err |= cbor_encode_uint(&mapEncoder, cbor_alarms.aco);
	DEBUG_ADD(err, "aco");

	// encode st - elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "st");
	err |= cbor_encode_uint(&mapEncoder, cbor_alarms.st);
	DEBUG_ADD(err, "st");

	// encode et - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "et");
	err |= cbor_encode_uint(&mapEncoder, cbor_alarms.et);
	DEBUG_ADD(err, "et");

	// encode dev - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "dev");
	C_UINT16 device = 0;
	NVM__ReadU32Value(MB_DEV_NVM, (C_UINT32*)&device);
	err |= cbor_encode_int(&mapEncoder, device);
	DEBUG_ADD(err, "dev");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	printf("alarmspkt len %d: \n", len);
	for (int i=0;i<len;i++){
		printf("%02X ", cbor_stream[i]);
	}
	printf("\n");

	return len;
}

/**
 * @brief CBOR_SendHello
 *
 * Prepares and sends an MQTT message containing the hello message
 *
 * @param none
 * @return void
 */
void CBOR_SendHello(void)
{
	size_t len = CBOR_Hello(txbuff);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/hello"), (C_SBYTE*)txbuff, len, 1, 1);
}

/**
 * @brief CBOR_Hello
 *
 * Prepares CBOR encoded message containing hello information
 *
 * @param Pointer to the CBOR-encoded payload
 * @return size of the encoded stream, returns -1 in case something's gone worng while encoding
 */

size_t CBOR_Hello(C_CHAR* cbor_stream)
{
	CborEncoder encoder, mapEncoder;
	size_t len;
	int err;

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "hello create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode pn - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "pn");
	err |= cbor_encode_text_stringz(&mapEncoder, GW_PARTNUMBER);
	DEBUG_ADD(err, "part number");

	// encode hwv - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "hwv");
	err |= cbor_encode_uint(&mapEncoder, GW_HW_REV);
	DEBUG_ADD(err, "hw version");

	// encode fwv - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "fwv");
	err |= cbor_encode_uint(&mapEncoder, GW_FW_REV);
	DEBUG_ADD(err, "fw version");

	// encode btm - elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "btm");
	err |= cbor_encode_uint(&mapEncoder, RTC_Get_UTC_Boot_Time());
	DEBUG_ADD(err, "boot time");

	// encode bau - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "bau");
	C_UINT32 baud_rate;
	if(C_SUCCESS == NVM__ReadU32Value(MB_BAUDRATE_NVM, &baud_rate)){
    	err |= cbor_encode_uint(&mapEncoder, baud_rate);
    }else{
    	err |= cbor_encode_uint(&mapEncoder, 0);
    }
	DEBUG_ADD(err, "baud rate");

	// encode mqv - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "mqv");
	C_BYTE MQTTver[3] = {3, 1, 1};							// todo maybe the version could be found somewhere...
	err |= cbor_encode_byte_string(&mapEncoder, MQTTver, 3);
	DEBUG_ADD(err, "mqttversion");

	// encode dev - elem8
	err |= cbor_encode_text_stringz(&mapEncoder, "dev");
	C_UINT16 device_address = 0;
	if(C_SUCCESS == NVM__ReadU32Value(MB_DEV_NVM, (C_UINT32*)&device_address)){
		err |= cbor_encode_uint(&mapEncoder, device_address);
	}else{
		err |= cbor_encode_uint(&mapEncoder, 0);
	}
	DEBUG_ADD(err, "dev");

	// encode gid - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "gid");
	err |= cbor_encode_byte_string(&mapEncoder, BinaryModel__GetHeaderptr(), 16);
	DEBUG_ADD(err, "guid");

	//encode crc - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "crc");
	C_UINT16 crc = 0;													// to be implemented
	err |= cbor_encode_uint(&mapEncoder, crc);
	DEBUG_ADD(err, "crc");

	//encode cid - elem11
	err |= cbor_encode_text_stringz(&mapEncoder, "cid");
	C_UINT16 cid=0;
	if(C_SUCCESS == NVM__ReadU32Value(MB_CID_NVM, (C_UINT32*)&cid)){
	   	err |= cbor_encode_uint(&mapEncoder, cid);
	}else{
	   	err |= cbor_encode_uint(&mapEncoder, 0);
	}
	// reset cid
	NVM__WriteU32Value(MB_CID_NVM, 0);
	DEBUG_ADD(err, "cid");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }
	//for(int i=0; i<len; i++)
	//printf("%x\n", cbor_stream[i]);
	return len;
}

/**
 * @brief CBOR_SendStatus
 *
 * Prepares and sends an MQTT message containing the status message
 *
 * @param none
 * @return void
 */
void CBOR_SendStatus(void)
{
	size_t len = CBOR_Status(txbuff);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/status"), (C_SBYTE*)txbuff, len, 0, 0);
}

/**
 * @brief CBOR_Status
 *
 * Prepares CBOR encoded message containing status information
 *
 * @param Pointer to the CBOR-encoded payload
 * @return size of the encoded stream, returns -1 in case something's gone wrong while encoding
 */
size_t CBOR_Status(C_CHAR* cbor_stream)
{
	CborEncoder encoder, mapEncoder;
	size_t len;
	int err;

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "status create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode t - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
	C_TIME t = RTC_Get_UTC_Current_Time();
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "t");

	// encode upt - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "upt");
	err |= cbor_encode_uint(&mapEncoder, t - RTC_Get_UTC_Boot_Time());
	DEBUG_ADD(err, "upt");

	// encode fme -elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "fme");
	C_UINT32 freemem = 2365; 												// to be implemented
	err |= cbor_encode_uint(&mapEncoder, freemem);
	DEBUG_ADD(err,"upt");

	// encode est -elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "est");
	err |= cbor_encode_uint(&mapEncoder, PollEngine_GetEngineStatus_CAREL());
	DEBUG_ADD(err,"est");

	// encode sgn -elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "sgn");
	C_INT16 rssi = -47;													// to be implemented
	err |= cbor_encode_int(&mapEncoder, rssi);
	DEBUG_ADD(err,"sgn");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}


static C_UINT32 pkt_cnt = 0;

/**
 * @brief CBOR_SendValues
 *
 * Prepares and sends an MQTT message containing the values message
 *
 * @param index, index in values table of the first value to be sent
 * @param number, number of values that must be sent
 * @param frame, frame number of the packet
 * @return void
 */
void CBOR_SendValues(C_UINT16 index, C_UINT16 number, C_INT16 frame)
{
	C_CHAR mybuf[500];

	size_t len = CBOR_Values(mybuf, index, number, frame);

#if 0
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')
#endif

	printf("valuespkt len %d: \n", len);
	for (int i=0;i<len;i++){
		printf("%02X ", mybuf[i]);
	}
	printf("\n");
#if 0
	printf("values pkt binary: \n");
	for (int i=0;i<len;i++){
			printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(mybuf[i]));
	}
	printf("\n");
#endif

	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/values"), (C_SBYTE*)mybuf, len, 0, 0);
}

/**
 * @brief CBOR_Values
 *
 * Prepares CBOR encoded message containing variable values
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Index of the first entry in table containing changed values to be sent
 * @param Number of entries of the table containing changed values that must be sent
 * @param Number of frame to be written in packet
 * @return size of the encoded stream, returns -1 in case something's gone wrong while encoding
 */
size_t CBOR_Values(C_CHAR* cbor_stream, C_UINT16 index, C_UINT16 number, C_INT16 frame)
{
	CborEncoder encoder, mapEncoder, mapEncoder1;
	size_t len;
	CborError err;

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, 7);
	DEBUG_ENC(err, "values create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode cnt - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "cnt");
	err |= cbor_encode_uint(&mapEncoder, pkt_cnt++);
	DEBUG_ADD(err, "cnt");

	// encode btm - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "btm");
	C_TIME t = RTC_Get_UTC_Boot_Time();
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "btm");

	// encode t - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
	// if there's no change to notify, update t at current time
	if (number == 0)
		t = RTC_Get_UTC_Current_Time();
	else
		t = Get_SamplingTime(index);
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "t");

	// encode vls - elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "vls");
	// map vals
	err = cbor_encoder_create_map(&mapEncoder, &mapEncoder1, CborIndefiniteLength);
	DEBUG_ENC(err, "vals create map");
	for (C_UINT16 i = 0; i < number; i++){
		err |= cbor_encode_text_stringz(&mapEncoder1, (char*)Get_Alias(index, i));
		if (memcmp((char*)Get_Value(index, i), "", sizeof("")) == 0)
			err |= cbor_encode_null(&mapEncoder1);
		else
			err |= cbor_encode_text_stringz(&mapEncoder1, (char*)Get_Value(index, i));
	}
	err |= cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	// encode frm - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "frm");
	err |= cbor_encode_int(&mapEncoder, frame);
	DEBUG_ADD(err, "frm");

	// encode dev - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "dev");
	C_UINT16 device = 0;
	NVM__ReadU32Value(MB_DEV_NVM, (C_UINT32*)&device);
	err |= cbor_encode_int(&mapEncoder, device);
	DEBUG_ADD(err, "dev");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_SendFragmentedValues
 *
 * Prepares CBOR encoded message fragmented into multiple pieces
 *
 * @param Index of the first entry in table containing changed values to be sent
 * @param Number of entries of the table containing changed values that must be sent
 * @return none
 *
 * This function assumes that all data that must be sent is stored in a table where each item of index index has the structure:
 * c_cborvals vals[], for the variables
 * C_TIME t, for the timestamp when value was sampled
 * C_UNT16 cnt, for the monotonic counter of sent packets
 * __________________________________________________
 * | index | vals[].alias  | vals[].values | t | cnt |
 * |___0___|______"2"______|_____"1.5"_____|_5_|__6__|
 * |___1___|______"7"______|_____"2.3"_____|_5_|__7__|
 * |___2___|______"5"______|_____"7.1"_____|11_|__8__|
 * |___3___|_____"11"______|_____"1.2"_____|13_|__9__|
 */
void CBOR_SendFragmentedValues(C_UINT16 index, C_UINT16 number)
{
	C_INT16 framecnt = 1;
	while(number > ENTRY_PER_PKT)
	{
		CBOR_SendValues(index, ENTRY_PER_PKT, framecnt);
		index++;
		number -= ENTRY_PER_PKT;
		framecnt++;
	}
	CBOR_SendValues(index, number, -framecnt);

}

/**
 * @brief CBOR_Mobile
 *
 * Prepares CBOR encoded message containing information on mobile connection
 *
 * @param Pointer to the CBOR-encoded payload
 * @return void
 */
size_t CBOR_Mobile(C_CHAR* cbor_stream)
{
	CborEncoder encoder, mapEncoder;
	size_t len;
	int err;

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "mobile create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode gup - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "gup");
	C_INT32 gup = 72000;											// to be implemented
	err |= cbor_encode_int(&mapEncoder, gup);
	DEBUG_ADD(err, "gup");

	// encode sig - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "sig");
	C_INT16 sig = -75;											// to be implemented
	err |= cbor_encode_int(&mapEncoder, sig);
	DEBUG_ADD(err, "sig");

	// encode ime - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "ime");
	C_BYTE imei[15]={9,9,0,0,0,0,8,6,2,4,7,1,8,5,4};												// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, imei, 15);
	DEBUG_ADD(err, "ime");

	// other elements to come (if needed)
	// encode ims - elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "ims");
	C_BYTE ims[15] = {3,1,0,1,5,0,1,2,3,4,5,6,7,8,9};					// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, ims, 15);
	DEBUG_ADD(err, "ims");

	// encode mcc - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "mcc");
	C_BYTE mcc[3] = {2,2,2};											// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, mcc, 3);
	DEBUG_ADD(err, "mcc");

	// encode mnc - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "mnc");
	C_BYTE mnc[3] = {8,8};											// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, mnc, 3);
	DEBUG_ADD(err, "mcc");

	// encode lac - elem8
	err |= cbor_encode_text_stringz(&mapEncoder, "lac");
	C_BYTE lac[16] = {1,2,3,4,5};										// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, lac, 16);
	DEBUG_ADD(err, "lac");

	// encode cel - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "cel");
	C_BYTE cel[16] = {5,4,3,2,1};										// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, cel, 16);
	DEBUG_ADD(err, "cel");

	// encode uci - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "uci");
	C_BYTE uci[16] = {1,3,4,2,1,7,7,2,7};								// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, uci, 16);
	DEBUG_ADD(err, "uci");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;

}

/**
 * @brief CBOR_ResHeader
 *
 * Prepares CBOR response header
 *
 * @param Pointer to the CBOR-encoded header
 * @param Received request data
 * @param CBOR encoder struct
 * @param CBOR map
 * @return void
 */
void CBOR_ResHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req, CborEncoder* encoder, CborEncoder* mapEncoder)
{

	int err;

	cbor_encoder_init(encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(encoder, mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "header response");

	// encode ver - elem1
	err |= cbor_encode_text_stringz(mapEncoder, "ver");
	err |= cbor_encode_uint(mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "ver");

	// encode rto - elem2
	err |= cbor_encode_text_stringz(mapEncoder, "rto");
	err |= cbor_encode_text_stringz(mapEncoder, (char*)cbor_req->rto);
	DEBUG_ADD(err, "rto");

	// encode cmd - elem3
	err |= cbor_encode_text_stringz(mapEncoder, "cmd");
	err |= cbor_encode_uint(mapEncoder, cbor_req->cmd);
	DEBUG_ADD(err, "cmd");

   	// encode res - elem4
	err = cbor_encode_text_stringz(mapEncoder, "res");
	err |= cbor_encode_int(mapEncoder, cbor_req->res);
	DEBUG_ADD(err, "res");

}

/**
 * @brief CBOR_ResSimple
 *
 * Prepares CBOR response to generic simple request (only waiting for a success/fail report
 *
 * @param Pointer to the CBOR-encoded header
 * @param Received request header
 * @return Size of the appended header
 */
size_t CBOR_ResSimple(C_CHAR* cbor_response, c_cborhreq* cbor_req)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	err = cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

size_t CBOR_ResSetDevsConfig(C_CHAR* cbor_response, c_cborhreq* cbor_req)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;
	C_UINT16 device = 0;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	err = NVM__ReadU32Value(MB_DEV_NVM, (C_UINT32*)&device);
	// encode dev - elem5
	err = cbor_encode_text_stringz(&mapEncoder, "dev");
	err |= cbor_encode_int(&mapEncoder, device);
	DEBUG_ADD(err, "dev");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ResScanLine
 *
 * Prepares CBOR encoded message containing result of serial line scan
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Address of first responding device
 * @param Answer of the first responding device to Modbus command ReportSlaveId (command 17)
 * @param Length of answer
 * @return void
 */
size_t CBOR_ResScanLine(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 device, C_BYTE* answer, C_UINT16 answer_len)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	// encode dev - elem5
	err = cbor_encode_text_stringz(&mapEncoder, "dev");
	err |= cbor_encode_int(&mapEncoder, device);
	DEBUG_ADD(err, "dev");

	// encode answer - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "ans");
	err |= cbor_encode_byte_string(&mapEncoder, answer, answer_len);
	DEBUG_ADD(err, "ans");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ResSendMbAdu
 *
 * Prepares CBOR encoded message containing result of send mb adu
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Adu identifier (same as request)
 * @param Modbus response
 * @param Length of Modbus response
 * @return void
 */
size_t CBOR_ResSendMbAdu(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 seq, C_BYTE* val, C_UINT16 val_len)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	// encode seq - elem5
	err = cbor_encode_text_stringz(&mapEncoder, "seq");
	err |= cbor_encode_int(&mapEncoder, seq);
	DEBUG_ADD(err, "seq");

	// encode val - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "val");
	err |= cbor_encode_byte_string(&mapEncoder, val, val_len);
	DEBUG_ADD(err, "val");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ResRdWrValues
 *
 * Prepares CBOR encoded message containing result of read or write values
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Pointer to alias of read/written value
 * @param Pointer to value of read/written value
 * @return void
 */
size_t CBOR_ResRdWrValues(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_CHAR* ali, C_CHAR* val)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	// encode ali - elem4
	err = cbor_encode_text_stringz(&mapEncoder, "ali");
	err |= cbor_encode_text_stringz(&mapEncoder, (char*)ali);
	DEBUG_ADD(err, "ali");

	// encode val - elem5
	err = cbor_encode_text_stringz(&mapEncoder, "val");
	if (memcmp(val,"0",sizeof("0"))==0)
		err |= cbor_encode_null(&mapEncoder);
	else
		err |= cbor_encode_text_stringz(&mapEncoder, (char*)val);
	DEBUG_ADD(err, "val");

	// encode dev - elem6
	err = cbor_encode_text_stringz(&mapEncoder, "dev");
	C_UINT16 device;
	NVM__ReadU32Value(MB_DEV_NVM, (C_UINT32*)&device);
	err |= cbor_encode_int(&mapEncoder, device);

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ResSendMbPassThrough
 *
 * Prepares CBOR encoded message containing result of send_mb_pass_through message
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Status of pass through mode
 * @return void
 */
size_t CBOR_ResSendMbPassThrough(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 cbor_pass)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	// encode pas - elem4
	err = cbor_encode_text_stringz(&mapEncoder, "pas");
	err |= cbor_encode_int(&mapEncoder, cbor_pass);
	DEBUG_ADD(err, "pas");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ReqHeader
 *
 * Interprets CBOR request header
 *
 * @param Pointer to the CBOR-encoded stream
 * @param Length of CBOR stream
 * @param Pointer to the request struct
 * @return CborError
 */
CborError CBOR_ReqHeader(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborhreq* cbor_req)
{
	// parse request header and get replyTo and cmd fields
	// write them to cbor_req
	CborParser parser;
	size_t stlen;
    char tag[TAG_SIZE];
	char rto[RTO_SIZE];
	CborError err;
	CborValue it, recursed;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "header request map");

	C_UINT16 tmp = 0;
	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "ver", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_req->ver = tmp;
			DEBUG_DEC(err, "header: ver");
		}
		else if (strncmp(tag, "rto", 3) == 0)
		{
			stlen = RTO_SIZE;
			err |= cbor_value_copy_text_string(&recursed, rto, &stlen, &recursed);
			strcpy((char*)cbor_req->rto, rto);
			DEBUG_DEC(err, "header: rto");
		}
		else if (strncmp(tag, "cmd", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_req->cmd = tmp;
			DEBUG_DEC(err, "header: cmd");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "header: discard element");
		}
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}

/**
 * @brief CBOR_ReqSetLinesConfig
 *
 * Interprets CBOR set lines config request
 *
 * @param Pointer to the CBOR current element
 * @param Pointer to new baud rate
 * @param Pointer to new serial connector type
 * @return CborError
 */
CborError CBOR_ReqSetLinesConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT32* new_baud_rate, C_BYTE* new_connector)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set lines config request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "bau", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, new_baud_rate);
			DEBUG_DEC(err, "req_set_lines_config: bau");
		}
		else if (strncmp(tag, "con", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, new_connector);
			DEBUG_DEC(err, "req_set_lines_config: con");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_set_lines_config: discard element");
		}
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}

/**
 * @brief CBOR_ReqSetDevsConfig
 *
 * Interprets CBOR set devs config
 *
 * @param Pointer to the CBOR current element
 * @param Pointer to new username
 * @param Pointer to new password
 * @param Pointer to new uri
 * @param Pointer to new cid (for update)
 * @return CborError
 */
CborError CBOR_ReqSetDevsConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqdwldevsconfig* download_devs_config)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set devs config request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "usr", 3) == 0)
		{
			stlen = USERNAME_SIZE;
			err |= cbor_value_copy_text_string(&recursed, download_devs_config->usr, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: usr");
		}
		else if (strncmp(tag, "pwd", 3) == 0)
		{
			stlen = PASSWORD_SIZE;
			err |= cbor_value_copy_text_string(&recursed, download_devs_config->pwd, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: pwd");
		}
		else if (strncmp(tag, "uri", 3) == 0)
		{
			stlen = URI_SIZE;
			err |= cbor_value_copy_text_string(&recursed, download_devs_config->uri, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: uri");
		}
		else if (strncmp(tag, "cid", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, (int64_t*)&download_devs_config->cid);
			DEBUG_DEC(err, "req_set_devs_config: cid");
		}
		else if (strncmp(tag, "crc", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, (int64_t*)&download_devs_config->crc);
			DEBUG_DEC(err, "req_set_devs_config: crc");
		}
		else if (strncmp(tag, "dev", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, (int64_t*)&download_devs_config->dev);
			DEBUG_DEC(err, "req_set_devs_config: dev");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_set_devs_config: discard element");
		}
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}

/**
 * @brief CBOR_ReqRdWrValues
 *
 * Interprets CBOR read/write values request
 *
 * @param Pointer to the CBOR current element
 * @param Pointer to structure for read/write values command
 * @return CborError
 */
CborError CBOR_ReqRdWrValues(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqrdwrvalues* cbor_rwv)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE]={'\0'};
	char text[30];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "write values request map");

	C_UINT16 tmp = 0;
	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "ali", 3) == 0)
		{
			stlen = ALIAS_SIZE;
			err |= cbor_value_copy_text_string(&recursed, text, &stlen, &recursed);
			memcpy(cbor_rwv->alias, text, stlen);
			DEBUG_DEC(err, "req_rdwr_values: ali");
		}
		else if (strncmp(tag, "val", 3) == 0)
		{
			stlen = VAL_SIZE;
			err |= cbor_value_copy_text_string(&recursed, text, &stlen, &recursed);
			memcpy(cbor_rwv->val, text, stlen);
			DEBUG_DEC(err, "req_rdwr_values: val");
		}
		else if (strncmp(tag, "fun", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->func = tmp;
			DEBUG_DEC(err, "req_rdwr_values: fun");
		}
		else if (strncmp(tag, "adr", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->addr = tmp;
			DEBUG_DEC(err, "req_rdwr_values: adr");
		}
		else if (strncmp(tag, "dim", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->dim = tmp;
			DEBUG_DEC(err, "req_rdwr_values: dim");
		}
		else if (strncmp(tag, "pos", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->pos = tmp;
			DEBUG_DEC(err, "req_rdwr_values: pos");
		}
		else if (strncmp(tag, "len", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->len = tmp;
			DEBUG_DEC(err, "req_rdwr_values: len");
		}
		else if (strncmp(tag, "a", 3) == 0)
		{
			stlen = A_SIZE;
			err |= cbor_value_copy_text_string(&recursed, text, &stlen, &recursed);
			memcpy(cbor_rwv->a, text, stlen);
			DEBUG_DEC(err, "req_rdwr_values: a");
		}
		else if (strncmp(tag, "b", 3) == 0)
		{
			stlen = B_SIZE;
			err |= cbor_value_copy_text_string(&recursed, text, &stlen, &recursed);
			memcpy(cbor_rwv->b, text, stlen);
			DEBUG_DEC(err, "req_rdwr_values: b");
		}
		else if (strncmp(tag, "flg", 3) == 0)
		{
			flag_t ftmp = {0};
			err |= CBOR_ExtractInt(&recursed, &ftmp);
			cbor_rwv->flags = ftmp;
			DEBUG_DEC(err, "req_rdwr_values: flg");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_rdwr_values: discard element");
		}
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}

/**
 * @brief CBOR_ExtractInt
 *
 * Extracts int from a CBOR stream and advance pointer to next field
 *
 * @param Pointer to the CBOR current element
 * @param Pointer to read value
 * @return CborError
 */
CborError CBOR_ExtractInt(CborValue* recursed, int64_t* read)
{
	CborError err = cbor_value_get_int64(recursed, read);
	err |= cbor_value_advance_fixed(recursed);
	return err;
}

/**
 * @brief CBOR_ReqSetGwConfig
 *
 * Interprets CBOR set gw config
 *
 * @param Pointer to the CBOR stream
 * @param Length of the CBOR stream
 * @param Pointer to structure for set gw config command
 * @return CborError
 */
CborError CBOR_ReqSetGwConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqsetgwconfig* cbor_setgwconfig)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set gw config request map");

	C_UINT16 tmp = 0;
	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "pva", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->pva = tmp;
			DEBUG_DEC(err, "req_set_gw_config: pva");
		}
		else if (strncmp(tag, "pst", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->pst = tmp;
			DEBUG_DEC(err, "req_set_gw_config: pst");
		}
		else if (strncmp(tag, "mka", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->mka = tmp;
			DEBUG_DEC(err, "req_set_gw_config: mka");
		}
		else if (strncmp(tag, "lss", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->lss = tmp;
			DEBUG_DEC(err, "req_set_gw_config: lss");
		}
		else if (strncmp(tag, "hss", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->hss = tmp;
			DEBUG_DEC(err, "req_set_gw_config: hss");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_set_gw_config: discard element");
		}
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}

/**
 * @brief CBOR_ReqSendMbAdu
 *
 * Interprets CBOR send mb adu
 *
 * @param Pointer to the CBOR stream
 * @param Length of the CBOR stream
 * @param Pointer to the field seq used to detect wrong order in message de-queueing
 * @param Pointer to the buffer containing modbus request
 * @return CborError
 */
CborError CBOR_ReqSendMbAdu(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16* seq, C_CHAR* adu)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "send mb adu request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "seq", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, seq);
			DEBUG_DEC(err, "req_send_mb_adu: seq");
		}
		else if (strncmp(tag, "adu", 3) == 0)
		{
			stlen = ADU_SIZE;
			err |= cbor_value_copy_text_string(&recursed, (char*)adu, &stlen, &recursed);
			DEBUG_DEC(err, "req_send_mb_adu: adu");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_send_mb_adu: discard element");
		}
	}
	err = cbor_value_leave_container(&it, &recursed);
	return err;
}

/**
 * @brief CBOR_ReqSendMbPassThrough
 *
 * Interprets CBOR send mb adu request
 *
 * @param Pointer to the CBOR stream
 * @param Length of the CBOR stream
 * @param Pointer to pass through mode status
 * @return CborError
 */
CborError CBOR_ReqSendMbPassThrough(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16* cbor_pass)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "send_mb_pass_through request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "pas", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, cbor_pass);
			DEBUG_DEC(err, "send_mb_pass_through: pas");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "send_mb_pass_through: discard element");
		}
	}
	err = cbor_value_leave_container(&it, &recursed);
	return err;


}

/**
 * @brief CBOR_ReqReboot
 *
 * Interprets CBOR reboot request
 *
 * @param Pointer to the CBOR stream
 * @param Length of the CBOR stream
 * @param Pointer to cid identifier
 * @return CborError
 */
CborError CBOR_ReqReboot(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16* cid)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "reboot request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "cid", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, cid);
			DEBUG_DEC(err, "reboot: cid");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "reboot: discard element");
		}
	}
	err = cbor_value_leave_container(&it, &recursed);
	return err;
}


/**
 * @brief CBOR_DiscardElement
 *
 * Interprets CBOR and discard unneedee fields
 *
 * @param Pointer to the CBOR struct
 * @return CborError
 */
CborError CBOR_DiscardElement(CborValue* it)
{
	CborError err = CborNoError;

	CborType type = cbor_value_get_type(it);
	switch (type) {
		case CborIntegerType: {
			int64_t val;
			err = CBOR_ExtractInt(it, &val);     // can't fail
			//printf("%lld\n", (long long)val);
			break;
		}
		case CborByteStringType: {
			size_t n=512;
			uint8_t stringa[512];
			//buf=&stringa;
			err = cbor_value_copy_byte_string(it, stringa, &n, it);
			//dumpbytes(stringa, n);
			break;
		}
		case CborTextStringType: {
			size_t n=512;
			char text[512];
			//buf=&text;
			err = cbor_value_copy_text_string(it, text, &n, it);
			//puts(text);
			break;
		}
		default:
			break;
	}
	return err;
}



/**
 * @brief CBOR_ReqUpdateDevFW
 *
 * Interprets CBOR update dev FW
 *
 * @param Pointer to the CBOR current element
 * @param Pointer to username
 * @param Pointer to password
 * @param Pointer to uri
 * @param Pointer to fid          0..9999 file index, valid only for modbus file transf.
 * @param Pointer to wet          number of second needed by the target to perform a reboot after an upgrade
 *
 * @return CborError
 */
CborError CBOR_ReqUpdateDevFW(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborrequpddevfw* update_dev_fw)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set devs config request map");

	C_UINT16 tmp;
	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "usr", 3) == 0)
		{
			stlen = USERNAME_SIZE;
			err |= cbor_value_copy_text_string(&recursed, update_dev_fw->usr, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: usr");
		}
		else if (strncmp(tag, "pwd", 3) == 0)
		{
			stlen = PASSWORD_SIZE;
			err |= cbor_value_copy_text_string(&recursed, update_dev_fw->pwd, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: pwd");
		}
		else if (strncmp(tag, "uri", 3) == 0)
		{
			stlen = URI_SIZE;
			err |= cbor_value_copy_text_string(&recursed, update_dev_fw->uri, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: uri");
		}
		else if (strncmp(tag, "fid", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			update_dev_fw->fid = tmp;
			DEBUG_DEC(err, "req_set_devs_config: fid");
		}
		else if (strncmp(tag, "wet", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			update_dev_fw->wet = tmp;
			DEBUG_DEC(err, "req_set_devs_config: wet");
		}
		else
		{
			err = CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_: discard element");
		}
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}



/**
 * @brief CBOR_ReqTopicParser
 *
 * Interprets CBOR received request
 *
 * @param Pointer to the CBOR stream
 * @param Length of the CBOR stream
 * @return CborError
 */
int CBOR_ReqTopicParser(C_CHAR* cbor_stream, C_UINT16 cbor_len){

	c_cborhreq cbor_req = {0};
	CborError err;
	C_CHAR cbor_response[RESPONSE_SIZE];		// buffer to store response, maybe better global...
	size_t len = 0;

	C_MQTT_TOPIC topic;
	C_GATEWAY_ID dev_id;
	Get_Gateway_ID(&dev_id);
	strcpy((char*)topic,(char*)dev_id);

	err = CBOR_ReqHeader(cbor_stream, cbor_len, &cbor_req);

	switch(cbor_req.cmd){

		case REBOOT:
		{
			C_UINT16 cbor_cid;
			err = CBOR_ReqReboot(cbor_stream, cbor_len, &cbor_cid);
			printf("cid %d\n", cbor_cid);
			//save somewhere in nvm cbor_cid

			// mqtt response
			cbor_req.res = (err == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			if(SUCCESS_CMD == cbor_req.res){
				PollEngine_StopEngine_CAREL();
				// save cid for successive hello
				NVM__WriteU32Value(MB_CID_NVM, cbor_cid);
				MQTT_FlushValues();
				GME__Reboot();		//todo
			}
		}
		break;

		case FLUSH_VALUES:
		{
			//flush values
			//answer?
			printf("flush_values\n");
		}
		break;


		case SET_GW_CONFIG:
		{
			c_cborreqsetgwconfig cbor_setgwconfig = {0};
			err = CBOR_ReqSetGwConfig(cbor_stream, cbor_len, &cbor_setgwconfig);

			// write new data to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = (execute_set_gw_config(cbor_setgwconfig) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;

			// mqtt response
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			if(SUCCESS_CMD == cbor_req.res)
				if(PollEngine_GetEngineStatus_CAREL() == RUNNING){
					PollEngine_StopEngine_CAREL();
					MQTT_FlushValues();
					GME__Reboot();		//todo
			}
		}
		break;

		case SET_DEVS_CONFIG:
		{
			c_cborreqdwldevsconfig download_devs_config = {0};
			err = CBOR_ReqSetDevsConfig(cbor_stream, cbor_len, &download_devs_config);

			// write new data to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = (execute_download_devs_config(&download_devs_config) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;

			// mqtt response
			//len = CBOR_ResSimple(cbor_response, &cbor_req);
			len = CBOR_ResSetDevsConfig(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			if(SUCCESS_CMD == cbor_req.res){
				if(PollEngine_GetEngineStatus_CAREL() == RUNNING){
					PollEngine_StopEngine_CAREL();
					// save cid for successive hello
					NVM__WriteU32Value(MB_CID_NVM, download_devs_config.cid);
					MQTT_FlushValues();
					GME__Reboot();		//todo
				}
			}
		}
		break;

		case SCAN_DEVICES:
		{
			// scan Modbus line

			C_UINT16 device = 0;
			C_BYTE answer[REPORT_SLAVE_ID_SIZE];
			C_INT16 length;
			cbor_req.res = SUCCESS_CMD;

			PollEngine_MBSuspend_IS();

			execute_scan_devices(&answer, &device, &length);

			if(length <= 0){
				memcpy(answer, "", 1);
				length=1;
			}
			else
			{   //include the 2 byte of the CRC modbus TODO
				length += 2;
			}

			//  // da riarrangiare
            len = CBOR_ResScanLine(cbor_response, &cbor_req, device, answer, length);

            // TODO
            sprintf(topic,"%s%s", "/res/", cbor_req.rto);
            mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			PollEngine_MBResume_IS();
		}
		break;

		case SET_LINES_CONFIG:
		{

			C_UINT32 new_baud_rate;
			C_BYTE new_connector;
			err = CBOR_ReqSetLinesConfig(cbor_stream, cbor_len, &new_baud_rate, &new_connector);

			// write new baud rate and connector to configuration file and put in res the result of operation
			cbor_req.res = (execute_set_line_config(new_baud_rate, new_connector) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			if(SUCCESS_CMD == cbor_req.res){
				if(PollEngine_GetEngineStatus_CAREL() == RUNNING){
					PollEngine_StopEngine_CAREL();
					MQTT_FlushValues();
					GME__Reboot();		//todo
				}
			}
		}
		break;


		case SEND_MB_ADU:
		{
			C_UINT16 seq = 0;
			C_CHAR adu[ADU_SIZE];
			err = CBOR_ReqSendMbAdu(cbor_stream, cbor_len, &seq, adu);

			// execute command (when polling machine available) and gather result
			// put modbus answer in adu buffer to reuse resources and save memory
		/*	cbor_req.res = 1; // todo
			C_UINT16 adulen = 8; // todo
			adu[0]=0x01;
			adu[1]=0x03;
			adu[2]=0x00;
			adu[3]=0x01;
			adu[4]=0x00;
			adu[5]=0x01;
			adu[6]=0xAA;
			adu[7]=0xBB;*/

			if(ACTIVATED == PollEngine__GetPassModeStatus()){

				PollEngine_MBSuspend_IS();
				PollEngine__SetPassModeCMD(RECEIVED);

				uint8_t data_rx[20] = {0};
				uint8_t data_rx_len;
//				c_cbor_send_mb_adu send_mb_adu = {0};
//				parse_send_mb_adu(root, &send_mb_adu);
//				data_rx_len = PollEngine__SendMBAdu(&send_mb_adu, data_rx);
data_rx_len=0;
				// mqtt response
				if(data_rx_len <= 0)
					cbor_req.res = C_FAIL;
				else
					cbor_req.res = C_SUCCESS;

				len = CBOR_ResSendMbAdu(cbor_response, &cbor_req, seq, data_rx, data_rx_len);
				sprintf(topic,"%s%s", "/res/", cbor_req.rto);
				mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

				PollEngine__SetPassModeCMD(EXECUTED);
				PollEngine_MBResume_IS();

			}

		}
		break;


		// TODO: evaluate to collapse read values and write values code portions into a single block to optimize memory occupation
		case READ_VALUES:
		{
			// which status should be polling engine?
			// should I check it is in a specific status?
			// what to do if it is not in that status?
			c_cborreqrdwrvalues cbor_rv = {0};
			len = CBOR_ReqRdWrValues(cbor_stream, cbor_len, &cbor_rv);

			// send modbus command to write values     //CHIEBAO
			// wait modbus response to get result
			while(STOPPED != PollEngine_GetPollingStatus_CAREL())
				vTaskDelay(100/portTICK_RATE_MS);

			cbor_req.res = (parse_read_values(&cbor_rv) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;

			if(cbor_req.res == C_SUCCESS)
			{
					printf("OPERATION_SUCCEEDED, PollEngine__Read_COIL_Req x=%s\n", cbor_rv.val);
					len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_rv.alias, cbor_rv.val);
			}
			else
			{
					printf("OPERATION_FAILED\n");
					len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_rv.alias, "0");
			}
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;


		case WRITE_VALUES:
		{
			// which status should be polling engine?
			// should I check it is in a specific status?
			// what to do if it is not in that status?
			c_cborreqrdwrvalues cbor_wv = {0};
			len = CBOR_ReqRdWrValues(cbor_stream, cbor_len, &cbor_wv);

			// send modbus command to write values     //CHIEBAO
			// wait modbus response to get result
			while(STOPPED != PollEngine_GetPollingStatus_CAREL())
				vTaskDelay(100/portTICK_RATE_MS);


			cbor_req.res = (parse_write_values(cbor_wv) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;


			if(cbor_req.res == C_SUCCESS)
			{
					printf("OPERATION_SUCCEEDED, PollEngine__Read_COIL_Req x=%s\n", cbor_wv.val);
					len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_wv.alias, cbor_wv.val);
			}
			else
			{
					printf("OPERATION_FAILED\n");
					len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_wv.alias, "0");
			}

			// send response with result
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);

			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;


		case UPDATE_GME_FIRMWARE:
		{
/*EGISIAN
			s_ota_gme_group = xEventGroupCreate();
			bool previous_poll_engine_status = false;
			req_update_gw_fw_t update_gw_fw = {0};
			parse_update_gw_fw(root, &update_gw_fw);

			if(PollEngine__GetEngineStatus() == RUNNING){
				PollEngine__StopEngine();
				MQTT_FlushValues();
				previous_poll_engine_status = true;
			}

			OTA__GMEInit();
			OTA_BITS = xEventGroupWaitBits(s_ota_gme_group, OTA_GME_OK | OTA_GME_FAIL, true, false, portMAX_DELAY);

			if((OTA_BITS & OTA_GME_OK) != 0){

				send_simple_res(ReqHeader.replyTo, UPDATE_GME_FIRMWARE, SUCCESS_CMD, MQTT__GetClient());
				GME__Reboot();

			}else if((OTA_BITS & OTA_GME_FAIL) != 0){
				send_simple_res(ReqHeader.replyTo, UPDATE_GME_FIRMWARE, ERROR_CMD, MQTT__GetClient());
			}

			memmgr_free(update_gw_fw.username);
			memmgr_free(update_gw_fw.password);
			memmgr_free(update_gw_fw.uri);
			vEventGroupDelete(s_ota_gme_group);

			if(previous_poll_engine_status == true){
				PollEngine__StartEngine();
			}
//EGISIAN*/

		}
		break;


		case UPDATE_DEV_FIRMWARE:
		{
			bool previous_poll_engine_status = false;

			c_cborrequpddevfw update_dev_fw = {0};

			CBOR_ReqUpdateDevFW(cbor_stream, cbor_len, &update_dev_fw);


			/* TODO BILATO
			if (PollEngine__GetEngineStatus() == RUNNING){
				PollEngine__StopEngine();
				MQTT_FlushValues();
				previous_poll_engine_status = true;
			}
			*/


			if (C_SUCCESS == OTA__DevFWUpdate(&update_dev_fw)){
				GME__Reboot();
			}else{
				//TODO BILATO
				//send_simple_res(ReqHeader.replyTo, UPDATE_DEV_FIRMWARE, ERROR_CMD, MQTT__GetClient());

			}

			/* TODO BILATO
			if(previous_poll_engine_status == true){
				PollEngine__StartEngine();
			}
			*/

		}
		break;


		case UPDATE_CA_CERTIFICATES:
		{
			c_cborrequpdatecacert update_ca_config = {0};
			err = CBOR_ReqUpdateCaCertificate(cbor_stream, cbor_len, &update_ca_config);

			// perform a https read file from uri, using usr and pwd authentication data
			cbor_req.res = (execute_update_ca_cert(&update_ca_config) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			// send a report of operation
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			if (cbor_req.res == C_SUCCESS)
			{
				if(PollEngine_GetEngineStatus_CAREL() == RUNNING){
					PollEngine_StopEngine_CAREL();
					MQTT_FlushValues();
					GME__Reboot();
				}
			}
		}
		break;


		case CHANGE_CREDENTIALS:
		{
			c_cborreqchangecred change_cred_config = {0};
			err = CBOR_ReqChangeCredentials(cbor_stream, cbor_len, &change_cred_config);

			// write new credentials to configuration file and put in res the result of operation
			cbor_req.res = (execute_change_cred(change_cred_config) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			// mqtt response
			// to be implemented
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			if(SUCCESS_CMD == cbor_req.res){
				if(PollEngine_GetEngineStatus_CAREL() == RUNNING){
					PollEngine_StopEngine_CAREL();
					MQTT_FlushValues();
					GME__Reboot();		//todo
				}
			}
		}
		break;


		case START_ENGINE:
		{
			PollEngine_StartEngine_CAREL();
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;


		case STOP_ENGINE:
		{
			PollEngine_StopEngine_CAREL();
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;

		case SEND_MB_PASS_THROUGH:
		{
			C_UINT16 cbor_pass;
			err = CBOR_ReqSendMbPassThrough(cbor_stream, cbor_len, &cbor_pass);
			// change pass through mode or just get status
			PollEngine__ActivatePassMode();

			len = CBOR_ResSendMbPassThrough(cbor_response, &cbor_req, cbor_pass);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;

		default:
			cbor_req.res = INVALID_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		break;

	}

	return 0;
}

C_RES execute_update_ca_cert(c_cborrequpdatecacert *update_ca_cert){

	https_conn_err_t err;

	printf("execute_update_ca_cert \n");

	err = HttpsClient__UpdateCertificate(update_ca_cert, CERT_1);

	if(CONN_FAIL == err){
		err = HttpsClient__UpdateCertificate(update_ca_cert, CERT_2);

	}else if(FILE_NOT_SAVED == err){
		return C_FAIL;
	}

	printf("execute_update_ca_cert err= %d \n",err);

	if(err != CONN_OK){
		return C_FAIL;
	}

	return C_SUCCESS;
}


C_RES execute_set_line_config(C_UINT32 new_baud_rate, C_BYTE new_connector){

	C_RES err = NVM__WriteU32Value(MB_BAUDRATE_NVM, new_baud_rate);
	err |= NVM__WriteU8Value(MB_CONNECTOR_NVM, new_connector);
	err |= NVM__WriteU8Value(SET_LINE_CONFIG_NVM, CONFIGURED);

	return err;
}

C_RES execute_download_devs_config(c_cborreqdwldevsconfig *download_devs_config){

	https_conn_err_t err;

	// just a travaso of data
	req_download_devs_config_t download_info = {0};
	download_info.password = download_devs_config->pwd;
	download_info.username = download_devs_config->usr;
	download_info.uri      = download_devs_config->uri;

	printf("execute_download_devs_config \n");
	err = NVM__WriteU32Value(MB_DEV_NVM, download_devs_config->dev);
	if (err == C_FAIL)
		return C_FAIL;
	err = HttpsClient__DownloadFile(&download_info, CERT_1, MODEL_FILE);
	if(CONN_FAIL == err)
		err = HttpsClient__DownloadFile(&download_info, CERT_2, MODEL_FILE);
	else if(FILE_NOT_SAVED == err)
		return C_FAIL;

	printf("execute_download_devs_config err= %d \n",err);
	if(CONN_OK == err){
		if(C_SUCCESS == NVM__WriteU8Value(SET_DEVS_CONFIG_NVM, CONFIGURED)){
			printf("MODEL FILE SAVED\n");
			err = C_SUCCESS;
		}else{
			printf("MODEL FILE NOT SAVED\n");
			err = C_FAIL;
		}
	}else
		return C_FAIL;

	return C_SUCCESS;
}


C_RES execute_set_gw_config(c_cborreqsetgwconfig set_gw_config)
{
	printf("Execute Set GW Config\n");
	req_set_gw_config_t gw_config_nvm = {0};
	size_t len = 0;
	C_BYTE gw_config_status;

	if(C_SUCCESS == NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status) && CONFIGURED == gw_config_status){
		NVM__ReadBlob(SET_GW_PARAM_NVM, (void*)&gw_config_nvm, &len);
	}

	gw_config_nvm.hispeedsamplevalue = set_gw_config.hss;

	gw_config_nvm.lowspeedsamplevalue = set_gw_config.lss;

	gw_config_nvm.mqttKeepAliveInterval = set_gw_config.mka;

	gw_config_nvm.statusPeriod = set_gw_config.pst;

	gw_config_nvm.valuesPeriod = set_gw_config.pva;

	C_RES err = NVM__WriteBlob(SET_GW_PARAM_NVM,(void*)&gw_config_nvm,sizeof(gw_config_nvm));
	if(C_SUCCESS == err){
		err = NVM__WriteU8Value(SET_GW_CONFIG_NVM, CONFIGURED);
	}

	return err;
}

C_RES execute_change_cred(c_cborreqdwldevsconfig change_cred){

	printf("Execute Change Credentials\n");
	if(C_SUCCESS == NVM__WriteString(MQTT_USER, change_cred.usr)
			&& C_SUCCESS == NVM__WriteString(MQTT_PASS, change_cred.pwd)){
		return C_SUCCESS;
	}else{
		return C_FAIL;
	}
}


/**
 * @brief execute_scan_devices
 *
 * execute_scan_devices
 *
 * @param Pointer to the
 * @param Pointer to
 * @param Pointer to
 * @return C_RES
 */
C_RES execute_scan_devices(C_BYTE *data_rx, C_UINT16 *add, C_INT16 * lnt)
{
	C_INT16 len = 0;
	C_UINT16 addr = 0;
	C_RES err = 0;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	// send request  ... manda la richiesta ma ritorna comunque un errore  TODO
	do{
	  err = app_report_slave_id_read(++addr);

	  vTaskDelay(500 / portTICK_PERIOD_MS);

	}while(err != C_SUCCESS);

	vTaskDelay(1000 / portTICK_PERIOD_MS);
#endif

	if(err != 0)
	  return C_FAIL;
//
// save the data
//
#ifdef INCLUDE_PLATFORM_DEPENDENT
	len = usMBSlaveIDLen;

	// get response
	for(C_INT16 i = 0; i < len; i++)
	  *(data_rx + i) = 	ucMBSlaveID[i];

#endif

	*add = addr;
	*lnt = len;

	return C_SUCCESS;
}

/**
 * @brief read_values_conversion
 *
 *   convert the read data into the right data type
 *
 * @param  hr_ir_low_high_poll_t *hr_to_read
 * @return long double
 */
long double read_values_conversion(hr_ir_low_high_poll_t *hr_to_read){

	long double conv_value = 0;

	switch(hr_to_read->read_type){
		case TYPE_A:
		{
			float c_read= 0.0;
			c_read = get_type_a(hr_to_read, CURRENT);
			printf("TYPE_A/B c_read: %f \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_B:
		{
			float c_read= 0.0;
			c_read = get_type_b(hr_to_read, CURRENT);
			printf("TYPE_A/B c_read: %f \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_C_SIGNED:
		{
			int32_t c_read = 0;
			c_read = get_type_c_signed(hr_to_read, CURRENT);
			printf("TYPE_CU c_read: %d \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_C_UNSIGNED:
		{
			uint32_t c_read = 0;
			c_read = get_type_c_unsigned(hr_to_read, CURRENT);
			printf("TYPE_CS c_read: %d \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_D:
		{
			uint8_t c_read = 0;
			c_read = get_type_d(hr_to_read, CURRENT);
			printf("TYPE_D c_read: %d \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_E:
		{
			int32_t c_read = 0;
			c_read = get_type_e(hr_to_read, CURRENT);
			printf("TYPE_E c_read: %d \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_F_SIGNED:
		{
			int16_t c_read = 0;
			c_read = get_type_f_signed(hr_to_read, CURRENT);
			printf("TYPE_FS c_read: %d \n",c_read);
			conv_value = (long double)c_read;
		}
				break;

		case TYPE_F_UNSIGNED:
		{
			uint16_t c_read = 0;
			c_read = get_type_f_unsigned(hr_to_read, CURRENT);
			printf("TYPE_FU c_read: %d \n",c_read);
			conv_value = (long double)c_read;
		}
			break;

		default:
			break;
		}
	return conv_value;
}


/**
 * @brief parse_write_values
 *
 *        manage the incoming cbor payload to write value via modbus
 *
 * @param  c_cborreqrdwrvalues cbor_wv
 * @return C_RES
 */
C_RES parse_write_values(c_cborreqrdwrvalues cbor_wv)
{
	C_RES result = C_FAIL;
	// todo da gestire tutte le conversioni di tipo nei vari casi

	C_CHAR num_reg;
	C_FLOAT val_to_write;

	double res;

	res = atof((C_SCHAR*)cbor_wv.val);

	val_to_write = (C_FLOAT)res;

	switch(cbor_wv.func){

		case mbW_COIL:{

			result = PollEngine__Write_COIL_Req(val_to_write, cbor_wv.addr);
		}
		break;

		case mbW_HR:{

			if(cbor_wv.flags.bit.fixedpoint == 1 ||
			   cbor_wv.flags.bit.ieee == 1)
				val_to_write = (val_to_write - (long double)(atof((C_SCHAR*)cbor_wv.b)))  /  (long double)(atof((C_SCHAR*)cbor_wv.a));

			if(cbor_wv.dim == 16) { num_reg = 1; }
			else                  { num_reg = 2; }

			result = PollEngine__Write_HR_Req(val_to_write , cbor_wv.addr, num_reg);
		}
		break;

		default:
		break;

	}
	return result;
}

/**
 * @brief parse_read_values
 *
 *    manage the incoming cbor payload to read value via modbus
 *
 * @param  c_cborreqrdwrvalues* cbor_rv
 * @return C_RES
 */
C_RES parse_read_values(c_cborreqrdwrvalues* cbor_rv){

	printf("function = %d\n", cbor_rv->func);
	C_RES result = C_FAIL;

	switch(cbor_rv->func){

	case mbR_COIL:
	case mbR_DI:
	{
		r_coil_di coil_to_read = {0};
		coil_to_read.Addr = cbor_rv->addr;

		C_UINT16 read_value = 0;

		result = PollEngine__Read_COIL_DI_Req(cbor_rv->func ,cbor_rv->addr, &read_value);

		C_UINT16 temp = 0;
		C_BYTE bit = 0;

		bit = coil_to_read.Addr % 16;

		temp = read_value & (C_UINT16)(1 << bit);
		temp == 0 ? (temp = 0) : (temp = 1);
		itoa(temp, (C_SCHAR*)cbor_rv->val, 10);
	}
	break;


	case mbR_IR:
	case mbR_HR:
	{
		hr_ir_low_high_poll_t hr_to_read = {0};

		long double conv_value = 0;

		hr_to_read.info.Addr = cbor_rv->addr;
		hr_to_read.info.dim = cbor_rv->dim;
		hr_to_read.info.bitposition = cbor_rv->pos;
		hr_to_read.info.len = cbor_rv->len;
		hr_to_read.info.linA = atoi((C_SCHAR*)cbor_rv->a);
		hr_to_read.info.linB = atoi((C_SCHAR*)cbor_rv->b);
		hr_to_read.info.flag.byte = cbor_rv->flags.byte;

		result = PollEngine__Read_HR_IR_Req(cbor_rv->func, hr_to_read.info.Addr, hr_to_read.info.dim ,(void*)&hr_to_read.c_value.value);

		if(C_SUCCESS == result){
			if(hr_to_read.info.dim > 16 && 1 == hr_to_read.info.flag.bit.bigendian){
				C_UINT32 temp = hr_to_read.c_value.value;
				hr_to_read.c_value.reg.high = (C_UINT16)temp;
				hr_to_read.c_value.reg.low = (C_UINT16)(temp >> 16);
		    }

		    hr_to_read.read_type = check_hr_ir_reg_type(hr_to_read.info);

			conv_value = read_values_conversion(&hr_to_read);

			if(hr_to_read.info.dim > 16)
			   sprintf((C_SCHAR*)cbor_rv->val,"%.1Lf", conv_value);
			else
			   itoa(conv_value, (C_SCHAR*)cbor_rv->val, 10);
		}
		else
			itoa(0, (C_SCHAR*)cbor_rv->val, 10);
	}
	break;

	default:
	break;
	}

	return result;
}


