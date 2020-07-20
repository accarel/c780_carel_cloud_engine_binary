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
#include "nvm_CAREL.h"
#include "binary_model.h"
#include "https_client_CAREL.h"
#include "ota_CAREL.h"
#include "ota_IS.h"
#include "sys_IS.h"
#include "wifi.h"
#include "main_CAREL.h"
#include "mobile.h"
#ifdef INCLUDE_PLATFORM_DEPENDENT
#include "mb_m.h"
#endif

/* Exported types ------------------------------------------------------------*/
#ifdef INCLUDE_PLATFORM_DEPENDENT
extern  CHAR     ucMBSlaveID[256];
extern  USHORT   usMBSlaveIDLen;
#endif

/* Exported constants --------------------------------------------------------*/

C_CHAR* txbuff;
uint16_t txbuff_len = 0;

C_UINT16 did;
c_cborhreq async_req = {0};
C_UINT16 async_cid = 0;

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
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/alarms"), (C_SBYTE*)txbuff, len, QOS_1, NO_RETAIN);
	//TODO CPPCHECK valore di ritorno non testato
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

	// encode did - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "did");
	err |= cbor_encode_int(&mapEncoder, CBOR_GetDid());
	DEBUG_ADD(err, "did");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__); 
        #endif
		len = -1;
	}

    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("alarmspkt len %d: \n", len);

	for (int i=0;i<len;i++)
	{
		printf("%02X ", cbor_stream[i]);
	}
	printf("\n");
    #endif

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
	//Allocate tx buffer
	uint32_t  freespace = uxTaskGetStackHighWaterMark(NULL);
	freespace -= 1000;
	txbuff_len = (uint16_t)freespace;

	//only to test fragmentation enable the below line
	//txbuff_len = 110;

	txbuff = malloc(txbuff_len);
	memset((void*)txbuff, 0, txbuff_len);

	size_t len = CBOR_Hello(txbuff);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/hello"), (C_SBYTE*)txbuff, len, QOS_1, NO_RETAIN);
	//TODO CPPCHECK valore di ritorno non testato
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
	err |= cbor_encode_text_stringz(&mapEncoder, CBOR_GetPartNumber());
	DEBUG_ADD(err, "part number");

	// encode hwv - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "hwv");
	err |= cbor_encode_text_stringz(&mapEncoder, GW_HW_REV);
	DEBUG_ADD(err, "hw version");

	// encode fwv - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "fwv");
	err |= cbor_encode_text_stringz(&mapEncoder, GW_FW_REV);
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

	// encode did - elem8
	err |= cbor_encode_text_stringz(&mapEncoder, "did");
	err |= cbor_encode_uint(&mapEncoder, CBOR_GetDid());
	DEBUG_ADD(err, "did");

	// encode gid - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "gid");
	err |= cbor_encode_byte_string(&mapEncoder, GME__GetHEaderInfo()->guid, 16);
	DEBUG_ADD(err, "guid");

	//encode crc - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "crc");
	C_UINT16 crc = BinaryModel_GetCrc();
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
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
		len = -1;
	}

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
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/status"), (C_SBYTE*)txbuff, len, QOS_0, NO_RETAIN);
	//TODO CPPCHECK valore di ritorno non testato
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
	C_UINT32 freemem = Sys__GetFreeHeapSize();
	err |= cbor_encode_uint(&mapEncoder, freemem);
	DEBUG_ADD(err,"fme");

	// encode est -elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "est");
	err |= cbor_encode_uint(&mapEncoder, PollEngine_GetEngineStatus_CAREL());
	DEBUG_ADD(err,"est");

	// encode sgn -elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "sgn");
	int8_t rssi = 0;
	rssi = Radio__GetRSSI();
	err |= cbor_encode_int(&mapEncoder, rssi);
	DEBUG_ADD(err,"sgn");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__); 
        #endif
		len = -1;
	}

	return len;
}

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
	size_t len = CBOR_Values(txbuff, index, number, frame);

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

    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("valuespkt len %d: \n", len);
	for (int i=0;i<len;i++){
		printf("%02X ", txbuff[i]);
	}
	printf("\n");
    #endif


#ifdef __DEBUG_CBOR_CAREL_LEV_3
	printf("values pkt binary: \n");
	for (int i=0;i<len;i++){
			printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(txbuff[i]));
	}
	printf("\n");
#endif

	C_RES err = mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/values"), (C_SBYTE*)txbuff, len, QOS_1, NO_RETAIN);
	//TODO CPPCHECK valore di ritorno non testato
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
	static C_UINT32 pkt_cnt = 0;
	char alias_tmp[ALIAS_SIZE];
	char value_tmp[VAL_SIZE];

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "values create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode cnt - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "cnt");
	err |= cbor_encode_uint(&mapEncoder, pkt_cnt);
	if (frame < 0)
		pkt_cnt++; 	// do not increment "cnt" field if message is fragmented (unless it is the last)
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
	for (C_UINT16 i = index; i < index + number; i++){
		err |= cbor_encode_text_stringz(&mapEncoder1, Get_Alias(i, alias_tmp));
		if (memcmp((char*)Get_Value(i, value_tmp), "", sizeof("")) == 0)
			err |= cbor_encode_null(&mapEncoder1);
		else
			err |= cbor_encode_text_stringz(&mapEncoder1, (char*)Get_Value(i, value_tmp));
	}
	err |= cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	// encode frm - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "frm");
	err |= cbor_encode_int(&mapEncoder, frame);
	DEBUG_ADD(err, "frm");

	// encode did - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "did");
	err |= cbor_encode_int(&mapEncoder, CBOR_GetDid());
	DEBUG_ADD(err, "did");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__); 
        #endif
		len = -1;
	}

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
 * This function considers the size of the tx buff and decides to fragment messages according to that
 */
void CBOR_SendFragmentedValues(C_UINT16 index, C_UINT16 number)
{
	C_INT16 framecnt = 1;
	// cbor values packet overhead is calculated based on the packet format... if it changes this number must be recalculated!!!
	C_UINT16 cborval_overhead = 50;
	// calculate values packet size based on maximum size of alias and values fields
	C_UINT16 values_size = number * (ALIAS_SIZE + VAL_SIZE + 2) + cborval_overhead;
	C_UINT16 entry_per_packet;

	// if a single packet cannot contain all data...
	if ( values_size > txbuff_len ) {
		// calculate the number of values that can be put in a single packet (according to tx buff size)...
		entry_per_packet = ( txbuff_len - cborval_overhead ) / (ALIAS_SIZE + VAL_SIZE + 2);

		while(number > entry_per_packet)
		{
			// ...and send fragmented packets...
			CBOR_SendValues(index, entry_per_packet, framecnt);
			index += entry_per_packet;
			number -= entry_per_packet;
			framecnt++;
		}
	}
	// ...until the last one
	CBOR_SendValues(index, number, -framecnt);

}

/**
 * @brief CBOR_SendMobile
 *
 * Prepares and sends an MQTT message containing the mobile message
 *
 * @param none
 * @return void
 */
void CBOR_SendMobile(void)
{
	size_t len = CBOR_Mobile(txbuff);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic("/mobile"), (C_SBYTE*)txbuff, len, QOS_0, NO_RETAIN);
	//TODO CPPCHECK valore di ritorno non testato
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
	// TODO valutare se usare RTC_Get_UTC_MQTTConnect_Time(); è sbagliato perché prende l'ora prima di essersi connesso all'ntp
	C_INT32 gup = Mobile__GetConnTime();
	if (gup == 0) gup = -1;
	printf("mobile payload, gup=%d\n", gup);
	err |= cbor_encode_int(&mapEncoder, gup);
	DEBUG_ADD(err, "gup");

	// encode sig - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "sig");
	C_INT16 sig = Mobile_GetSignalQuality();
	err |= cbor_encode_int(&mapEncoder, sig);
	DEBUG_ADD(err, "sig");

	// encode t - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
	C_TIME t = RTC_Get_UTC_Current_Time();
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "t");

	// encode ime - elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "ime");
	err |= cbor_encode_text_stringz(&mapEncoder, Mobile__GetImeiCode());
	DEBUG_ADD(err, "ime");

	// other elements (if needed)
	// encode ims - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "ims");
	err |= cbor_encode_text_stringz(&mapEncoder, Mobile__GetImsiCode());
	DEBUG_ADD(err, "ims");

	// following data can be obtained from AT+QENG?
	// encode mcc - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "mcc");
	err |= cbor_encode_text_stringz(&mapEncoder, Mobile__GetMccCode());
	DEBUG_ADD(err, "mcc");

	// encode mnc - elem8
	err |= cbor_encode_text_stringz(&mapEncoder, "mnc");
	err |= cbor_encode_text_stringz(&mapEncoder, Mobile__GetMncCode());
	DEBUG_ADD(err, "mnc");

	// encode lac - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "lac");
	err |= cbor_encode_text_stringz(&mapEncoder, Mobile__GetLacCode());
	DEBUG_ADD(err, "lac");

	// encode cel - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "cel");
	err |= cbor_encode_text_stringz(&mapEncoder, Mobile__GetCidCode());
	DEBUG_ADD(err, "cel");


	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__); 
        #endif
		len = -1;
	}

	return len;

}


/**
 * @brief CBOR_Connected
 *
 * Prepares CBOR encoded message containing iconnected payload
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Status of connection (1: connected, 0: disconnected)
 * @return void
 */
size_t CBOR_Connected(C_CHAR* cbor_stream, C_UINT16 cbor_status)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	cbor_encoder_init(&encoder, (unsigned char*)cbor_stream, CBORSTREAM_SIZE, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);
	DEBUG_ENC(err, "connected payload");

	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "ver");

	// encode ts - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
	C_TIME t = RTC_Get_UTC_MQTTConnect_Time();
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "t");

	// encode sta - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "sta");
	err |= cbor_encode_uint(&mapEncoder, cbor_status);
	DEBUG_ADD(err, "sta");

	err = cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_stream);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
		len = -1;
	}

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
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
		len = -1;
	}

	return len;
}

/**
 * @brief CBOR_ResSetDevsConfig
 *
 * Prepares CBOR encoded message containing result of set_devs_config
 *
 * @param Pointer to the CBOR-encoded payload
 * @param Pointer to the structure containing received request
 * @param Did, corresponding to combination of address and configuration
 * @return void
 */
size_t CBOR_ResSetDevsConfig(C_CHAR* cbor_response, c_cborhreq* cbor_req, C_UINT16 did)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	CborError err;

	CBOR_ResHeader(cbor_response, cbor_req, &encoder, &mapEncoder);

	// encode dev - elem5
	err = cbor_encode_text_stringz(&mapEncoder, "did");
	err |= cbor_encode_int(&mapEncoder, did);
	DEBUG_ADD(err, "did");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
	    len = -1;
	}

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
	if(device == 0)
		err |= cbor_encode_null(&mapEncoder);
	else
		err |= cbor_encode_int(&mapEncoder, device);
	DEBUG_ADD(err, "dev");

	// encode answer - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "ans");
	err |= cbor_encode_byte_string(&mapEncoder, answer, answer_len);
	DEBUG_ADD(err, "ans");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
	    len = -1; 	
	}

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
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
		len = -1;
	}

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

	// encode val - elem5 - only read
	if (cbor_req->cmd == READ_VALUES) {
		err = cbor_encode_text_stringz(&mapEncoder, "val");
		err |= cbor_encode_text_stringz(&mapEncoder, (char*)val);
		DEBUG_ADD(err, "val");
	}

	// encode dev - elem6
	err = cbor_encode_text_stringz(&mapEncoder, "dev");
	err |= cbor_encode_int(&mapEncoder, Modbus__GetAddress());

	// encode t - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "t");
	C_TIME t = RTC_Get_UTC_Current_Time();
	err |= cbor_encode_uint(&mapEncoder, t);
	DEBUG_ADD(err, "t");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, (unsigned char*)cbor_response);
	else
	{
        #ifdef __DEBUG_CBOR_CAREL_LEV_1
		printf("%s: invalid CBOR stream\n",  __func__);
        #endif
		len = -1;
	}

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

	int64_t tmp = 0;

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "ver", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_req->ver = (C_INT16)tmp;
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
			cbor_req->cmd = (C_INT16)tmp;
			DEBUG_DEC(err, "header: cmd");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "header: discard element");
		}
		if (err)
			return err;
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
CborError CBOR_ReqSetLinesConfig(C_CHAR* cbor_stream, C_UINT16 cbor_len, c_cborreqlinesconfig *set_line_cfg)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "set lines config request map");

	int64_t tmp = 0;

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "bau", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			set_line_cfg->baud = (C_UINT32)tmp;
			DEBUG_DEC(err, "req_set_lines_config: bau");
		}
		else if (strncmp(tag, "con", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			set_line_cfg->conn = (C_BYTE)tmp;
			DEBUG_DEC(err, "req_set_lines_config: con");
		}
		else if (strncmp(tag, "del", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			set_line_cfg->del = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_lines_config: del");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_set_lines_config: discard element");
		}
		if (err)
			return err;
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

	int64_t tmp = 0;

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
			err |= CBOR_ExtractInt(&recursed, &tmp);
			download_devs_config->cid = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: cid");
		}
		else if (strncmp(tag, "crc", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			download_devs_config->crc = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: crc");
		}
		else if (strncmp(tag, "dev", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			download_devs_config->dev = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: dev");
		}
		else if (strncmp(tag, "did", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			download_devs_config->did = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: did");
		}
		else if (strncmp(tag, "fil", 3) == 0)
		{
			stlen = FIL_SIZE;
			err |= cbor_value_copy_text_string(&recursed, download_devs_config->fil, &stlen, &recursed);
			DEBUG_DEC(err, "req_set_devs_config: fil");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_set_devs_config: discard element");
		}
		if (err)
			return err;
	}

	err = cbor_value_leave_container(&it, &recursed);
	return err;
}


/**
 * @brief CBOR_ReqScanLine
 *
 * Interprets CBOR scan line request
 *
 * @param Pointer to the CBOR stream
 * @param Length of the CBOR stream
 * @param Pointer to the address to be queried (if 0, query all addresses)
 * @return CborError
 */
CborError CBOR_ReqScanLine(C_CHAR* cbor_stream, C_UINT16 cbor_len, C_UINT16 *device)
{
	CborError err = CborNoError;
	size_t stlen;
	char tag[TAG_SIZE];
	CborValue it, recursed;
	CborParser parser;

	err = cbor_parser_init((unsigned char*)cbor_stream, cbor_len, 0, &parser, &it);
	err |= cbor_value_enter_container(&it, &recursed);
	DEBUG_DEC(err, "scan line request map");

	int64_t tmp;

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "dev", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			*device = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_scan_line: dev");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_scan_line: discard element");
		}
		if (err)
			return err;
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

	int64_t tmp = 0;

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
			cbor_rwv->func = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_rdwr_values: fun");
		}
		else if (strncmp(tag, "adr", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->addr = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_rdwr_values: adr");
		}
		else if (strncmp(tag, "dim", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->dim = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_rdwr_values: dim");
		}
		else if (strncmp(tag, "pos", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->pos = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_rdwr_values: pos");
		}
		else if (strncmp(tag, "len", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->len = (C_UINT16)tmp;
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
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_rwv->flags = ftmp;
			DEBUG_DEC(err, "req_rdwr_values: flg");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_rdwr_values: discard element");
		}
		if (err)
			return err;
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

	int64_t tmp = 0;

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "pva", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->pva = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_gw_config: pva");
		}
		else if (strncmp(tag, "pst", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->pst = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_gw_config: pst");
		}
		else if (strncmp(tag, "mka", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->mka = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_gw_config: mka");
		}
		else if (strncmp(tag, "lss", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->lss = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_gw_config: lss");
		}
		else if (strncmp(tag, "hss", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			cbor_setgwconfig->hss = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_gw_config: hss");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_set_gw_config: discard element");
		}
		if (err)
			return err;
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

	int64_t tmp=0;

	while (!cbor_value_at_end(&recursed)) {
		stlen = TAG_SIZE;
		memset(tag,'0',sizeof(tag));
		err = cbor_value_copy_text_string(&recursed, tag, &stlen, &recursed);

		if (strncmp(tag, "cid", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			*cid = (C_UINT16)tmp;
			DEBUG_DEC(err, "reboot: cid");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "reboot: discard element");
		}
		if (err)
			return err;
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
			break;
		}
		case CborByteStringType: {
			size_t n=512;
			uint8_t stringa[512];
			err = cbor_value_copy_byte_string(it, stringa, &n, it);
			break;
		}
		case CborTextStringType: {
			size_t n=512;
			char text[512];
			err = cbor_value_copy_text_string(it, text, &n, it);
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

	int64_t tmp=0;

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
			update_dev_fw->fid = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: fid");
		}
		else if (strncmp(tag, "wet", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			update_dev_fw->wet = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: wet");
		}
		else if (strncmp(tag, "cid", 3) == 0)
		{
			err |= CBOR_ExtractInt(&recursed, &tmp);
			update_dev_fw->cid = (C_UINT16)tmp;
			DEBUG_DEC(err, "req_set_devs_config: cid");
		}
		else
		{
			err |= CBOR_DiscardElement(&recursed);
			DEBUG_DEC(err, "req_: discard element");
		}
		if (err)
			return err;
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
	C_CHAR cbor_response[RESPONSE_SIZE];
	size_t len = 0;
	int ret = 0;

	C_MQTT_TOPIC topic;
	C_GATEWAY_ID dev_id;

	Get_Gateway_ID((C_SBYTE *)(&dev_id));

	strcpy((char*)topic,(char*)dev_id);

	err = CBOR_ReqHeader(cbor_stream, cbor_len, &cbor_req);
	if (err)
		cbor_req.cmd = NO_COMMAND;		// request is not recognized

	switch(cbor_req.cmd){

		case REBOOT:
		{
			C_UINT16 cbor_cid;
			err = CBOR_ReqReboot(cbor_stream, cbor_len, &cbor_cid);
            #ifdef __DEBUG_CBOR_CAREL_LEV_1
			printf("reboot > cid %d\n", cbor_cid);
            #endif


			// mqtt response
			cbor_req.res = (err == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);

			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato

			if(SUCCESS_CMD == cbor_req.res){
				PollEngine_StopEngine_CAREL();
				// save cid for successive hello
				NVM__WriteU32Value(MB_CID_NVM, cbor_cid);
				GME__Reboot();
			}
		}
		break;

		case FLUSH_VALUES:
		{
			//flush values
            #ifdef __DEBUG_CBOR_CAREL_LEV_1
			printf("flush_values\n");
            #endif
			ForceSending();		// TODO move this call after publish? ...that way response to flush is surely sent before forced values
			cbor_req.res = SUCCESS_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;


		case SET_GW_CONFIG:
		{
			c_cborreqsetgwconfig cbor_setgwconfig = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqSetGwConfig(cbor_stream, cbor_len, &cbor_setgwconfig);
			if (err == C_SUCCESS) {
				// write new data to configuration file and put in res the result of operation
				// to be implemented
				cbor_req.res = (execute_set_gw_config(cbor_setgwconfig) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			}
			// mqtt response
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;

		case SET_DEVS_CONFIG:
		{
			c_cborreqdwldevsconfig download_devs_config = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqSetDevsConfig(cbor_stream, cbor_len, &download_devs_config);
			if (err == C_SUCCESS) {
				CBOR_SaveAsyncRequest(cbor_req, download_devs_config.cid);

				// empty uri means device has to be disactivated
				// no model must be downloaded
				// configuration flag in nvm must be cleared
				// save cid for successive reboot
				if (!strcmp(download_devs_config.uri,"")) {
					unlink(MODEL_FILE);
					if((C_SUCCESS == NVM__WriteU8Value(SET_DEVS_CONFIG_NVM, DEFAULT)) &&
						(C_SUCCESS == NVM__WriteU32Value(MB_CID_NVM, download_devs_config.cid)) &&
						(C_SUCCESS == NVM__WriteU32Value(MB_DID_NVM, 0)) )
						err = C_SUCCESS;
					else
						err = C_FAIL;
					err == C_SUCCESS ? CBOR_SendAsyncResponseDid(0, download_devs_config.did) : CBOR_SendAsyncResponseDid(1, download_devs_config.did);
					if(err == C_SUCCESS)
						GME__Reboot();

					break;
				}
				OTA__ModelInit(download_devs_config);
			}
			// response will be sent when ota task will come to its end
		}
		break;

		case SET_LINES_CONFIG:
		{
			c_cborreqlinesconfig set_line_cfg = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqSetLinesConfig(cbor_stream, cbor_len, &set_line_cfg);
			if (err == C_SUCCESS) {
				// write new baud rate and connector to configuration file and put in res the result of operation
				cbor_req.res = (execute_set_line_config(set_line_cfg) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			}
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;

		case SCAN_DEVICES:
		{
			// scan Modbus line
			C_UINT16 device = 0;
			C_BYTE answer[REPORT_SLAVE_ID_SIZE];
			C_INT16 length = 0;
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqScanLine(cbor_stream, cbor_len, &device);
			if (err == C_SUCCESS) {
				cbor_req.res = (execute_scan_devices((C_BYTE *)(&answer), &device, &length) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			}
			if (cbor_req.res == ERROR_CMD)
				device = 0;

			len = CBOR_ResScanLine(cbor_response, &cbor_req, device, answer, length);
            sprintf(topic,"%s%s", "/res/", cbor_req.rto);
            mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
            //TODO CPPCHECK valore di ritorno non testato
		}
		break;

		case READ_VALUES:
		case WRITE_VALUES:
		{
			c_cborreqrdwrvalues cbor_rwv = {0};
			cbor_req.res = ERROR_CMD;
			// following check to be sure we are not asking something on non-configured serial
			if (GetsmStatus() == GME_IDLE_INTERNET_CONNECTED) {
				err = CBOR_ReqRdWrValues(cbor_stream, cbor_len, &cbor_rwv);
				if (err == C_SUCCESS) {
					// send modbus command to read/write values
					// wait modbus response to get result
					if (cbor_req.cmd == READ_VALUES)
						cbor_req.res = (parse_read_values(&cbor_rwv) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
					else
						cbor_req.res = (parse_write_values(cbor_rwv) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
				}
			}
			if (cbor_req.res == SUCCESS_CMD)	{
					#ifdef __DEBUG_CBOR_CAREL_LEV_2
					printf("OPERATION_SUCCEEDED, PollEngine__Read_COIL_Req x=%s\n", cbor_rwv.val);
					#endif

					len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_rwv.alias, cbor_rwv.val);
			} else {
					#ifdef __DEBUG_CBOR_CAREL_LEV_1
					printf("CBOR R/W VALUES OPERATION_FAILED\n");
					#endif
					len = CBOR_ResRdWrValues(cbor_response, &cbor_req, cbor_rwv.alias, "0");
			}

			// send response with result
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato

		}
		break;


		case UPDATE_GME_FIRMWARE:
		{
			c_cborrequpdgmefw update_gw_fw = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqUpdateGMEFW(cbor_stream, cbor_len, &update_gw_fw);
			if (err == C_SUCCESS) {
				Modbus_Disable();
				CBOR_SaveAsyncRequest(cbor_req, update_gw_fw.cid);
				OTA__GMEInit(update_gw_fw);
			}
			// response will be sent when ota task will come to its end
		}
		break;


		case UPDATE_DEV_FIRMWARE:
		{
			c_cborrequpddevfw update_dev_fw = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqUpdateDevFW(cbor_stream, cbor_len, &update_dev_fw);
			if (err == C_SUCCESS) {
				CBOR_SaveAsyncRequest(cbor_req, 0);
				OTA__DEVInit(update_dev_fw);
				ret = 1;	// this let polling restart after update
			}
			// response will be sent when update_dev task will come to its end
		}
		break;


		case UPDATE_CA_CERTIFICATES:
		{
			c_cborrequpdatecacert update_ca_config = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqUpdateCaCertificate(cbor_stream, cbor_len, &update_ca_config);
			if (err == C_SUCCESS) {
				//Modbus is disabled to let resource available for other tasks (not for functional reasons)
			//	Modbus_Disable();
				CBOR_SaveAsyncRequest(cbor_req, 0);
				// start a task that performs a https read file from uri, using usr and pwd authentication data
				OTA__CAInit(update_ca_config);
				ret = 1;
			}
			// response will be sent when update_dev task will come to its end
		}
		break;


		case CHANGE_CREDENTIALS:
		{
			c_cborreqchangecred change_cred_config = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqChangeCredentials(cbor_stream, cbor_len, &change_cred_config);
			if (err == C_SUCCESS) {
				// write new credentials to configuration file and put in res the result of operation
				cbor_req.res = (execute_change_cred(change_cred_config) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			}
			// mqtt response
			// to be implemented
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;


		case START_ENGINE:
		{
			if ( C_SUCCESS == NVM__WriteU8Value(PE_STATUS_NVM, RUNNING) ) {
				PollEngine_StartEngine_CAREL();
				cbor_req.res = SUCCESS_CMD;
			}
			else {
				cbor_req.res = ERROR_CMD;
			}
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;


		case STOP_ENGINE:
		{
			if ( C_SUCCESS == NVM__WriteU8Value(PE_STATUS_NVM, STOPPED) ) {
				PollEngine_StopEngine_CAREL();
				cbor_req.res = SUCCESS_CMD;
				ret = 1;
			}
			else {
				cbor_req.res = ERROR_CMD;
			}
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;

		case UPDATE_FILE:
		{
			c_cborrequpdatefile update_file = {0};
			cbor_req.res = ERROR_CMD;

			err = CBOR_ReqUpdateFile(cbor_stream, cbor_len, &update_file);
			if(err == C_SUCCESS) {
				// write new data to configuration file and put in res the result of operation
				// to be implemented
				cbor_req.res = (execute_update_file(&update_file) == C_SUCCESS) ? SUCCESS_CMD : ERROR_CMD;
			}
			// mqtt response
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		}
		break;

		case NO_COMMAND:
		default:
			cbor_req.res = INVALID_CMD;
			len = CBOR_ResSimple(cbor_response, &cbor_req);
			sprintf(topic,"%s%s", "/res/", cbor_req.rto);
			mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
			//TODO CPPCHECK valore di ritorno non testato
		break;

	}

	return ret;
}

void CBOR_SaveAsyncRequest(c_cborhreq cbor_req, C_UINT16 my_cid){
	async_req = cbor_req;
	async_cid = my_cid;
}

void CBOR_SendAsyncResponse(C_INT16 res){
	C_CHAR cbor_response[RESPONSE_SIZE];
	C_MQTT_TOPIC topic;

	async_req.res = res;
	size_t len = CBOR_ResSimple(cbor_response, &async_req);
	sprintf(topic,"%s%s", "/res/", async_req.rto);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);
	if(res == SUCCESS_CMD){
		// save cid for successive hello
		if(async_cid != 0) {
			NVM__WriteU32Value(MB_CID_NVM, async_cid);
			GME__Reboot();
		}
	}
}

void CBOR_SendAsyncResponseDid(C_INT16 res, C_UINT16 did){
	C_CHAR cbor_response[RESPONSE_SIZE];
	C_MQTT_TOPIC topic;

	async_req.res = res;
	size_t len = CBOR_ResSetDevsConfig(cbor_response, &async_req, did);
	sprintf(topic,"%s%s", "/res/", async_req.rto);
	mqtt_client_publish((C_SCHAR*)MQTT_GetUuidTopic(topic), (C_SBYTE*)cbor_response, len, QOS_0, NO_RETAIN);

	if(res == SUCCESS_CMD){
		// save cid for successive hello
		if(async_cid != 0) {
			NVM__WriteU32Value(MB_CID_NVM, async_cid);
			GME__Reboot();
		}
	}
}


C_RES execute_set_line_config(c_cborreqlinesconfig set_line_cfg){

	C_RES err = NVM__WriteU32Value(MB_BAUDRATE_NVM, set_line_cfg.baud);
	err |= NVM__WriteU8Value(MB_CONNECTOR_NVM, set_line_cfg.conn);
	err |= NVM__WriteU8Value(SET_LINE_CONFIG_NVM, CONFIGURED);
	err |= NVM__WriteU32Value(MB_DELAY_NVM, set_line_cfg.del);

	return err;
}

C_RES execute_set_gw_config(c_cborreqsetgwconfig set_gw_config)
{
    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("Execute Set GW Config\n");
    #endif
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

	C_BYTE res = C_SUCCESS;

    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("Execute Change Credentials\n");
    #endif

	// save new credentials, user/password
	res |= NVM__WriteString(MQTT_USER, change_cred.usr);
	res |= NVM__WriteString(MQTT_PASSWORD, change_cred.pwd);

	return res;
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
	C_RES err = C_SUCCESS;

#ifdef INCLUDE_PLATFORM_DEPENDENT
	if (*add == 0) {
		// send request
		do{
		  err = app_report_slave_id_read(++addr);

		  Sys__Delay(100);

		}while(err != C_SUCCESS && addr < MB_ADDRESS_MAX);
	}
	else {
		  err = app_report_slave_id_read(*add);
	}

	Sys__Delay(1000);
#endif

	if(err != 0)
	  return C_FAIL;
//
// save the data
//
#ifdef INCLUDE_PLATFORM_DEPENDENT
	len = usMBSlaveIDLen;
	*(data_rx) = addr;
	// get response
	for(C_INT16 i = 0; i < len + 2; i++)
	  *(data_rx + i + 1) = 	ucMBSlaveID[i];

#endif

	*add = addr;
	*lnt = len + 1;
	*lnt += 2;	//include the 2 byte of the CRC modbus

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
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_A/B c_read: %f \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_B:
		{
			float c_read= 0.0;
			c_read = get_type_b(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_A/B c_read: %f \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_C_SIGNED:
		{
			int32_t c_read = 0;
			c_read = get_type_c_signed(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_CU c_read: %d \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_C_UNSIGNED:
		{
			uint32_t c_read = 0;
			c_read = get_type_c_unsigned(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_CS c_read: %d \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_D:
		{
			uint8_t c_read = 0;
			c_read = get_type_d(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_D c_read: %d \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_E:
		{
			int16_t c_read = 0;
			c_read = get_type_e(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_E c_read: %d \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		case TYPE_F_SIGNED:
		{
			int16_t c_read = 0;
			c_read = get_type_f_signed(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_FS c_read: %d \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
				break;

		case TYPE_F_UNSIGNED:
		{
			uint16_t c_read = 0;
			c_read = get_type_f_unsigned(hr_to_read, CURRENT);
            #ifdef __DEBUG_CBOR_CAREL_LEV_2
			printf("TYPE_FU c_read: %d \n",c_read);
            #endif
			conv_value = (long double)c_read;
		}
			break;

		default:
			break;
		}
	return conv_value;
}


C_INT16 getBitMask(C_INT16 len)
{
	C_INT16 data = 0;

	switch(len)
	{
	   case 0: { data = 0x0000; break; }
	   case 1: { data = 0x0001; break; }
	   case 2: { data = 0x0003; break; }
	   case 3: { data = 0x0007; break; }
	   case 4: { data = 0x000F; break; }
	   case 5: { data = 0x001F; break; }
	   case 6: { data = 0x003F; break; }
	   case 7: { data = 0x007F; break; }
	   case 8: { data = 0x00FF; break; }
	   case 9: { data = 0x01FF; break; }
	   case 10: { data = 0x03FF; break; }
	   case 11: { data = 0x07FF; break; }
	   case 12: { data = 0x0FFF; break; }
	   case 13: { data = 0x1FFF; break; }
	   case 14: { data = 0x3FFF; break; }
	   case 15: { data = 0x7FFF; break; }
	   case 16: { data = 0xFFFF; break; }
	}
	return data;
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
	// TODO da gestire tutte le conversioni di tipo nei vari casi

	C_CHAR num_reg;
	C_FLOAT val_to_write;
	C_INT32 ivalue;

    // only for Nibble management
	hr_ir_low_high_poll_t hr_to_read = {0};

    uint16_t tmp, mask;

	double res;

	res = atof((C_SCHAR*)cbor_wv.val);

	val_to_write = (C_FLOAT)res;

	switch(cbor_wv.func){

		case mbW_COIL:
		case mbW_COILS:{

			result = PollEngine__Write_COIL_Req(val_to_write, cbor_wv.addr, cbor_wv.func);
		}
		break;

		case mbW_HR:
		case mbW_HRS:{

			if(cbor_wv.dim == 16) { num_reg = 1; }
			else                  {	num_reg = 2; }

			if(cbor_wv.flags.bit.fixedpoint == 1 ||
			   cbor_wv.flags.bit.ieee == 1)
			{
				val_to_write = (val_to_write - (long double)(atof((C_SCHAR*)cbor_wv.b)))  /  (long double)(atof((C_SCHAR*)cbor_wv.a));
			        result = PollEngine__Write_HR_Req(val_to_write, cbor_wv.addr, num_reg, cbor_wv.flags.bit.bigendian, cbor_wv.func);
			}
			else
			{  
			   if(cbor_wv.len == 16)
			   {
				// is an Integer number
				ivalue = atoi((C_SCHAR*)cbor_wv.val);				
				ivalue = (ivalue - (C_INT32)(atoi((C_SCHAR*)cbor_wv.b)))  /  (C_INT32)(atoi((C_SCHAR*)cbor_wv.a));				
				result = PollEngine__Write_HR_Req_Int(ivalue, cbor_wv.addr, num_reg, cbor_wv.flags.bit.bigendian, cbor_wv.func);
			   }
			   else if(cbor_wv.len == 32)
			   {
				// for Int32 bit
				ivalue = atol((C_SCHAR*)cbor_wv.val);
				ivalue = (ivalue - (C_INT32)(atol((C_SCHAR*)cbor_wv.b)))  /  (C_INT32)(atol((C_SCHAR*)cbor_wv.a));
				result = PollEngine__Write_HR_Req_Int(ivalue, cbor_wv.addr, num_reg, cbor_wv.flags.bit.bigendian, cbor_wv.func);
			   }
			   else
			   {
			     // Write a nibble
				 hr_to_read.info.Addr = cbor_wv.addr;
				 hr_to_read.info.dim = cbor_wv.dim;
				 hr_to_read.info.bitposition = cbor_wv.pos;
				 hr_to_read.info.len = cbor_wv.len;
				 hr_to_read.info.linA = atof((C_SCHAR*)cbor_wv.a);
				 hr_to_read.info.linB = atof((C_SCHAR*)cbor_wv.b);
				 hr_to_read.info.flag.byte = cbor_wv.flags.byte;

				 // read the actual data
				 result = PollEngine__Read_HR_IR_Req(mbR_HR,  hr_to_read.info.Addr, hr_to_read.info.dim ,(void*)&hr_to_read.c_value.value);

				 mask = getBitMask(cbor_wv.len);
				 tmp = ((uint16_t)hr_to_read.c_value.value & (0xFFFF^(mask << (cbor_wv.pos))));
				 tmp |= (((uint16_t)val_to_write & mask) << (cbor_wv.pos));

				 result = PollEngine__Write_HR_Req(tmp , cbor_wv.addr, num_reg, cbor_wv.flags.bit.bigendian, cbor_wv.func);   
			   }
			}
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

    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("function = %d\n", cbor_rv->func);
    #endif
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
		hr_to_read.info.linA = atof((C_SCHAR*)cbor_rv->a);
		hr_to_read.info.linB = atof((C_SCHAR*)cbor_rv->b);
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

C_RES execute_update_file(c_cborrequpdatefile *update_file){

	https_conn_err_t err = CONN_FAIL;
	uint8_t cert_num = CERT_1;

    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("execute_update_file\n");
    #endif

	if ((memcmp(update_file->fil,LOGIN_HTML, strlen(LOGIN_HTML)))!=0 &&
			(memcmp(update_file->fil,CHANGE_CRED_HTML, strlen(CHANGE_CRED_HTML)))!=0 &&
			(memcmp(update_file->fil,CONFIG_HTML, strlen(CONFIG_HTML)))!=0 &&
			(memcmp(update_file->fil,STYLE_CSS, strlen(STYLE_CSS)))!=0 &&
			(memcmp(update_file->fil,FAV_ICON, strlen(FAV_ICON)))!=0) {
		return C_FAIL;
	}

	// get current certificate number and download file
	if(C_SUCCESS != NVM__ReadU8Value(MB_CERT_NVM, &cert_num))
		cert_num = CERT_1;

	err = HttpsClient__DownloadFile(update_file, cert_num, update_file->fil);
	if(CONN_OK != err)
		return C_FAIL;

    #ifdef __DEBUG_CBOR_CAREL_LEV_2
	printf("execute_update_file err= %d \n",err);
    #endif

	return C_SUCCESS;
}

void CBOR_ReadDidFromNVM (void)
{
	C_UINT32 val;

	if (C_SUCCESS != NVM__ReadU32Value(MB_DID_NVM, &val))
	{
		did = 0;
	}
	else
	{
		did = (C_UINT16)val;
	}
}


C_UINT16 CBOR_GetDid (void)
{
	return did;
}

char* CBOR_GetPartNumber(void)
{
	if (PLATFORM(PLATFORM_DETECTED_WIFI) || PLATFORM(PLATFORM_DETECTED_ESP_WROVER_KIT) || PLATFORM(PLATFORM_DETECTED_BCU))
		return GW_WIFI_PARTNUMBER;
	else if (PLATFORM(PLATFORM_DETECTED_2G))
		return GW_GSM_PARTNUMBER;
	else return "";
}
