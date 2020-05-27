/**
 * @file data_types_CAREL.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  common definitions & data types
 */

#ifndef __DATA_TYPES_CAREL_H
#define __DATA_TYPES_CAREL_H


/* ==== Include ==== */
#include "data_types_IS.h"
#include <stddef.h>

#define  CAREL_TYPES_VERSION   257	// 0x101

#define USERNAME_SIZE	34
#define PASSWORD_SIZE	34
#define SERVER_SIZE		60
#define MQTT_PORT_SIZE	30
#define PORT_SIZE 		6

#define URI_SIZE 		64
#define RTO_SIZE 		64
#define TOPIC_SIZE		100


/* ======================================================= */
/*                     ! WARNING !                         */
/*              DO NOT modified the below part             */
/* ======================================================= */

/* =============== derived data type ====================== */
#define C_TIME     C_INT32

//ie 192.168.100.1    [0]=192 [1]=168 [2]=100 [3]=1
typedef C_BYTE       C_IPV4[4];     
                     
typedef C_SBYTE       C_USERNAME[USERNAME_SIZE];
typedef C_SBYTE       C_PASSWORD[PASSWORD_SIZE]; 
                     
typedef C_SBYTE       C_URI[URI_SIZE];      
                     
typedef C_INT16      C_RES;     

typedef C_SCHAR       C_MQTT_TOPIC[TOPIC_SIZE];

typedef C_BYTE	     C_GUID[16];

typedef C_BYTE	     C_RTO[48];

extern CRC_TABLE_TYPE CRCTABLE[256];

typedef C_BYTE       C_BOOL;


/** 
@brief this data type is able to store both a WiFi or GSM ID
*/
typedef C_SBYTE C_GATEWAY_ID[18];

// type of cloud interface
enum cloud_type {
	notacloudtype = 0,
	TYPEC_WIFI,
	TYPEC_ETH,
	TYPEC_MOBILE
};

// type of field interface
enum field_type {
	notafieldtype=0,
	TYPEF_ETH=2,
	TYPEF_RS485=5
};

// list of protocols
enum list_protocols{
	notaprotocol = 0,
	LPR_MODBUS,
	LPR_CAREL,
	LPR_BACNET};

/*****************
* Cloud to GME
*****************/
#pragma pack(1)
typedef struct req_set_gw_config_s{
	C_UINT32 valuesPeriod;
	C_UINT32 statusPeriod;
	C_UINT32 mqttKeepAliveInterval;
	C_UINT32 lowspeedsamplevalue;
	C_UINT32 hispeedsamplevalue;
}req_set_gw_config_t;
#pragma pack()

#pragma pack(1)
typedef struct cfg_data{
	C_SBYTE cfg_version[2];
	C_UINT16 enc_key;
	C_CHAR  mqtt_broker[SERVER_SIZE];
	C_CHAR  mqtt_port[MQTT_PORT_SIZE];
	C_SBYTE mqtt_pssw[PASSWORD_SIZE];
	C_SBYTE mqtt_user[USERNAME_SIZE];
	C_CHAR  ntp_server[30];
	C_CHAR  ntp_port[PORT_SIZE];
	C_CHAR  apn_name[64];
	C_CHAR	apn_user[64];
	C_CHAR  apn_password[64];
	C_CHAR  crc[2];
}cfg_data_t, *ptr_cfg_data_t;
#pragma pack()

ptr_cfg_data_t pCfgData;
cfg_data_t    CfgData;


C_BYTE hw_platform_detected; //the value is the detected hw platform

C_BYTE modbusPort; // can be uart2 or uart0 if rs485 or TTL

/* ======================================================= */
/*                 GENERAL PURPOSE DEFINE                  */
/* ======================================================= */
#define C_TRUE        1
#define C_FALSE       0
#define C_ON          1
#define C_OFF         0

#define C_FAIL      -1
#define C_SUCCESS   0


/* ======================================================= */
/* CBOR use tag to map the data type below the             */ 
/* corrispondence                                          */
/* ======================================================= */

/* ==== CBOR tag used type ==== */
#define C_CBOR_TAG_TIME	        1
 
#define C_CBOR_TAG_FLOAT        85
#define C_CBOR_TAG_IPV4_ADDR	260


/* ==== Define types for CBOR ==== */
#define C_CBOR_FLOAT_NaN 0xf97e00






/* ========== MACROS ========== */

/* to check easily the running platform */

#define PLATFORM(a)  (hw_platform_detected == (a))


#endif   /* #ifndef __CAREL_TYPES_H */
