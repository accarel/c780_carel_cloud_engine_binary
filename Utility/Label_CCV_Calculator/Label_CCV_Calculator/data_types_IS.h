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



/* you MUST change the data type to match the type your    */ 
/* compiler data types                                     */
typedef unsigned char       C_BYTE;      // 8 bit value
typedef char  			    C_SBYTE;     // 8 bit value
typedef char                C_CHAR;      // 8 bit value
typedef char   			    C_SCHAR;     // 8 bit value
typedef	short int           C_INT16;	 // 16-bit value
typedef	unsigned short int	C_UINT16;	 // 16-bit value
typedef	int		            C_INT32;	 // 32-bit value
typedef	unsigned int	    C_UINT32;	 // 32-bit value
typedef	unsigned long long int	C_UINT64;   // 64-bit value

/* floating point IEEE 754 little endian 4 bytes */
typedef float               C_FLOAT;    //32-bit value




/* ======================================================= */
/*           END OF CUSTOMIZABLE SECTION                   */
/* ======================================================= */

#endif   /* #ifndef __CAREL_TYPES_H */
