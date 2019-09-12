/**
 * @file data_types_IS.h
 * @author carel
 * @date 9 Sep 2019
 * @brief   common definitions & data types
 *      
 */

#ifndef __DATA_TYPES_IS_H
#define __DATA_TYPES_IS_H

/* ==== Include ==== */


/* ======================================================= */
/*        BEGIN OF CUSTOMIZABLE SECTION                    */
/* this part could be modified to match with your target   */
/* ======================================================= */

/*                    WARNING!                             */
/**
 * @brief the CCL library use this define to adapt itself 
 *        to different types of communication interface
 * @warning only one of this two define must be active
 */
#define IS_A_WIFI_GATEWAY
//#define IS_A_GSM_GATEWAY



/* you MUST change the data type to match the type your    */ 
/* compiler data types                                     */
#define C_CHAR	 uint8
#define C_BYTE	 uint8

#define C_INT16  int16
#define C_UINT16 uint16

#define C_INT32  int32

/* floating point IEEE 754 little endian 4 bytes */
#define C_FLOAT  float32        

/* ======================================================= */
/*           END OF CUSTOMIZABLE SECTION                   */
/* ======================================================= */

/* just a check */
#ifdef IS_A_GSM_GATEWAY 
#ifdef IS_A_WIFI_GATEWAY
#error Only one define is allowed IS_A_GSM_GATEWAY or IS_A_WIFI_GATEWAY
#endif
#endif

#endif   /* #ifndef __CAREL_TYPES_H */