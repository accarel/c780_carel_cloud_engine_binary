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
#include "CBOR_CAREL.h"
#include "File_System_CAREL.h"
#include "tinycbor/cbor.h"
#include "Miscellaneous_IS.h"
#include "RTC_IS.h"
#include "poll_engine.h"
#include "MQTT_Interface_IS.h"
#include "MQTT_Interface_CAREL.h"
#include "nvm.h"
#include "binary_model.h"



/* Exported types ------------------------------------------------------------*/


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

	cbor_encoder_init(&encoder, cbor_stream, CBORSTREAM_SIZE, 0);
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

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

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

	cbor_encoder_init(&encoder, cbor_stream, CBORSTREAM_SIZE, 0);
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
	C_UINT16 device_address = 1;	//same as done by Arsanius
	//Get_Device_Address(&device_address);
	err |= cbor_encode_uint(&mapEncoder, device_address);
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
	if(C_SUCCESS == NVM__ReadU32Value(MB_CID_NVM, &cid)){
	   	err |= cbor_encode_uint(&mapEncoder, cid);
	}else{
	   	err |= cbor_encode_uint(&mapEncoder, 0);
	}
	DEBUG_ADD(err, "cid");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }
	for(int i=0; i<len; i++)
	printf("%x\n", cbor_stream[i]);
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

	cbor_encoder_init(&encoder, cbor_stream, CBORSTREAM_SIZE, 0);
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
	err |= cbor_encode_uint(&mapEncoder, PollEngine__GetEngineStatus());
	DEBUG_ADD(err,"est");

	// encode sgn -elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "sgn");
	C_INT16 rssi = -47;													// to be implemented
	err |= cbor_encode_int(&mapEncoder, rssi);
	DEBUG_ADD(err,"sgn");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

db_values val_array[10];

C_UINT32 Get_Counter(C_UINT16 index){return val_array[index].cnt;}
C_TIME Get_SamplingTime(C_UINT16 index){return val_array[index].t;}
C_CHAR a[1];
C_CHAR* Get_Alias(C_UINT16 index, C_UINT16 i){return val_array[index].vls[i].alias;}
C_CHAR* Get_Value(C_UINT16 index, C_UINT16 i){return val_array[index].vls[i].values;}

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
	size_t len = CBOR_Values(mybuf, index, number, frame);
	printf("valuespkt len %d: \n", len);
	for (int i=0;i<len;i++){
		printf("%02X ", mybuf[i]);
	}
	printf("\n");
	printf("values pkt binary: \n");
	for (int i=0;i<len;i++){
			printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(mybuf[i]));
	}
	printf("\n");
	#endif
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/values"), (C_SBYTE*)txbuff, len, 0, 0);
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

	cbor_encoder_init(&encoder, cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, 6);
	DEBUG_ENC(err, "values create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode cnt - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "cnt");
	C_UINT32 cnt = Get_Counter(index);
	err |= cbor_encode_uint(&mapEncoder, cnt);
	DEBUG_ADD(err, "cnt");

	// encode btm - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "btm");
	C_TIME t = RTC_Get_UTC_Boot_Time();
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "btm");

	// encode t - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
	t = RTC_Get_UTC_Current_Time();
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

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
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

	cbor_encoder_init(&encoder, cbor_stream, CBORSTREAM_SIZE, 0);
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

	// encode cid - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "cid");
	C_BYTE cid[16] = {5,4,3,2,1};										// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, cid, 16);
	DEBUG_ADD(err, "cid");

	// encode uci - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "uci");
	C_BYTE uci[16] = {1,3,4,2,1,7,7,2,7};								// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, uci, 16);
	DEBUG_ADD(err, "uci");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
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

	cbor_encoder_init(encoder, cbor_stream, CBORSTREAM_SIZE, 0);
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
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
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
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
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
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
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
	if (val == NULL)
		err |= cbor_encode_null(&mapEncoder);
	else
		err |= cbor_encode_text_stringz(&mapEncoder, (char*)val);
	DEBUG_ADD(err, "val");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
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
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
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
	char rto[REPLYTO_SIZE];
	CborError err;
	CborValue it, recursed;

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "header request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "ver", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_req->ver));
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
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_req->cmd));
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set lines config request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "bau", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)new_baud_rate));
			DEBUG_DEC(err, "req_set_lines_config: bau");
		}
		else if (strncmp(tag, "con", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)new_connector));
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
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
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)download_devs_config->cid));
			DEBUG_DEC(err, "req_set_devs_config: cid");
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "write values request map");

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
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_rwv->func));
			DEBUG_DEC(err, "req_rdwr_values: fun");
		}
		else if (strncmp(tag, "adr", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_rwv->addr));
			DEBUG_DEC(err, "req_rdwr_values: adr");
		}
		else if (strncmp(tag, "dim", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_rwv->dim));
			DEBUG_DEC(err, "req_rdwr_values: dim");
		}
		else if (strncmp(tag, "pos", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_rwv->pos));
			DEBUG_DEC(err, "req_rdwr_values: pos");
		}
		else if (strncmp(tag, "len", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_rwv->len));
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
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_rwv->flags));
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set gw config request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "pva", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_setgwconfig->pva));
			DEBUG_DEC(err, "req_set_gw_config: pva");
		}
		else if (strncmp(tag, "pst", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_setgwconfig->pst));
			DEBUG_DEC(err, "req_set_gw_config: pst");
		}
		else if (strncmp(tag, "mka", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_setgwconfig->mka));
			DEBUG_DEC(err, "req_set_gw_config: mka");
		}
		else if (strncmp(tag, "lss", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_setgwconfig->lss));
			DEBUG_DEC(err, "req_set_gw_config: lss");
		}
		else if (strncmp(tag, "hss", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, ((int64_t*)&cbor_setgwconfig->hss));
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "send mb adu request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "seq", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, (int64_t*)seq);
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "send_mb_pass_through request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "pas", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, (int64_t*)cbor_pass);
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

	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "reboot request map");

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "cid", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, (int64_t*)cid);
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

			if(PollEngine__GetEngineStatus() == RUNNING){
				PollEngine__StopEngine();
//				MQTT__FlushValues();
			}
//			GME__Reboot();		//todo
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
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			if(SUCCESS_CMD == cbor_req.res)
				if(PollEngine__GetEngineStatus() == RUNNING){
					PollEngine__StopEngine();
//					MQTT__FlushValues();
			//		GME__Reboot();		//todo
			}
		}
		break;

		case SET_DEVS_CONFIG:
		{
			c_cborreqdwldevsconfig download_devs_config = {0};
			err = CBOR_ReqSetDevsConfig(cbor_stream, cbor_len, &download_devs_config);

			// write new data to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = (execute_download_devs_config(download_devs_config) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;

			// mqtt response
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			if(SUCCESS_CMD == cbor_req.res){
				if(PollEngine__GetEngineStatus() == RUNNING){
					PollEngine__StopEngine();
//					MQTT__FlushValues();
//					GME__Reboot();		//todo
				}
			}
		}
		break;

		case SCAN_DEVICES:
		{
			C_UINT16 device = 0;
			// scan Modbus line

			// save address of first responding device in device and corresponding answer in answer
			// answer with result
			if(ACTIVETED == PollEngine__GetPassModeStatus()){		//ACTIVATED!!!

				C_BYTE answer[REPORT_SLAVE_ID_SIZE];
				C_INT16 length;
				cbor_req.res = SUCCESS_CMD;

//				PollEngine__MBSuspend();
//				PollEngine__SetPassModeCMD(RECEIVED);

				length = execute_scan_devices(answer);
				if(length <= 0){
					memcpy(answer, "", 1);
					length=1;
				}
				device = answer[0];
				len = CBOR_ResScanLine(cbor_response, &cbor_req, device, answer, length);
				mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

//				PollEngine__SetPassModeCMD(EXECUTED);
//				PollEngine__MBResume();
			}

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
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			if(SUCCESS_CMD == cbor_req.res){
				if(PollEngine__GetEngineStatus() == RUNNING){
					PollEngine__StopEngine();
//					MQTT__FlushValues();
//					GME__Reboot();		//todo
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
			cbor_req.res = 1; // todo
			C_UINT16 adulen = 8; // todo
			adu[0]=0x01;
			adu[1]=0x03;
			adu[2]=0x00;
			adu[3]=0x01;
			adu[4]=0x00;
			adu[5]=0x01;
			adu[6]=0xAA;
			adu[7]=0xBB;
			// mqtt response
			// TODO BILATO to be implemented as Cellini suggested
			// len = CBOR_ResSendMbAdu(cbor_response, &cbor_req, seq, adu, adulen);
			
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			// restart polling machine?
/*EGISIAN
			if(ACTIVETED == PollEngine__GetPassModeStatus()){
				PollEngine__MBSuspend();
				PollEngine__SetPassModeCMD(RECEIVED);

				uint8_t data_rx[20] = {0};
				uint8_t data_rx_len;
				req_send_mb_adu_t send_mb_adu = {0};
				parse_send_mb_adu(root, &send_mb_adu);
				data_rx_len = PollEngine__SendMBAdu(&send_mb_adu, data_rx);

				if(data_rx_len <= 0){
					send_mb_adu_res(ReqHeader.replyTo, SEND_MB_ADU, data_rx, data_rx_len, send_mb_adu.sequence, MQTT__GetClient());
				}else{
					send_mb_adu_res(ReqHeader.replyTo, SEND_MB_ADU, data_rx, data_rx_len, send_mb_adu.sequence, MQTT__GetClient());
				}
				PollEngine__SetPassModeCMD(EXECUTED);

				memmgr_free(send_mb_adu.adu);
				PollEngine__SetPassModeCMD(EXECUTED);
				PollEngine__MBResume();
			}
//EGISIAN*/
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

			// send modbus command to write values
			// wait modbus response to get result
			C_CHAR val[VAL_SIZE];
			memcpy(val, "1.2", sizeof("1.2"));
			cbor_req.res = 1; // todo
			// send response with result
			len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_rv.alias, val);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			// put polling machine in some specific status?
/*EGISIAN
			if(ACTIVETED == PollEngine__GetPassModeStatus()){
				PollEngine__SetPassModeCMD(RECEIVED);
				parse_read_values(root, ReqHeader.replyTo);
				PollEngine__SetPassModeCMD(EXECUTED);
			}
//EGISIAN*/
		}
		break;


		case WRITE_VALUES:
		{
			// which status should be polling engine?
			// should I check it is in a specific status?
			// what to do if it is not in that status?

			c_cborreqrdwrvalues cbor_wv = {0};
			len = CBOR_ReqRdWrValues(cbor_stream, cbor_len, &cbor_wv);

			// send modbus command to write values
//			if(ACTIVETED == PollEngine__GetPassModeStatus())
//			{
//				PollEngine__SetPassModeCMD(RECEIVED);
				cbor_req.res = (parse_write_values(cbor_wv) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
				// send response with result
				len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_wv.alias, NULL);
				mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
//				PollEngine__SetPassModeCMD(EXECUTED);
//			}
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
				MQTT__FlushValues();
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
/*EGISIAN
			bool previous_poll_engine_status = false;
			req_update_dev_fw_t update_dev_fw = {0};
			parse_update_dev_fw(root, &update_dev_fw);

			if(PollEngine__GetEngineStatus() == RUNNING){
				PollEngine__StopEngine();
				MQTT__FlushValues();
				previous_poll_engine_status = true;
			}

			if(ESP_OK == OTA__DevFWInit(&update_dev_fw)){
				send_simple_res(ReqHeader.replyTo, UPDATE_DEV_FIRMWARE, SUCCESS_CMD, MQTT__GetClient());
				GME__Reboot();
			}else{
				send_simple_res(ReqHeader.replyTo, UPDATE_DEV_FIRMWARE, ERROR_CMD, MQTT__GetClient());
			}

			if(previous_poll_engine_status == true){
				PollEngine__StartEngine();
			}
//EGISIAN*/
		}
		break;


		case UPDATE_CA_CERTIFICATES:
		{
			c_cborreqdwldevsconfig update_ca_config = {0};
			err = CBOR_ReqUpdateCaCertificate(cbor_stream, cbor_len, &update_ca_config);

			// perform a https read file from uri, using usr and pwd authentication data
			cbor_req.res = 1; // todo

			// send a report of operation
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			// reboot?
/*EGISIAN
			if(ESP_OK == execute_update_ca_cert(&update_ca_cert)){
				send_simple_res(ReqHeader.replyTo, UPDATE_CA_CERTIFICATES, SUCCESS_CMD, MQTT__GetClient());
				if(PollEngine__GetEngineStatus() == RUNNING){
					PollEngine__StopEngine();
					MQTT__FlushValues();
					GME__Reboot();
				}
			}else{
				send_simple_res(ReqHeader.replyTo, UPDATE_CA_CERTIFICATES, ERROR_CMD, MQTT__GetClient());
			}
			memmgr_free(update_ca_cert.username);
			memmgr_free(update_ca_cert.password);
			memmgr_free(update_ca_cert.uri);
//EGISIAN*/
		}
		break;


		case CHANGE_CREDENTIALS:
		{
			c_cborreqdwldevsconfig change_cred_config = {0};
			err = CBOR_ReqChangeCredentials(cbor_stream, cbor_len, &change_cred_config);

			// write new credentials to configuration file and put in res the result of operation
			cbor_req.res = (execute_change_cred(change_cred_config) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			// mqtt response
			// to be implemented
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);

			if(SUCCESS_CMD == cbor_req.res){
				if(PollEngine__GetEngineStatus() == RUNNING){
					PollEngine__StopEngine();
//					MQTT__FlushValues();
//					GME__Reboot();		//todo
				}
			}
		}
		break;


		case START_ENGINE:
		{
			PollEngine__StartEngine();
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;


		case STOP_ENGINE:
		{
			PollEngine__StopEngine();
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;

		case SEND_MB_PASS_THROUGH:
		{
			C_UINT16 cbor_pass;
			err = CBOR_ReqSendMbPassThrough(cbor_stream, cbor_len, &cbor_pass);
			// change pass through mode or just get status
			PollEngine__ActivatePassMode();

			len = CBOR_ResSendMbPassThrough(cbor_response, &cbor_req, cbor_pass);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;

		default:
			cbor_req.res = INVALID_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		break;

	}

	return 0;
}



C_RES execute_set_line_config(C_UINT32 new_baud_rate, C_BYTE new_connector){

	C_RES err = NVM__WriteU32Value(MB_BAUDRATE_NVM, new_baud_rate);
	err |= NVM__WriteU8Value(MB_CONNECTOR_NVM, new_connector);
	err |= NVM__WriteU8Value(SET_LINE_CONFIG_NVM, CONFIGURED);

	return err;
}

C_RES execute_download_devs_config(c_cborreqdwldevsconfig download_devs_config){
#if 0
	https_conn_err_t err;


	printf("execute_download_devs_config \n");

	err = HttpsClient__DownloadModelFile(download_devs_config,CERT_1);

	if(CONN_FAIL == err){
		err = HttpsClient__DownloadModelFile(download_devs_config,CERT_2);

	}else if(FILE_NOT_SAVED == err){
		return ESP_FAIL;
	}

	printf("execute_download_devs_config err= %d \n",err);
	if(CONN_OK == err){

		if(ESP_OK == NVM__WriteU8Value(SET_DEVS_CONFIG_NVM, CONFIGURED)){
			printf("MODEL FILE SAVED\n");
			err = ESP_OK;
		}else{
			printf("MODEL FILE NOT SAVED\n");
			err = ESP_FAIL;
		}
	}else{
		return ESP_FAIL;
	}

	return ESP_OK;
#endif
	return 1;
}


C_RES execute_set_gw_config(c_cborreqsetgwconfig set_gw_config)
{
	printf("Execute Set GW Config\n");
	//c_cborreqsetgwconfig gw_config_nvm = {0};	// todo: is a copy actually needed?
	size_t len = 0;
	C_BYTE gw_config_status;

	if(C_SUCCESS == NVM__ReadU8Value(SET_GW_CONFIG_NVM, &gw_config_status) && CONFIGURED == gw_config_status){
		NVM__ReadBlob(SET_GW_PARAM_NVM,(void*)&set_gw_config,&len);
	}
/*
	gw_config_nvm.hss = set_gw_config.hss;
	gw_config_nvm.lss = set_gw_config.lss;
	gw_config_nvm.mka = set_gw_config.mka;
	gw_config_nvm.pst = set_gw_config.pst;
	gw_config_nvm.pva = set_gw_config.pva;
*/
	C_RES err = NVM__WriteBlob(SET_GW_PARAM_NVM,(void*)&set_gw_config,sizeof(set_gw_config));
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

C_INT16 execute_scan_devices(C_BYTE* data_rx){

	C_INT16 len = -2;
#if 0
	mbc_master_suspend();

	C_BYTE data_tx[4] = {0};
	int i = 0;
	data_tx[0]=	line_id;
	data_tx[1]=	0x11;	//cmd 17

	C_UINT16 crc = CRC16(data_tx, 2 );

	data_tx[2] = (C_BYTE)(crc & 0x00ff);
	data_tx[3] = (C_BYTE)((crc>>8) & 0x00ff);

	if(PollEngine__GetPollEnginePrintMsgs() == 1){
		printf("Scan Line Request Packet:  ");
		for(i=0; i<sizeof(data_tx); i++){
				printf("[%d]=%02X  ",i,data_tx[i]);
			}
		printf("\n\n");
	}

	ClearQueueMB();

	for(int i=0; i<4; i++)
	{
		uart_write_bytes(MB_PORTNUM, (const char *) &data_tx[i], 1);
	}

	len = uart_read_bytes(MB_PORTNUM, data_rx, 255,  MB_RESPONSE_TIMEOUT(4));

	if(PollEngine__GetPollEnginePrintMsgs() == 1){
		printf("\nuart_read_bytes len = %d\n\n",len);

		for(i=0; i<len; i++){
			printf("[%d]=%02X  ",i,data_rx[i]);
		}
		printf("\n\n");
	}

	uart_flush_input(MB_PORTNUM);
	uart_flush(MB_PORTNUM);

	ClearQueueMB();
	mbc_master_resume();

	vTaskDelay(1000 / portTICK_PERIOD_MS);
#endif
	return len;
}

C_RES PollEngine__Write_COIL_Req(C_CHAR* alias, C_UINT16 write_value, C_UINT16 addr){
	printf("write_coil: alias %s, addr %d, val %d\n", alias, addr, (C_UINT16)write_value);
	return 0;
}

C_RES PollEngine__Write_HR_Req(C_CHAR* alias, void* write_value){
	printf("write_hr: alias %s, val %d\n", alias, (C_UINT16)write_value);
	return 0;
}

C_RES parse_write_values(c_cborreqrdwrvalues cbor_wv)
{
	C_RES result = C_FAIL;
	// todo da gestire tutte le conversioni di tipo nei vari casi
	//C_INT32 val_to_write = atof((C_SCHAR*)cbor_wv.val);
	C_FLOAT val_to_write = atof((C_SCHAR*)cbor_wv.val);
	switch(cbor_wv.func){
		case mbW_COIL:{
			printf("Alias: %s, Addr = %d, val to write: %d \n",cbor_wv.alias, cbor_wv.addr, (C_UINT16)val_to_write);
			result = PollEngine__Write_COIL_Req(cbor_wv.alias, val_to_write, cbor_wv.addr);
			printf("OPERATION_RESULT %d\n",result);
		}
		break;

		case mbW_HR:{
			if(cbor_wv.flags.bit.fixedpoint == 1  ||  cbor_wv.flags.bit.ieee == 1)
				val_to_write = (val_to_write - (long double)(atoi((C_SCHAR*)cbor_wv.a)))  /  (long double)(atoi((C_SCHAR*)cbor_wv.b));

			printf("Alias = %s, Addr = %d val_to_write = %.6f\n", cbor_wv.alias, cbor_wv.addr, val_to_write);
			result = PollEngine__Write_HR_Req(cbor_wv.alias, (void*)&val_to_write);
			printf("OPERATION_RESULT %d\n",result);
		}
		break;

		default:
		break;

	}
	return result;
}


