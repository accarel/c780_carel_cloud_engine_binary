/*
  File  : carel_types.h
  
  Scope : common definitions & data types         
  
  Note  : 
  
  - the software library use CBOR see RFC7049 due to that 
    some of information below refer to it
  
  
*/

#ifndef __CAREL_TYPES_H
#define __CAREL_TYPES_H

/* ==== Include ==== */


/* ======================================================= */
/*        BEGIN OF CUSTOMIZABLE PORTABLE SECTION           */
/* this part could be modified to match with your target   */
/* ======================================================= */

/* you MUST change the data type to match the type your    */ 
/* compiler name                                           */
#define C_CHAR	 uint8
#define C_BYTE	 uint8

#define C_INT16  int16
#define C_UINT16 uint16

#define C_INT32  int32

//expressed as IEEE 754 little endian
#define C_FLOAT  float32        

/* ======================================================= */
/*           END OF CUSTOMIZABLE PORTABLE SECTION          */
/* ======================================================= */

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



/* ==== Define ==== */
#define TRUE        1
#define FALSE       0
#define ON          1
#define OFF         0


/* ======================================================= */
/* CBOR use tag to map the data type below the             */ 
/* corrispondence                                          */
/* ======================================================= */

/* ==== CBOR tag used type ==== */
#define CBOR_TAG_TIME	    1

#define CBOR_TAG_FLOAT      85
#define CBOR_TAG_IPV4_ADDR	260


/* ==== Define types for CBOR ==== */
#define CBOR_FLOAT_NaN 0xf97e00




#endif   /* #ifndef __CAREL_TYPES_H */