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
	CborEncoder encoder, mapEncoder, mapEncoder1, mapEncoder2, mapEncoder3;
	size_t len;
	int err;

	cbor_encoder_init(&encoder, cbor_stream, 1000, 0);
	// map1
	err = cbor_encoder_create_map(&encoder, &mapEncoder, 9);
	DEBUG_ENC(err, "hello create main map");
	// encode VER - elem1, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "VER");
	DEBUG_ENC(err, "VER");
	err |= cbor_encode_uint(&mapEncoder, CAREL_TYPES_VERSION);
	DEBUG_ADD(err, "version");

	// encode MOD - elem2, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "MDL");
	DEBUG_ENC(err, "MDL");
	err |= cbor_encode_text_stringz(&mapEncoder, GW_TYPE);
	DEBUG_ADD(err, "model");
	// encode PN_ - elem3, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "PN_");
	DEBUG_ENC(err, "PN_");
	err |= cbor_encode_text_stringz(&mapEncoder, GW_PARTNUMBER);
	DEBUG_ADD(err, "part number");
	// encode HWV - elem4, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "HWV");
	DEBUG_ENC(err, "HWV");
	err |= cbor_encode_uint(&mapEncoder, atoi(GW_HW_REV));
	DEBUG_ADD(err, "hw version");
	// encode FWV - elem5, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "FWV");
	DEBUG_ENC(err, "FWV");
	err |= cbor_encode_uint(&mapEncoder, atoi(GW_FW_REV));
	DEBUG_ADD(err, "fw version");
	// encode CIF - elem6, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "CIF");
	DEBUG_ENC(err, "CIF");
	// CIF array
	err |= cbor_encoder_create_array(&mapEncoder, &mapEncoder1, 1);
	// CIF map
	err |= cbor_encoder_create_map(&mapEncoder1, &mapEncoder2, 6);
	DEBUG_ENC(err, "create cloudinterfaces array/map");

	// CIF map - elem1
	err |= cbor_encode_text_stringz(&mapEncoder2, "TYP");
	DEBUG_ENC(err, "TYP");
#ifdef IS_A_WIFI_GATEWAY
	err |= cbor_encode_uint(&mapEncoder2, 1);
#elif IS_A_GSM_GATEWAY
	err |= cbor_encode_uint(&mapEncoder2, 3);
#endif
	DEBUG_ADD(err, "cloudinterfaces, type");
	// CIF map - elem2
	err |= cbor_encode_text_stringz(&mapEncoder2, "CFG");
	DEBUG_ENC(err, "CFG");
	err |= cbor_encode_uint(&mapEncoder2, 1);			//CONFIGURED?
	DEBUG_ADD(err, "cloudinterfaces, configured");
	// CIF map - elem3
	err |= cbor_encode_text_stringz(&mapEncoder2, "ID_");
	DEBUG_ENC(err, "ID_");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "cloudinterfaces, id");
	// CIF map - elem4
	err |= cbor_encode_text_stringz(&mapEncoder2, "NAM");
	DEBUG_ENC(err, "NAM");
	err |= cbor_encode_text_stringz(&mapEncoder2, "WIFI-1");
	DEBUG_ADD(err, "cloudinterfaces, name");
	// CIF map - elem5
	err |= cbor_encode_text_stringz(&mapEncoder2, "MAC");
	DEBUG_ENC(err, "MAC");
	err |= cbor_encode_text_stringz(&mapEncoder2, "D481D7B55A77");
	DEBUG_ADD(err, "cloudinterfaces, mac address");
	// CIF map - elem6
	err |= cbor_encode_text_stringz(&mapEncoder2, "OPM");
	DEBUG_ENC(err, "OPM");
	err |= cbor_encode_uint(&mapEncoder2, 2);
	DEBUG_ADD(err, "cloudinterfaces, operating mode");

	cbor_encoder_close_container(&mapEncoder1, &mapEncoder2);
	cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	// encode FIF - elem7, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "FIF");
	DEBUG_ENC(err, "FIF");
	// FIF array
	err |= cbor_encoder_create_array(&mapEncoder, &mapEncoder1, 1);
	// FIF map
	err |= cbor_encoder_create_map(&mapEncoder1, &mapEncoder2, 5);
	DEBUG_ENC(err, "create fieldinterfaces array/map");

	// FIF map, elem1
	err |= cbor_encode_text_stringz(&mapEncoder2, "TYP");
	DEBUG_ENC(err, "TYP");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "fieldifaces, type");
	// FIF map, elem2
	err |= cbor_encode_text_stringz(&mapEncoder2, "BAU");
	DEBUG_ENC(err, "BAU");
	err |= cbor_encode_uint(&mapEncoder2, 9600);
	DEBUG_ADD(err, "fieldifaces, baud");
	// FIF map, elem3
	err |= cbor_encode_text_stringz(&mapEncoder2, "PRO");
	DEBUG_ENC(err, "PRO");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "fieldifaces, protocol");
	// FIF map, elem4
	err |= cbor_encode_text_stringz(&mapEncoder2, "PID");
	DEBUG_ENC(err, "PID");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "fieldifaces, portid");
	// FIF map, elem5
	err |= cbor_encode_text_stringz(&mapEncoder2, "NAM");
	DEBUG_ENC(err, "NAM");
	err |= cbor_encode_text_stringz(&mapEncoder2, "RS485-1");
	DEBUG_ADD(err, "fieldifaces, name");
	cbor_encoder_close_container(&mapEncoder1, &mapEncoder2);
	cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	// encode CAP - elem8, map1
	cbor_encode_text_stringz(&mapEncoder, "CAP");
	DEBUG_ENC(err, "CAP");
	// CAP array
	err |= cbor_encoder_create_array(&mapEncoder, &mapEncoder1, 1);
	// CAP map
	err |= cbor_encoder_create_map(&mapEncoder1, &mapEncoder2, 6);
	DEBUG_ENC(err, "create capabilities array/map");

	// CAP map, elem1
	err |= cbor_encode_text_stringz(&mapEncoder2, "VCD");
	DEBUG_ENC(err, "VCD");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "capabilities, variablecode");
	// CAP map, elem2
	err |= cbor_encode_text_stringz(&mapEncoder2, "MQV");
	DEBUG_ENC(err, "MQV");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "capabilities, mqttversion");
	// CAP map, elem3
	err |= cbor_encode_text_stringz(&mapEncoder2, "LGV");
	DEBUG_ENC(err, "LGV");
	err |= cbor_encode_int(&mapEncoder2, -1);
	DEBUG_ADD(err, "capabilities, logvalues");
	// CAP map, elem4
	err |= cbor_encode_text_stringz(&mapEncoder2, "LGA");
	DEBUG_ENC(err, "LGA");
	err |= cbor_encode_int(&mapEncoder2, -1);
	DEBUG_ADD(err, "capabilities, logalarms");
	// CAP map, elem5
	err |= cbor_encode_text_stringz(&mapEncoder2, "LGF");
	DEBUG_ENC(err, "capabilities, LGF");
	err |= cbor_encoder_create_array(&mapEncoder2, &mapEncoder3, 2);
	DEBUG_ENC(err, "create capabilities logfrequency array/map");
	cbor_encode_uint(&mapEncoder3, 60);
	cbor_encode_uint(&mapEncoder3, 300);
	cbor_encoder_close_container(&mapEncoder2, &mapEncoder3);
	// CAP map, elem6
	cbor_encode_text_stringz(&mapEncoder2, "LPR");
	DEBUG_ENC(err, "LPR");
	err |= cbor_encoder_create_array(&mapEncoder2, &mapEncoder3, 2);
	DEBUG_ENC(err, "create capabilities protocols array/map");
	cbor_encode_text_stringz(&mapEncoder3, "modbus");
	cbor_encode_text_stringz(&mapEncoder3, "bacnet");
	cbor_encoder_close_container(&mapEncoder2, &mapEncoder3);

	cbor_encoder_close_container(&mapEncoder1, &mapEncoder2);
	cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	//encode GUI - elem9, map1
	err |= cbor_encode_text_stringz(&mapEncoder, "GUI");
	DEBUG_ENC(err, "GUI");
	err |= cbor_encoder_create_array(&mapEncoder, &mapEncoder1, 1);
	err |= cbor_encoder_create_map(&mapEncoder1, &mapEncoder2, 3);
	DEBUG_ENC(err, "create modelguid array/map");

	err |= cbor_encode_text_stringz(&mapEncoder2, "LIN");
	DEBUG_ENC(err, "LIN");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "modelGuids,line");
	err |= cbor_encode_text_stringz(&mapEncoder2, "DEV");
	DEBUG_ENC(err, "DEV");
	err |= cbor_encode_uint(&mapEncoder2, 1);
	DEBUG_ADD(err, "modelGuids,dev");
	err |= cbor_encode_text_stringz(&mapEncoder2, "GUI");
	DEBUG_ENC(err, "GUI");
	err |= cbor_encode_text_stringz(&mapEncoder2, "xxxxxxxxx");
	DEBUG_ADD(err, "modelGuids,guid");

	cbor_encoder_close_container(&mapEncoder1, &mapEncoder2);
	cbor_encoder_close_container(&mapEncoder, &mapEncoder1);

	err |= cbor_encoder_close_container(&encoder, &mapEncoder);
	if(err == CborNoError)
		len = cbor_encoder_get_buffer_size(&encoder, cbor_stream);
	else { printf("invalid stream\n"); len = -1; }

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

