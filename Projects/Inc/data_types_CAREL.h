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


#define  CAREL_TYPES_VERSION   100


/* ======================================================= */
/*                     ! WARNING !                         */
/*              DO NOT modified the below part             */
/* ======================================================= */

/* =============== derived data type ====================== */
#define C_TIME     C_INT32

//ie 192.168.100.1    [0]=192 [1]=168 [2]=100 [3]=1
typedef C_BYTE       C_IPV4[4];     
                     
typedef C_BYTE       C_USERNAME[34];
typedef C_BYTE       C_PASSWORD[32]; 
                     
typedef C_BYTE       C_URI[64];      
                     
typedef C_INT16      C_RES;     

typedef C_CHAR       C_MQTT_TOPIC[64];

extern CRC_TABLE_TYPE CRCTABLE[256];


/** 
@brief this data type is able to store both a WiFi or GSM ID
*/
typedef C_BYTE C_GATEWAY_ID[18];



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






#endif   /* #ifndef __CAREL_TYPES_H */
