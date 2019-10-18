/**
 * @file CBOR_CAREL.c
 * @author Carel
 * @date 12 Sep 2019
 * @brief Carel CBOR glue logic
 * Functions performing encoding need a payload buffer to be passed as reference in order to be filled with required info.
 * This buffer needs to be previously statically allocated (with proper size)
 *      
 */
#include "data_types_CAREL.h" 
#include "CBOR_CAREL.h"
#include "File_System_CAREL.h"
#include "cbor.h"
#include "Miscellaneous_IS.h"
#include "RTC_IS.h"
#include "ccl_manager_CAREL.h"
//#include "MQTT_Interface_IS.h"
//#include "MQTT_Interface_CAREL.h"

/* Exported types ------------------------------------------------------------*/ 


/* Exported constants --------------------------------------------------------*/

C_CHAR txbuff[1000];



void CBOR_SendAlarms(c_cboralarms cbor_alarms)
{
	CBOR_Alarms(txbuff, cbor_alarms);
//	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/alarms"), (C_SBYTE*)txbuff, 0, 0, 0);
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

void CBOR_SendHello(void)
{
	CBOR_Hello(txbuff);
//	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/hello"), (C_SBYTE*)txbuff, 0, 1, 1);
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
	Get_RS485_BaudRate(&baud_rate);
	err |= cbor_encode_uint(&mapEncoder, baud_rate);
	DEBUG_ADD(err, "baud rate");

	// encode mqv - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "mqv");
	C_BYTE MQTTver[3] = {3, 1, 1};
	err |= cbor_encode_byte_string(&mapEncoder, MQTTver, 3);
	DEBUG_ADD(err, "mqttversion");

	// encode dev - elem8
	err |= cbor_encode_text_stringz(&mapEncoder, "dev");
	C_UINT16 device_address;
	Get_Device_Address(&device_address);
	err |= cbor_encode_uint(&mapEncoder, device_address);
	DEBUG_ADD(err, "dev");

	// encode gid - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "gid");
	C_BYTE Guid[16];
	Get_Guid(Guid);
	err |= cbor_encode_byte_string(&mapEncoder, Guid, 16);
	DEBUG_ADD(err, "guid");
	
	//encode crc - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "crc");
	C_UINT16 crc = 0;													// to be implemented
	err |= cbor_encode_uint(&mapEncoder, crc);
	DEBUG_ADD(err, "crc");
	
	//encode cid - elem11
	err |= cbor_encode_text_stringz(&mapEncoder, "cid");
	C_UINT16 cid=0;														// to be implemented
	err |= cbor_encode_uint(&mapEncoder, cid);
	DEBUG_ADD(err, "cid");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}


void CBOR_SendStatus(void)
{
	CBOR_Status(txbuff);
//	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/status"), (C_SBYTE*)txbuff, 0, 0, 0);
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
	t=1571216311;
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "t");
	
	// encode upt - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "upt");
	//err |= cbor_encode_uint(&mapEncoder, t - RTC_Get_UTC_Boot_Time());
	err |= cbor_encode_uint(&mapEncoder, 347);
	DEBUG_ADD(err, "upt");
	
	// encode fme -elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "fme");
	C_UINT32 freemem = 2365; 												// to be implemented
	err |= cbor_encode_uint(&mapEncoder, freemem);
	DEBUG_ADD(err,"upt");
	
	// encode est -elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "est");
	//err |= cbor_encode_uint(&mapEncoder, Get_Polling_Status());
	err |= cbor_encode_uint(&mapEncoder, 1);
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

extern db_values val_array[10];

C_UINT32 Get_Counter(C_UINT16 index){return val_array[index].cnt;}
C_TIME Get_SamplingTime(C_UINT16 index){return val_array[index].t;}
C_CHAR a[1]; 
C_CHAR* Get_Alias(C_UINT16 index, C_UINT16 i){return val_array[index].vls[i].alias;}
C_CHAR* Get_Value(C_UINT16 index, C_UINT16 i){return val_array[index].vls[i].values;}


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


void CBOR_SendValues(C_UINT16 index, C_UINT16 number, C_INT16 frame)
{
	C_CHAR mybuf[500];
int i;
	size_t len = CBOR_Values(mybuf, index, number, frame);
	printf("valuespkt len %d: \n", len);
	for (i=0;i<len;i++){
		printf("%02X ", mybuf[i]);
	}
	printf("\n");
	printf("values pkt binary: \n");
	for (i=0;i<len;i++){
			printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(mybuf[i]));
	}
	printf("\n");
//	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/values"), (C_SBYTE*)txbuff, 0, 0, 0);
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
	//C_TIME t = RTC_Get_UTC_Boot_Time();
	C_TIME t = 1571220602;
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "btm");
	
	// encode t - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
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
 * @param Result of the operation
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

		printf("response len %d: \n", len);
		for (int i=0;i<len;i++){
			printf("%02X ", cbor_response[i]);
		}
		printf("\n");
		printf("response pkt binary: \n");
		for (int i=0;i<len;i++){
				printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(cbor_response[i]));
			}
			printf("\n");

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
 * @return void
 */
size_t CBOR_ResScanLine(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 device, C_CHAR* answer)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;
	
	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);
	
	// encode lid - elem5
//	err = cbor_encode_text_stringz(&mapEncoder, "lid");
//	err |= cbor_encode_int(&mapEncoder, 1); //TODO ---> do we really need it?
//	DEBUG_ADD(err, "lid");

	// encode dev - elem6
	err = cbor_encode_text_stringz(&mapEncoder, "dev");
	err |= cbor_encode_int(&mapEncoder, device); 
	DEBUG_ADD(err, "dev");

	// encode answer - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "ret");
	err |= cbor_encode_text_stringz(&mapEncoder, (char*)answer);
	DEBUG_ADD(err, "ret");

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
 * @param Result of Address of first responding device
 * @param Answer of the first responding device to Modbus command ReportSlaveId (command 17)
 * @return void
 */
size_t CBOR_ResSendMbAdu(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 seq, C_CHAR* val)
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
	err |= cbor_encode_text_stringz(&mapEncoder, (char*)val);
	DEBUG_ADD(err, "val");
	
	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ResWriteValues
 * 
 * Prepares CBOR encoded message containing result of write values
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Pointer to alias of written value
 * @return void
 */
size_t CBOR_ResWriteValues(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_CHAR* ali)
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
	err |= cbor_encode_null(&mapEncoder);
	DEBUG_ADD(err, "val");
	
	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_response);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief CBOR_ResReadValues
 * 
 * Prepares CBOR encoded message containing result of read values
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Pointer to alias of written value
 * @return void
 */
size_t CBOR_ResReadValues(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_CHAR* ali, C_CHAR* val)
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
	err |= cbor_encode_text_stringz(&mapEncoder, (char*)val);
	DEBUG_ADD(err, "val");
	
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
 * @param Pointer to the request struct
 * @param Pointer to CBOR container
 * @param Pointer to first element of the CBOR container
 * @return CborError
 */
CborError CBOR_ReqHeader(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborhreq* cbor_req, CborValue* it, CborValue* recursed)
{
	// parse request header and get replyTo and cmd fields
	// write them to cbor_req
	CborParser parser;
	size_t stlen;
    char tag[TAG_SIZE];
	char rto[REPLYTO_SIZE];
	CborError err;
	C_INT16 val;
	
	err = cbor_parser_init(cbor_stream, cbor_len, 0, &parser, it);
	err |= cbor_value_enter_container(it, recursed);
	DEBUG_DEC(err, "header request map");
	
	stlen = TAG_SIZE;
	err |= cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, &val);
	cbor_req->ver = val;
	err |= cbor_value_advance_fixed(recursed);
	DEBUG_DEC(err, "header: ver");

	stlen = TAG_SIZE;
	err |= cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = RTO_SIZE;
	err |= cbor_value_copy_text_string(recursed, rto, &stlen, recursed);
	strcpy((char*)cbor_req->rto, rto);
	DEBUG_DEC(err, "header: rto");

	stlen = TAG_SIZE;
	err |= cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	cbor_value_get_int(recursed, &val);
	cbor_req->cmd = val;
	err |= cbor_value_advance_fixed(recursed);
	DEBUG_DEC(err, "header: cmd");
	
	return err;
}

/**
 * @brief CBOR_ReqSetLinesConfig
 * 
 * Interprets CBOR set lines config request
 *
 * @param Pointer to the CBOR current element 
 * @param Pointer to new baud rate
 * @return CborError
 */
CborError CBOR_ReqSetLinesConfig(CborValue* recursed, C_UINT32* new_baud_rate, C_BYTE* new_connector)
{
	CborError err;
	C_UINT64 val;
	C_INT16 con;
	size_t stlen;
	char tag[TAG_SIZE];

	stlen = TAG_SIZE;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_uint64(recursed, &val);
	*new_baud_rate = val;
	DEBUG_DEC(err, "req_set_lines_config: bau");
	
	stlen = TAG_SIZE;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, &con);
	*new_connector = con;
	DEBUG_DEC(err, "req_set_lines_config: con");
	
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
CborError CBOR_ReqSetDevsConfig(CborValue* recursed, C_SCHAR* usr, C_SCHAR* pwd, C_SCHAR* uri, C_UINT16* cid)
{
	CborError err;
	C_UINT16 val;
	size_t stlen;
	char tag[TAG_SIZE];
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = USERNAME_SIZE;
	err |= cbor_value_copy_text_string(recursed, usr, &stlen, recursed);
	DEBUG_DEC(err, "req_set_devs_config: usr");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = PASSWORD_SIZE;
	err |= cbor_value_copy_text_string(recursed, pwd, &stlen, recursed);
	DEBUG_DEC(err, "req_set_devs_config: pwd");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = URI_SIZE;
	err |= cbor_value_copy_text_string(recursed, uri, &stlen, recursed);
	DEBUG_DEC(err, "req_set_devs_config: uri");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)&val);
	*cid = val;
	DEBUG_DEC(err, "req_set_devs_config: cid");
	
	return err;	
}

/**
 * @brief CBOR_ReqChangeCredentials
 * 
 * Interprets CBOR change credentials
 *
 * @param Pointer to the CBOR current element 
 * @param Pointer to new username
 * @param Pointer to new password
 * @return CborError
 */
CborError CBOR_ReqChangeCredentials(CborValue* recursed, C_SCHAR* usr, C_SCHAR* pwd)
{
	CborError err;
	size_t stlen;
	char tag[TAG_SIZE];
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = USERNAME_SIZE;
	err |= cbor_value_copy_text_string(recursed, usr, &stlen, recursed);
	DEBUG_DEC(err, "req_change_cred: usr");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = PASSWORD_SIZE;
	err |= cbor_value_copy_text_string(recursed, pwd, &stlen, recursed);
	DEBUG_DEC(err, "req_change_cred: pwd");

	return err;
}

/**
 * @brief CBOR_ReqReadValues
 * 
 * Interprets CBOR write values request
 *
 * @param Pointer to the CBOR current element 
 * @param Pointer to structure for read values command
 * @return CborError
 */
CborError CBOR_ReqReadValues(CborValue* recursed, c_cborreqreadvalues* cbor_rv)
{
	CborError err;
	size_t stlen;
	char tag[TAG_SIZE];
	C_INT16 flg;
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = ALIAS_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_rv->alias, &stlen, recursed);
	DEBUG_DEC(err, "req_read_values: ali");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_rv->func);
	DEBUG_DEC(err, "req_read_values: fun");
		
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_rv->addr);
	DEBUG_DEC(err, "req_read_values: adr");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_rv->dim);
	DEBUG_DEC(err, "req_read_values: dim");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_rv->pos);
	DEBUG_DEC(err, "req_read_values: pos");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_rv->len);
	DEBUG_DEC(err, "req_read_values: len");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = A_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_rv->a, &stlen, recursed);
	DEBUG_DEC(err, "req_read_values: a");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = B_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_rv->b, &stlen, recursed);
	DEBUG_DEC(err, "req_read_values: b");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, &flg);
	cbor_rv->flags = flg;
	DEBUG_DEC(err, "req_read_values: flg");
	
	return err;
}

/**
 * @brief CBOR_ReqWriteValues
 * 
 * Interprets CBOR write values request
 *
 * @param Pointer to the CBOR current element 
 * @param Pointer to structure for write values command
 * @return CborError
 */
CborError CBOR_ReqWriteValues(CborValue* recursed, c_cborreqwritevalues* cbor_wv)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE]={'\0'};

	while (!cbor_value_at_end(recursed)) {
		stlen = 3;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);

		if (strncmp(tag, "ali", 3) == 0)
		{
			char text[30];
			stlen = ALIAS_SIZE;
			err |= cbor_value_copy_text_string(recursed, text, &stlen, recursed);
			memcpy(cbor_wv->alias, text, stlen);
			DEBUG_DEC(err, "req_write_values: ali");
		}
		else if (strncmp(tag, "val", 3) == 0)
		{
			char text[30];
			stlen = VAL_SIZE;
			err |= cbor_value_copy_text_string(recursed, text, &stlen, recursed);
			memcpy(cbor_wv->val, text, stlen);
			DEBUG_DEC(err, "req_write_values: val");
		}
		else if (strncmp(tag, "fun", 3) == 0)
		{
			CBOR_ExtractInt(recursed, ((int64_t*)&cbor_wv->func));
			DEBUG_DEC(err, "req_write_values: fun");
		}
		else if (strncmp(tag, "adr", 3) == 0)
		{
			CBOR_ExtractInt(recursed, ((int64_t*)&cbor_wv->addr));
			DEBUG_DEC(err, "req_write_values: adr");
		}
		else if (strncmp(tag, "dim", 3) == 0)
		{
			CBOR_ExtractInt(recursed, ((int64_t*)&cbor_wv->dim));
			DEBUG_DEC(err, "req_write_values: dim");
		}
		else if (strncmp(tag, "pos", 3) == 0)
		{
			CBOR_ExtractInt(recursed, ((int64_t*)&cbor_wv->pos));
			DEBUG_DEC(err, "req_write_values: pos");
		}
		else if (strncmp(tag, "len", 3) == 0)
		{
			CBOR_ExtractInt(recursed, ((int64_t*)&cbor_wv->len));
			DEBUG_DEC(err, "req_write_values: len");
		}
		else if (strncmp(tag, "a", 3) == 0)
		{
			char text[30];
			stlen = A_SIZE;
			err |= cbor_value_copy_text_string(recursed, text, &stlen, recursed);
			memcpy(cbor_wv->a, text, stlen);
			DEBUG_DEC(err, "req_write_values: a");
		}
		else if (strncmp(tag, "b", 3) == 0)
		{
			char text[30];
			stlen = B_SIZE;
			err |= cbor_value_copy_text_string(recursed, text, &stlen, recursed);
			memcpy(cbor_wv->b, text, stlen);
			DEBUG_DEC(err, "req_write_values: b");
		}
		else if (strncmp(tag, "flg", 3) == 0)
		{
			CBOR_ExtractInt(recursed, ((int64_t*)&cbor_wv->flags));
			DEBUG_DEC(err, "req_write_values: flg");
		}
	}
	return err;
}

C_INT16 CBOR_ExtractInt(CborValue* recursed, int64_t* read)
{
	CborError err = cbor_value_get_int64(recursed, read);
	err |= cbor_value_advance_fixed(recursed);
	return err;
}
#if 0
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = ALIAS_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_wv->alias, &stlen, recursed);
	DEBUG_DEC(err, "req_write_values: ali");

	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	CborType type = cbor_value_get_type(recursed);
	printf("type: %d\n", type);
	err |= cbor_value_get_int64(recursed, (C_INT16*)&cbor_wv->func);
	DEBUG_DEC(err, "req_write_values: fun");

	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_wv->addr);
	DEBUG_DEC(err, "req_write_values: adr");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_wv->dim);
	DEBUG_DEC(err, "req_write_values: dim");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_wv->pos);
	DEBUG_DEC(err, "req_write_values: pos");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_wv->len);
	DEBUG_DEC(err, "req_write_values: len");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = A_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_wv->a, &stlen, recursed);
	DEBUG_DEC(err, "req_write_values: a");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = B_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_wv->b, &stlen, recursed);
	DEBUG_DEC(err, "req_write_values: b");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, &flg);
	cbor_wv->flags = flg;
	DEBUG_DEC(err, "req_write_values: flg");

	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = VAL_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)cbor_wv->val, &stlen, recursed);
	DEBUG_DEC(err, "req_write_values: val");

#endif

/**
 * @brief CBOR_ReqSetGwConfig
 * 
 * Interprets CBOR set gw config
 *
 * @param Pointer to the CBOR current element 
 * @param Pointer to structure for set gw config command
 * @return CborError
 */
CborError CBOR_ReqSetGwConfig(CborValue* recursed, c_cborreqsetgwconfig* cbor_setgwconfig)
{
	CborError err;
	size_t stlen;
	char tag[TAG_SIZE];
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_setgwconfig->pva);
	DEBUG_DEC(err, "req_set_gw_config: pva");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_setgwconfig->pst);
	DEBUG_DEC(err, "req_set_gw_config: pst");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_setgwconfig->mka);
	DEBUG_DEC(err, "req_set_gw_config: mka");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_setgwconfig->lss);
	DEBUG_DEC(err, "req_set_gw_config: lss");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)cbor_setgwconfig->hss);
	DEBUG_DEC(err, "req_set_gw_config: hss");
	
	return err;
}

/**
 * @brief CBOR_ReqSendMbAdu
 * 
 * Interprets CBOR send mb adu
 *
 * @param Pointer to the CBOR current element
 * @param Pointer to the field seq used to detect wrong order in message de-queueing
 * @param Pointer to the buffer containing modbus request
 * @return CborError
 */
CborError CBOR_ReqSendMbAdu(CborValue* recursed, C_UINT16* seq, C_CHAR* adu)
{
	CborError err;
	size_t stlen;
	char tag[TAG_SIZE];
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, (C_INT16*)seq);
	DEBUG_DEC(err, "req_send_mb_adu: seq");
	
	stlen = 3;
	err = cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	stlen = ADU_SIZE;
	err |= cbor_value_copy_text_string(recursed, (char*)adu, &stlen, recursed);
	
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
	CborValue recursed, it;
	CborError err;
	C_CHAR cbor_response[RESPONSE_SIZE];		// buffer to store response, maybe better global...
	size_t len;
	
	C_MQTT_TOPIC topic;
	C_GATEWAY_ID dev_id;
	Get_Gateway_ID(&dev_id);
	strcpy((char*)topic,(char*)dev_id); 
			
	err = CBOR_ReqHeader(cbor_stream, cbor_len, &cbor_req, &it, &recursed);
	
	switch(cbor_req.cmd){
		case SET_GW_CONFIG:
		{
			c_cborreqsetgwconfig cbor_setgwconfig = {0};
			err = CBOR_ReqSetGwConfig(&recursed, &cbor_setgwconfig);
			err = cbor_value_advance_fixed(&recursed);
			err = cbor_value_leave_container(&it, &recursed);
			
			// write new data to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = 1; // todo
			
			// mqtt response 
			// to be implemented
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// reboot (is it needed to stop polling and flush data before rebooting????)
			// to be implemented
		}
		break;

		case SET_DEVS_CONFIG:
		{
			C_USERNAME usr;
			C_PASSWORD pwd;
			C_URI uri;
			C_UINT16 cid;
			err = CBOR_ReqSetDevsConfig(&recursed, usr, pwd, uri, &cid);
			err = cbor_value_advance_fixed(&recursed);
			err = cbor_value_leave_container(&it, &recursed);
			
			// write new data to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = 1; // todo
			
			// mqtt response 
			// to be implemented
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// reboot (is it needed to stop polling and flush data before rebooting????)
			// to be implemented
		}
		break;

		case SCAN_DEVICES:
		{
			// scan Modbus line
			
			
			// save address of first responding device in device and corresponding answer in answer
			C_UINT16 device = 1; 								// to be implemented
			C_CHAR answer[REPORT_SLAVE_ID_SIZE]={0};
			// answer with result
			cbor_req.res = 1; // todo
			len = CBOR_ResScanLine(cbor_response, &cbor_req, device, answer);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
		}
		break;

		case SET_LINES_CONFIG:
		{
			
			C_UINT32 new_baud_rate;
			C_BYTE new_connector;
			err = CBOR_ReqSetLinesConfig(&recursed, &new_baud_rate, &new_connector);
			err = cbor_value_advance_fixed(&recursed);
			err = cbor_value_leave_container(&it, &recursed);
			
			// write new baud rate and connector to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = 1; // todo

			// mqtt response 
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// reboot (is it needed to stop polling and flush data before rebooting????)
			// to be implemented
			
		}
		break;


		case SEND_MB_ADU:
		{
			C_UINT16 seq = 0;
			C_CHAR adu[ADU_SIZE];
			CBOR_ReqSendMbAdu(&recursed, &seq, adu);

			// execute command (when polling machine available) and gather result
			// put modbus answer in adu buffer to reuse resources and save memory
			cbor_req.res = 1; // todo
			
			// mqtt response
			// to be implemented
			len = CBOR_ResSendMbAdu(cbor_response, &cbor_req, seq, adu);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// restart polling machine?
		}
		break;

		// TODO: evaluate to collapse read values and write values code portions into a single block to optimize memory occupation
		case READ_VALUES:
		{
			// which status should be polling engine?
			// should I check it is in a specific status?
			// what to do if it is not in that status?
		
			c_cborreqreadvalues cbor_rv = {0};
			len = CBOR_ReqReadValues(&recursed, &cbor_rv);
			
			// send modbus command to write values
			// wait modbus response to get result
			C_CHAR val[VAL_SIZE];
			cbor_req.res = 1; // todo
			// send response with result
			CBOR_ResReadValues(cbor_response, &cbor_req, cbor_rv.alias, val);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// put polling machine in some specific status?
			
		}
		break;


		case WRITE_VALUES:
		{
			// which status should be polling engine?
			// should I check it is in a specific status?
			// what to do if it is not in that status?
			
			c_cborreqwritevalues cbor_wv = {0};
			len = CBOR_ReqWriteValues(&recursed, &cbor_wv);
			
			// send modbus command to write values
			// wait modbus response to get result
			cbor_req.res = 1; // todo
			
			// send response with result
			len = CBOR_ResWriteValues(cbor_response, &cbor_req, cbor_wv.alias);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// put polling machine in some specific status?
			
		}
		break;


		case UPDATE_GME_FIRMWARE:
		{
		/*	req_update_gw_fw_t update_gw_fw = {0};
			parse_update_gw_fw(root, &update_gw_fw);
			memmgr_free(update_gw_fw.username);
			memmgr_free(update_gw_fw.password);
			memmgr_free(update_gw_fw.uri);
		*/}
		break;


		case UPDATE_DEV_FIRMWARE:
		{
		/*	req_update_dev_fw_t update_dev_fw = {0};
			parse_update_dev_fw(root, &update_dev_fw);
			memmgr_free(update_dev_fw.username);
			memmgr_free(update_dev_fw.password);
			memmgr_free(update_dev_fw.uri);
		*/}
		break;


		case UPDATE_CA_CERTIFICATES:
		{
			C_USERNAME usr;
			C_PASSWORD pwd;
			C_URI uri;
			C_UINT16 cid;	// unica differenza con il payload set_devs_config... come gestire il fatto che qui non c'è il cid?
			err = CBOR_ReqSetDevsConfig(&recursed, usr, pwd, uri, &cid);
			err = cbor_value_advance_fixed(&recursed);
			err = cbor_value_leave_container(&it, &recursed);
			
			// perform a https read file from uri, using usr and pwd authentication data
			cbor_req.res = 1; // todo
			
			// send a report of operation
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// reboot?
		}
		break;


		case CHANGE_CREDENTIALS:
		{
			C_USERNAME usr;
			C_PASSWORD pwd;
			err = CBOR_ReqChangeCredentials(&recursed, usr, pwd);
			err = cbor_value_advance_fixed(&recursed);
			err = cbor_value_leave_container(&it, &recursed);
			
			// write new baud rate and connector to configuration file and put in res the result of operation
			// to be implemented
			cbor_req.res = 1; // todo
			
			// mqtt response 
			// to be implemented
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
			
			// reboot (is it needed to stop polling and flush data before rebooting????)
			// to be implemented
			
		}
		break;


		case START_ENGINE:
		{
			Set_Polling_Status(C_START);
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		}
		break;


		case STOP_ENGINE:
		{
			Set_Polling_Status(C_STOP);
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		/*
			PollEngine__ActivatePassMode();
		*/}
		break;


		default:
			cbor_req.res = INVALID_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
//			mqtt_client_publish((C_SCHAR*)topic, (C_SBYTE*)cbor_response, len, QOS_1, RETAIN);
		break;

	}
	
	return 0;
}

