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

/* Exported types ------------------------------------------------------------*/ 


/* Exported constants --------------------------------------------------------*/

/**
 * @brief CBOR_Alarms
 *
 * Prepares CBOR encoded message containing information on alarms occurred in the interval from @tstart till @tstop
 *
 * @param alias Modbus alias of the connected device.
 * @param tstart Starting time of the monitored interval.
 * @param tstop Stop time of the monitored interval.
 * @param alarm_issue ???
 * @param Pointer to the CBOR-encoded payload
 * @return void
 */
void CBOR_Alarms(C_UINT16 alias, C_TIME tstart, C_TIME tstop, C_BYTE alarm_issue, C_CHAR* cbor_stream)
{
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
	CborEncoder encoder, mapEncoder, mapEncoder1;
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
	err |= cbor_encode_uint(&mapEncoder, atoi(GW_HW_REV));
	DEBUG_ADD(err, "hw version");

	// encode fwv - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "fwv");
	err |= cbor_encode_uint(&mapEncoder, atoi(GW_FW_REV));
	DEBUG_ADD(err, "fw version");

	// encode cfg - elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "cfg");
	err |= cbor_encode_uint(&mapEncoder, Is_GME_Configured());			//CONFIGURED?
	DEBUG_ADD(err, "configured");

	// encode bau - elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "bau");
	C_UINT32 baud_rate;
	Get_RS485_BaudRate(&baud_rate);
	err |= cbor_encode_uint(&mapEncoder, baud_rate);
	DEBUG_ADD(err, "baud rate");

	// encode mqv - elem7
	err |= cbor_encode_text_stringz(&mapEncoder, "mqv");
	err |= cbor_encode_uint(&mapEncoder, 1);
	DEBUG_ADD(err, "capabilities, mqttversion");

	// encode lgf - elem8
	err |= cbor_encode_text_stringz(&mapEncoder, "lgf");
	err |= cbor_encoder_create_array(&mapEncoder, &mapEncoder1, 2);
	DEBUG_ENC(err, "create capabilities logfrequency array/map");
	C_UINT16 stime;
	Get_HighSpeedSamplingTime(&stime);
	cbor_encode_uint(&mapEncoder1, stime);
	Get_LowSpeedSamplingTime(&stime);
	cbor_encode_uint(&mapEncoder1, stime);
	cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	// encode dev - elem9
	err |= cbor_encode_text_stringz(&mapEncoder, "dev");
	C_UINT16 device_address;
	Get_Device_Address(&device_address);
	err |= cbor_encode_uint(&mapEncoder, device_address);
	DEBUG_ADD(err, "dev");

	// encode gid - elem10
	err |= cbor_encode_text_stringz(&mapEncoder, "gid");
	C_BYTE Guid[16];
	Get_Guid(Guid);
	err |= cbor_encode_byte_string(&mapEncoder, Guid, 16);
	DEBUG_ADD(err, "guid");
	
	//encode crc - elem 11
	err |= cbor_encode_text_stringz(&mapEncoder, "crc");
	C_UINT16 crc = 0;													// to be implemented
	err |= cbor_encode_uint(&mapEncoder, crc);
	DEBUG_ADD(err, "crc");
	
	//encode crc - elem 12
	err |= cbor_encode_text_stringz(&mapEncoder, "cid");
	C_UINT16 cid=0;														// to be implemented
	err |= cbor_encode_uint(&mapEncoder, crc);
	DEBUG_ADD(err, "cid");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
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
	DEBUG_ENC(err, "hello create main map");
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
	C_UINT32 freemem = 0; 												// to be implemented
	err |= cbor_encode_uint(&mapEncoder, freemem);
	DEBUG_ADD(err,"upt");
	
	// encode est -elem5
	err |= cbor_encode_text_stringz(&mapEncoder, "est");
	err |= cbor_encode_uint(&mapEncoder, Get_Polling_Status());
	DEBUG_ADD(err,"est");
	
	// encode sgn -elem6
	err |= cbor_encode_text_stringz(&mapEncoder, "sgn");
	C_INT16 rssi = 0;													// to be implemented
	err |= cbor_encode_uint(&mapEncoder, rssi);
	DEBUG_ADD(err,"sgn");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;	
}

/**
 * @brief CBOR_Values
 * 
 * Prepares CBOR encoded message containing variable values
 *
 * @param Pointer to the CBOR-encoded payload
 * @return void
 */
void CBOR_Values(C_CHAR* cbor_stream)
{
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
	DEBUG_ENC(err, "hello create main map");
	// encode ver - elem1
	err |= cbor_encode_text_stringz(&mapEncoder, "ver");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");
	
	// encode gup - elem2
	err |= cbor_encode_text_stringz(&mapEncoder, "gup");
	C_INT32 gup = 0;											// to be implemented
	err |= cbor_encode_uint(&mapEncoder, gup);
	DEBUG_ADD(err, "gup");
	
	// encode sig - elem3
	err |= cbor_encode_text_stringz(&mapEncoder, "sig");
	C_INT16 sig = 0;											// to be implemented
	err |= cbor_encode_uint(&mapEncoder, sig);
	DEBUG_ADD(err, "sig");
	
	// encode ime - elem4
	err |= cbor_encode_text_stringz(&mapEncoder, "ime");
	C_BYTE imei[15];												// to be implemented
	err |= cbor_encode_byte_string(&mapEncoder, imei, 15);
	DEBUG_ADD(err, "ime");
	
	// other elements to come (if needed)
	
	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;	

}

/**
 * @brief Prepares CBOR encoded message containing response to a request
 * @param Pointer to the CBOR-encoded payload
 * @param Received request header
 * @param Result
 * @return Size of the appended header
 */
void CBOR_Response(C_CHAR* cbor_stream, c_cborhreq* cbor_req, C_INT16 result)
{
	/* prepare header with CBOR_ResHeader */
	/* append payload starting from returned position */
	/* data depends on received request */
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
	C_INT16 sig = 0;											// to be implemented
	err |= cbor_encode_uint(mapEncoder, cbor_req->cmd);
	DEBUG_ADD(err, "cmd");
   
}

/**
 * @brief CBOR_ResSetLinesConfig
 * 
 * Prepares CBOR response to set lines config
 *
 * @param Pointer to the CBOR-encoded header
 * @param Received request header
 * @param Result of the operation
 * @return Size of the appended header
 */
size_t CBOR_ResSetLinesConfig(C_CHAR* cbor_stream, c_cborhreq* cbor_req, C_UINT16 res)
{
	size_t len;
	CborEncoder encoder, mapEncoder;
	int err;
	
	CBOR_ResHeader(cbor_stream, cbor_req, &encoder, &mapEncoder);
	
	// encode res - elem4
	err = cbor_encode_text_stringz(&mapEncoder, "res");
	err |= cbor_encode_uint(&mapEncoder, res);
	DEBUG_ADD(err, "res");

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);

	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("%s: invalid CBOR stream\n",  __func__); len = -1; }

	return len;
}

/**
 * @brief Prepares CBOR encoded message containing TODO
 * @param Pointer to the CBOR-encoded payload
 * @return void
 */
void CBOR_ResWriteValues(C_CHAR* cbor_stream, c_cborreswritevalues* cbor_reswv)
{
}

void CBOR_ResReadValues(C_CHAR* cbor_stream)
{
}

void CBOR_ResScanLine(C_CHAR* cbor_stream)
{
}

void CBOR_ResMbAdu(C_CHAR* cbor_stream)
{
}

void CBOR_ResSetDevsConfig(C_CHAR* cbor_stream)
{
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
CborError CBOR_ReqHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req, CborValue* it, CborValue* recursed)
{
	// parse request header and get replyTo and cmd fields
	// write them to cbor_req
	CborParser parser;
	size_t stlen;
    char tag[TAG_SIZE];
	char rto[REPLYTO_SIZE];
	CborError err;
	C_INT16 val;
	
	err = cbor_parser_init(cbor_stream, HEADERREQ_LEN, 0, &parser, it);
	err |= cbor_value_enter_container(it, recursed);
	DEBUG_DEC(err, "header request map");
	err |= cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_get_int(recursed, &val);
	cbor_req->ver = val;
	err |= cbor_value_advance_fixed(recursed);
	DEBUG_DEC(err, "header: ver");

	err |= cbor_value_copy_text_string(recursed, tag, &stlen, recursed);
	err |= cbor_value_copy_text_string(recursed, rto, &stlen, recursed);
	strcpy((char*)cbor_req->rto, rto);
	DEBUG_DEC(err, "header: rto");

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
CborError CBOR_ReqSetLinesConfig(CborValue* recursed, C_UINT32* new_baud_rate)
{
	CborError err;
	C_UINT64 val;
	size_t st1;
	char text[TAG_SIZE];

	err = cbor_value_copy_text_string(recursed, text, &st1, recursed);
	err |= cbor_value_get_uint64(recursed, &val);
	*new_baud_rate = val;
	DEBUG_DEC(err, "req_set_lines_config: bau");
	
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
	
	err = CBOR_ReqHeader(cbor_stream, &cbor_req, &it, &recursed);
	
	switch(cbor_req.cmd){
		case SET_GW_CONFIG:
		{

		}
		break;

		case DOWNLOAD_DEVS_CONFIG:
		{/*
			req_download_devs_config_t download_devs_config = {0};
			parse_download_devs_config(root, &download_devs_config);
			//execute_download_devs_config(&download_devs_config);
			memmgr_free(download_devs_config.username);
			memmgr_free(download_devs_config.password);
			memmgr_free(download_devs_config.uri);
		*/}
		break;


		case SET_LINES_CONFIG:
		{
			
			C_UINT32 new_baud_rate;
			err = CBOR_ReqSetLinesConfig(&recursed, &new_baud_rate);
			err = cbor_value_advance_fixed(&recursed);
			err = cbor_value_leave_container(&it, &recursed);
			
			// write new baud rate to configuration file and put in res the result of operation
			// to be implemented
			C_INT16 res = 0;

			// mqtt response with success or fail 
			// to be implemented
			CBOR_ResSetLinesConfig(cbor_response, &cbor_req, res);
			
			// reboot (is it needed to stop polling and flush data before rebooting????)
			// to be implemented
			
		}
		break;


		case SEND_MB_ADU:
		{/*
			req_send_mb_adu_t send_mb_adu = {0};
			parse_send_mb_adu(root, &send_mb_adu);
			printf("sequence : %d, adu = %s\n",send_mb_adu.sequence, send_mb_adu.adu);

			if(ACTIVETED == PollEngine__GetPassModeStatus()){
				execute_send_mb_adu(send_mb_adu);

			}
			memmgr_free(send_mb_adu.adu);
		*/}
		break;


		case READ_VALUES:
		{
		//	parse_read_values(root);
		}
		break;


		case WRITE_VALUES:
		{
		//	parse_write_values(root);
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
		/*	req_update_ca_cert_t update_ca_cert = {0};
			parse_update_ca_cert(root, &update_ca_cert);
			memmgr_free(update_ca_cert.username);
			memmgr_free(update_ca_cert.password);
			memmgr_free(update_ca_cert.uri);
		*/}
		break;


		case CHANGE_CREDENTIALS:
		{
		/*	req_change_cred_t change_cred_req = {0};
			parse_change_cred(root, &change_cred_req);
			printf("change_cred_req->username : %s\n",change_cred_req.username);
			printf("change_cred_req->password : %s\n",change_cred_req.password);

			if(ESP_OK == execute_change_cred(change_cred_req)){
				send_simple_res(ReqHeader.replyTo, CHANGE_CREDENTIALS, SUCCESS_CMD, MQTT__GetClient());
			}else{
				send_simple_res(ReqHeader.replyTo, CHANGE_CREDENTIALS, ERROR_CMD, MQTT__GetClient());
			}

			memmgr_free(change_cred_req.username);
			memmgr_free(change_cred_req.password);

			//TODO Restart
		*/}
		break;


		case START_ENGINE:
		{
			Set_Polling_Status(C_START);
		/*
			send_simple_res(ReqHeader.replyTo, START_ENGINE, SUCCESS_CMD, MQTT__GetClient());
		*/}
		break;


		case STOP_ENGINE:
		{
			Set_Polling_Status(C_STOP);
		/*
			send_simple_res(ReqHeader.replyTo, STOP_ENGINE, SUCCESS_CMD, MQTT__GetClient());
			PollEngine__ActivatePassMode();
		*/}
		break;


		default:
		//	send_simple_res(ReqHeader.replyTo, ReqHeader.cmd, INVALID_CMD, MQTT__GetClient());
			break;

	}
	
	return 0;
}

