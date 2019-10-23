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

#define INCLUDE_PLATFORM_DEPENDENT 1

#define  CAREL_TYPES_VERSION   257	// 0x101

#define USERNAME_SIZE	34
#define PASSWORD_SIZE	34
#define URI_SIZE 		64
#define RTO_SIZE 		48
#define TOPIC_SIZE		100

/* ========================================================================== */
/* Cloud related                                                              */
/* ========================================================================== */
#define MQTT_DEFAULT_BROKER "mqtts://mqtt-dev.tera.systems"
#define MQTT_DEFAULT_PORT   (8883)
#define MQTT_DEFAULT_USER   "admin"
#define MQTT_DEFAULT_PWD    "5Qz*(3_>K&vU!PS^"
#define MQTT_KEEP_ALIVE_DEFAULT_SEC   (600)

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

typedef C_SCHAR       C_MQTT_TOPIC[URI_SIZE];

typedef C_BYTE	     C_GUID[16];

typedef C_BYTE	     C_RTO[48];

extern CRC_TABLE_TYPE CRCTABLE[256];


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

#ifdef IS_A_WIFI_GATEWAY
#define MACORIMEISIZE 	12
#endif
#ifdef  IS_A_GSM_GATEWAY
#define MACORIMEISIZE 	15
#endif

// temporarily put here to compile
#define GW_TYPE "GME"
#define GW_PARTNUMBER "GTW000M2G0"
#define GW_HW_REV  256	// 0x100
#define GW_FW_REV  256	// 0x100

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
#define C_CBOR_TAG_TIME	    1
 
#define C_CBOR_TAG_FLOAT      85
#define C_CBOR_TAG_IPV4_ADDR	260


/* ==== Define types for CBOR ==== */
#define C_CBOR_FLOAT_NaN 0xf97e00



typedef enum _conf{
	DEFAULT = 0,
	TO_RECONFIGURE = 1,
	CONFIGURED = 2,
}configuration_t;



#endif   /* #ifndef __CAREL_TYPES_H */