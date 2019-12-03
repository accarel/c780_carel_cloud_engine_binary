

#ifndef _MAIN_H
#define _MAIN_H


#include "types.h"
#include <stdint.h>
#include "common.h"
#include "data_types_CAREL.h"

// dim inside a model 
#define HEADER_SIGNATURE_DIM			(8)
#define HEADER_VERSION_DIM				(2)
#define HEADER_MODEL_GUID_DIM			(16)
#define HEADER_MODEL_VERSION_DIM		(4)
#define HEADER_RS485_STOPB				(1)
#define HEADER_RS485_PARITY				(1)

#define HEADER_NUMOF_COIL_POLL			(2)  // the same for low, hig, allarm
#define HEADER_NUMOF_COIL_DISC			(2)  // the same for low, hig, allarm
#define HEADER_NUMOF_COIL_HR			(2)  // the same for low, hig, allarm
#define HEADER_NUMOF_COIL_IR			(2)  // the same for low, hig, allarm

#define COIL_TABLE_LOW				(4)
#define COIL_TABLE_HIGH				(4)
#define COIL_TABLE_ALARM			(4)

#define DI_TABLE_LOW				(4)
#define DI_TABLE_HIGH				(4)
#define DI_TABLE_ALARM				(4)

#define HR_TABLE_LOW				(20)
#define HR_TABLE_HIGH				(20)
#define HR_TABLE_ALARM				(5)

#define IR_TABLE_LOW				(20)
#define IR_TABLE_HIGH				(20)
#define IR_TABLE_ALARM				(5)


// offset inside a model
#define HEADER_SIGNATURE_OFFSET		(0)
#define HEADER_VERSION_OFFSET		(8)
#define HEADER_MODEL_GUID_OFFSET	(10)
#define HEADER_MODEL_VERSION		(26)


#define GME_MODEL	"GME_MBT\x0"

#define __DEBUG_BYNARY_MODEL
#ifdef __DEBUG_BYNARY_MODEL
#define DEBUG_BINARY_MODEL(a) printf("%s: %s\n", __func__, a);
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



int BinaryModel_Init (void);
//int BinaryModel__GetNum(PollType_t polling_type, RegType_t reg_type);
uint8_t* get_p_coil_alarm_sect (void);
uint8_t* BinaryModel__GetPtrSec(PollType_t polling_type, RegType_t reg_type);
void BinaryModel__GetNum(uint8_t DeviceParamCount[MAX_POLLING][MAX_REG]);
uint8_t* BinaryModel__GetHeaderptr(void);
uint16_t BinaryModel_CalcModelCrc(void);

uint8_t* BinaryModel_GetChunk(uint32_t sz);
uint16_t BinaryModel_GetCrc(void);
C_RES BinaryModel_CheckCrc(void);









#endif
