/*
  File  : data_types_IS.h
  
  Scope : common definitions & data types         
  
  Note  : 
  
  - the software library use CBOR see RFC7049 due to that 
    some of information below refer to it
  
  
*/

#ifndef __DATA_TYPES_IS_H
#define __DATA_TYPES_IS_H

/* ==== Include ==== */


/* ======================================================= */
/*        BEGIN OF CUSTOMIZABLE SECTION                    */
/* this part could be modified to match with your target   */
/* ======================================================= */

/*                    WARNING!                             */
/*    only one of this two define must be active           */
#define IS_A_WIFI_GATEWAY
//#define IS_A_GSM_GATEWAY


/* you MUST change the data type to match the type your    */ 
/* compiler name                                           */
#define C_CHAR	 uint8
#define C_BYTE	 uint8

#define C_INT16  int16
#define C_UINT16 uint16

#define C_INT32  int32

//floating point IEEE 754 little endian 4 bytes
#define C_FLOAT  float32        

/* ======================================================= */
/*           END OF CUSTOMIZABLE SECTION                   */
/* ======================================================= */


#endif   /* #ifndef __CAREL_TYPES_H */