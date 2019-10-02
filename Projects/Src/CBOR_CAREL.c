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

const char cannot_encode[]="cannot encode";
const char cannot_add[]="cannot add";
   
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
 * @return void
 */
void CBOR_Status(C_CHAR* cbor_stream)
{
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
 * Prepares CBOR encoded message containing TODO
 *
 * @param Pointer to the CBOR-encoded payload
 * @return void
 */
void CBOR_Mobile(C_CHAR* cbor_stream)
{
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
 * @param Received request header
 * @param Result
 * @return Size of the appended header
 */
C_UINT16 CBOR_ResHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req, C_INT16 result)
{
return 1;
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

void CBOR_ResSetLinesConfig(C_CHAR* cbor_stream)
{
}





/**
 * @brief CBOR_ReqHeader
 * 
 * Interprets CBOR request header
 *
 * @param Pointer to the CBOR-encoded header
 * @param Received request header
  * @return Size of the appended header
 */
C_UINT16 CBOR_ReqHeader(C_CHAR* cbor_stream, c_cborhreq* cbor_req)
{
	// parse request header and get replyTo and cmd fields
	// writes them to cbor_req
	return 1;
}


int CBOR_ReqTopicParser(C_CHAR* cbor_stream){
	#if 0
	
    CBOR_ReqHeader(&cbor_stream, &cbor_req);
	
	switch(cbor_req.cmd){
		case SET_GW_CONFIG:
		{

		}
		break;

		case DOWNLOAD_DEVS_CONFIG:
		{
			req_download_devs_config_t download_devs_config = {0};
			parse_download_devs_config(root, &download_devs_config);
			//execute_download_devs_config(&download_devs_config);
			memmgr_free(download_devs_config.username);
			memmgr_free(download_devs_config.password);
			memmgr_free(download_devs_config.uri);
		}
		break;


		case SET_LINES_CONFIG:
		{
			req_set_lines_config_t set_lines_config = {0};
			parse_set_line_config(root, &set_lines_config);
			printf("set_lines_config.baud : %d\n",set_lines_config.baud);

			if(ESP_OK == execute_set_line_config(set_lines_config)){
				send_set_lines_config_res(ReqHeader.replyTo, CHANGE_CREDENTIALS, OPERATION_SUCCEEDED, MQTT__GetClient());
			}else{
				send_set_lines_config_res(ReqHeader.replyTo, CHANGE_CREDENTIALS, OPERATION_FAILED, MQTT__GetClient());
			}
			//TODO Restart
		}
		break;


		case SEND_MB_ADU:
		{
			req_send_mb_adu_t send_mb_adu = {0};
			parse_send_mb_adu(root, &send_mb_adu);
			printf("sequence : %d, adu = %s\n",send_mb_adu.sequence, send_mb_adu.adu);

			if(ACTIVETED == PollEngine__GetPassModeStatus()){
				execute_send_mb_adu(send_mb_adu);

			}
			memmgr_free(send_mb_adu.adu);
		}
		break;


		case READ_VALUES:
		{
			parse_read_values(root);
		}
		break;


		case WRITE_VALUES:
		{
			parse_write_values(root);
		}
		break;


		case UPDATE_GME_FIRMWARE:
		{
			req_update_gw_fw_t update_gw_fw = {0};
			parse_update_gw_fw(root, &update_gw_fw);
			memmgr_free(update_gw_fw.username);
			memmgr_free(update_gw_fw.password);
			memmgr_free(update_gw_fw.uri);
		}
		break;


		case UPDATE_DEV_FIRMWARE:
		{
			req_update_dev_fw_t update_dev_fw = {0};
			parse_update_dev_fw(root, &update_dev_fw);
			memmgr_free(update_dev_fw.username);
			memmgr_free(update_dev_fw.password);
			memmgr_free(update_dev_fw.uri);
		}
		break;


		case UPDATE_CA_CERTIFICATES:
		{
			req_update_ca_cert_t update_ca_cert = {0};
			parse_update_ca_cert(root, &update_ca_cert);
			memmgr_free(update_ca_cert.username);
			memmgr_free(update_ca_cert.password);
			memmgr_free(update_ca_cert.uri);
		}
		break;


		case CHANGE_CREDENTIALS:
		{
			req_change_cred_t change_cred_req = {0};
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
		}
		break;


		case START_ENGINE:
		{
			PollEngine__StartEngine();
			send_simple_res(ReqHeader.replyTo, START_ENGINE, SUCCESS_CMD, MQTT__GetClient());
		}
		break;


		case STOP_ENGINE:
		{
			PollEngine__StopEngine();
			send_simple_res(ReqHeader.replyTo, STOP_ENGINE, SUCCESS_CMD, MQTT__GetClient());
			PollEngine__ActivatePassMode();
		}
		break;


		default:
			send_simple_res(ReqHeader.replyTo, ReqHeader.cmd, INVALID_CMD, MQTT__GetClient());
			break;

	}
	#endif
	return 0;
}

