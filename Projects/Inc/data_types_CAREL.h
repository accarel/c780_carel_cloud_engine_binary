/**
 * @file data_types.h
 * @author carel
 * @date 9 Sep 2019
 * @brief  common definitions & data types
 */

#ifndef __DATA_TYPES_H
#define __DATA_TYPES_H


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
#define C_IPV4     C_BYTE[4]

#define C_USERNAME C_BYTE[34]
#define C_PASSWORD C_BYTE[32]

#define C_URI      C_BYTE[64]

#define C_RES      C_INT16


#ifdef IS_A_WIFI_GATEWAY

/**
 * @brief in this case it contain the MAC address
 */
#define C_GATEWAY_ID C_BYTE[6]
#endif

#ifdef IS_A_GSM_GATEWAY
/**
 * @brief in this case it contain the IMEI 
 */
#define C_GATEWAY_ID C_BYTE[16]
#endif


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