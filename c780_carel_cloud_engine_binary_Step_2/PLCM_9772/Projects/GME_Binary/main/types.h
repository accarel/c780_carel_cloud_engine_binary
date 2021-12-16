/*
********************************************************************************
* Filename : types.h
* Programmer(s): 
* Created :
* Modified :
* Description : Define of data type.
********************************************************************************
*/

#ifndef _TYPES_H_
#define _TYPES_H_

#include "common.h"

#ifndef NULL
# define NULL		((void *) 0)
#endif

//typedef enum { false, true } bool;

//#ifndef _SIZE_T
//#define _SIZE_T
//typedef unsigned int size_t;
//#endif

typedef uint8_t BYTE_T;     // 8 bit value
typedef	unsigned char	UCHAR;		// 8-bit value
typedef	         int	INT;		// 16-bit value
typedef	unsigned int	UINT;		// 16-bit value
typedef	unsigned short	USHORT;		// 16-bit value
typedef	unsigned short	WORD;		// 16-bit value
typedef	unsigned long	ULONG;		// 32-bit value
typedef	unsigned long	DWORD_2;		// 32-bit value

// bsd
typedef unsigned char	u_char;		// 8-bit value
typedef unsigned short	u_short;	// 16-bit value
typedef unsigned int    u_int;		// 32-bit value
typedef unsigned long	u_long;		// 64-bit value


#endif		// _TYPES_H_

