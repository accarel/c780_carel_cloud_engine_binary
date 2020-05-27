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

/* you MUST change the data type to match the type your    */ 
/* compiler data types                                     */
typedef unsigned char   C_BYTE;     // 8 bit value
typedef char  			C_SBYTE;     // 8 bit value
typedef char            C_CHAR;     // 8 bit value
typedef char   			C_SCHAR;     // 8 bit value
typedef	short	        C_INT16;	// 16-bit value
typedef	unsigned short	C_UINT16;	// 16-bit value
typedef	int		        C_INT32;	// 32-bit value
typedef	unsigned int	C_UINT32;	// 32-bit value
//typedef	unsigned long	C_WORD;	    // 32-bit value
typedef	unsigned long long	C_UINT64;   // 64-bit value

/* floating point IEEE 754 little endian 4 bytes */
typedef float           C_FLOAT;    //32-bit value

/* 
   don't change the contents of the following tables
   the tables are here only to give the possibility to set some
   memory modification compiler directives and store it in 
   the best place, alternatively is possible to point to to your 
   modbus library CRC table that use the same polynomial.
   
   Some option you probably have :
   
   static const C_UINT16 CRCTABLE[]   
   
   #pragma memory=FLASH_REGION_1

      
   or something like that   
*/
typedef unsigned int CRC_TABLE_TYPE;












/* ======================================================= */
/*           END OF CUSTOMIZABLE SECTION                   */
/* ======================================================= */

#endif   /* #ifndef __CAREL_TYPES_H */
