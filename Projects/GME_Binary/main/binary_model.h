/**
 * @file binary_model.c
 * @author carel
 * @date   9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         binary model used to pooling the connected device
 */

#ifndef _BINARY_MODEL_H
#define _BINARY_MODEL_H

#include "CAREL_GLOBAL_DEF.h"
#include <stdint.h>
#include "types.h"
#include "common.h"
#include "data_types_CAREL.h"

/* ========================================================================== */
/* typedefs and defines                                                       */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

//this define enable the output of the communications errors
#define __DEBUG_BINARY_MODEL_LEV_1

//this define enable the output of others debug informations
#define __DEBUG_BINARY_MODEL_LEV_2

#endif

/**
 * @brief GME_MODEL
 *        device model signature
 */
#define GME_MODEL			"GME_MBT\x0"
#define HEADER_VERSION 		256
#define GME_MODEL_MAX_SIZE	2048


#ifdef __DEBUG_BINARY_MODEL_LEV_1
#define DEBUG_BINARY_MODEL(a) printf("%s: %s\n", __func__, a);
#else
#define	DEBUG_BINARY_MODEL(...)
#endif


/*
   Header struct with GME info
*/

#pragma pack(1)
typedef struct HeaderModel{
	    uint8_t    signature[8];
		uint16_t   version;
		uint8_t    guid[16];
		u_long    modelVer;       
		uint8_t    Rs485Stop;
		uint8_t    Rs485Parity;
}H_HeaderModel;
#pragma pack()

#pragma pack(1)
typedef struct NumOfPoll{
	uint16_t numOfCOIL;
	uint16_t numOfDISC;
	uint16_t numOfHR;
	uint16_t numOfIR;
}myNumOfPoll;
#pragma pack()


#pragma pack(1)
// for coil and DI
typedef struct record_coil_di{
	uint16_t Alias;
	uint16_t Addr;
}r_coil_di;
#pragma pack()

#pragma pack(1)
typedef struct record_coil_di_alarm {
	uint16_t Alias;
	uint16_t Addr;
}r_coil_di_alarm;
#pragma pack()


#pragma pack(1)
typedef union flag_s{
	uint8_t  byte;
	struct{
		uint8_t  fixedpoint:1;
		uint8_t  bit1:1;
		uint8_t  ieee:1;
		uint8_t  bit3:1;
		uint8_t  bit4:1;
		uint8_t  bit5:1;
		uint8_t  signed_f:1;
		uint8_t  bigendian:1;
	}bit;
}flag_t;
#pragma pack(1)




#pragma pack(1)
// for Hr and IR
typedef struct record_hr_ir{
	uint16_t Alias;
	uint16_t Addr;
	flag_t	 flag;
	uint8_t  dim;
	uint8_t  bitposition;
	uint8_t  len;
	float   linA;
	float   linB;
	float   Hyster;
}r_hr_ir;
#pragma pack()


#pragma pack(1)
typedef struct record_hr_ir_alarm {
	uint16_t Alias;
	uint16_t Addr;
	uint8_t  dim;
}r_hr_ir_alarm;
#pragma pack()

typedef enum{
	LOW_POLLING = 0,
	HIGH_POLLING,
	ALARM_POLLING,
	MAX_POLLING,
}PollType_t;


typedef enum{
	COIL = 0,
	DI,
	HR,
	IR,
	MAX_REG,
}RegType_t;

uint16_t CRC16(const uint8_t *nData, uint16_t wLength);
int BinaryModel_Init (void);
//int BinaryModel__GetNum(PollType_t polling_type, RegType_t reg_type);
uint8_t* get_p_coil_alarm_sect (void);
uint8_t* BinaryModel__GetPtrSec(PollType_t polling_type, RegType_t reg_type);
void BinaryModel__GetNum(uint8_t DeviceParamCount[MAX_POLLING][MAX_REG]);
uint16_t BinaryModel_CalcModelCrc(void);

uint8_t* BinaryModel_GetChunk(long sz);
uint16_t BinaryModel_GetCrc(void);
C_RES BinaryModel_CheckCrc(void);

bool CheckModelValidity(void);



#endif
