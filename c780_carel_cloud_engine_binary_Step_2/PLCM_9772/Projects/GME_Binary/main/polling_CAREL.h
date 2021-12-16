/*
 * polling_CAREL.h
 *
 *  Created on:  9 Oct 2019
 *      Author: carel
 */

#ifndef _POLLING_CAREL_H_
#define _POLLING_CAREL_H_



/* ========================================================================== */
/* include                                                                    */
/* ========================================================================== */
#include "CAREL_GLOBAL_DEF.h"
#include "data_types_CAREL.h"

#include "common.h"
#include "binary_model.h"
#include "mbcontroller.h"

#include "CBOR_CAREL.h"
#include "mb_m.h"


//#include "mb_device_params.h"

/* ========================================================================== */
/* general purpose                                                            */
/* ========================================================================== */



/* ========================================================================== */
/* debugging purpose                                                          */
/* ========================================================================== */
#ifdef __CCL_DEBUG_MODE

//this define enable the output of the communications errors
//#define __DEBUG_POLLING_CAREL_LEV_1

//this define enable the output of others debug informations
//#define __DEBUG_POLLING_CAREL_LEV_2

#endif

/* ========================================================================== */
/* other                                                                      */
/* ========================================================================== */

/*  NB: Remember, the size of the polled variable 
 *	 	buffer depends on this size SENSE_TRIGGER_TASK_STACK_SIZE
 *   
 *      example (1024 * 6) --->  more than 300 variables
 *
 *				(1024 * 5) --->  ~238 variables
 *   
 */
#define SENSE_TRIGGER_TASK_STACK_SIZE   (1024 * 7)  //(1024 * 6)
#define SENSE_TRIGGER_TASK_PRIO         (6)

#define T_LOW_POLL	(30)   //(120)   //120
#define TSEND		(10*60)
#define T_HIGH_POLL	(10)   //(65)

#define SINGLE    	0
#define MULTI    	1

//Register: Coil and DI low polling and high polling
#pragma pack(1)
typedef struct coil_di_low_high_s{
	r_coil_di		 	info;
	uint8_t 			c_value:1;
	uint8_t 			p_value:1;
	uint8_t				error:3;
	uint8_t				p_error:3;
}coil_di_low_high_t;
#pragma pack()

// usefull to write a Holding regiaster via modbus
#pragma pack(1)
typedef union Data{
	   C_FLOAT value;
	   struct{
		   C_UINT16 low;
		   C_UINT16 high;
	   }reg;
}data_f;
#pragma pack()


#pragma pack(1)
typedef union Data_Int{
	   C_INT32 value;
	   struct{
		   C_UINT16 low;
		   C_UINT16 high;
	   }reg;
}data_int_f;
#pragma pack()


//Table: Coil and DI low polling and high polling tables
#pragma pack(1)
typedef struct coil_di_poll_tables_s{
	coil_di_low_high_t 	*reg;
	//uint32_t 			cur_time_stamp;
	//uint32_t			prev_time_stamp;
}coil_di_poll_tables_t;
#pragma pack()


//Register: Coil and DI alarm polling tables
#pragma pack(1)
typedef struct alarm_read_s{
	uint32_t	start_time;
	uint32_t	stop_time;
	uint8_t 	value:1;
	uint8_t		error:3;
	uint8_t 	send_flag:1;
	uint8_t 	dummy:3;
}alarm_read_t;
#pragma pack()

//Table: Coil and DI alarm polling tables
#pragma pack(1)
typedef struct alarm_tables_s{
	r_coil_di_alarm 	info;
	alarm_read_t		data;
}coil_di_alarm_tables_t;
#pragma pack()


//struct for HR and IR low polling and high polling

#pragma pack(1)
typedef enum{
	TYPE_A = 0,
	TYPE_B,
	TYPE_C_SIGNED,
	TYPE_C_UNSIGNED,
	TYPE_D,
	TYPE_E,
	TYPE_F_SIGNED,
	TYPE_F_UNSIGNED,
	MAX_TYPES,
}hr_ir_read_type_t;
#pragma pack()


#pragma pack(1)
typedef union hr_ir_low_high_value_s{
	int32_t value;
	struct{
		int16_t low;
		int16_t high;
	}reg;
}hr_ir_low_high_value_t;
#pragma pack()


#pragma pack(1)
typedef struct hr_ir_low_high_poll_s{
	r_hr_ir 				info;
	hr_ir_low_high_value_t c_value;
	hr_ir_low_high_value_t p_value;
	hr_ir_read_type_t	   read_type;
	uint8_t					error;
	uint8_t					p_error;
}hr_ir_low_high_poll_t;
#pragma pack()

//struct for HR and IR low polling and high polling tables
#pragma pack(1)
typedef struct hr_ir_poll_tables_s{
	hr_ir_low_high_poll_t 	*tab;

	//uint32_t 				cur_time_stamp;
	//uint32_t				prev_time_stamp;
}hr_ir_poll_tables_t;
#pragma pack()

//struct for HR and IR alarm polling
#pragma pack(1)
typedef struct hr_ir_alarm_s{
	uint32_t	start_time;
	uint32_t	stop_time;
	uint8_t 	value:1;
	uint8_t		error:3;
	uint8_t 	send_flag:1;
	uint8_t 	dummy:3;
}hr_ir_alarm_t;
#pragma pack()

//Table: Coil and DI alarm polling tables
#pragma pack(1)
typedef struct hr_ir_alarm_tables_s{
	r_hr_ir_alarm 		info;
	hr_ir_alarm_t		data;
}hr_ir_alarm_tables_t;
#pragma pack()

#pragma pack(1)
typedef struct poll_req_num_s{
	uint8_t coil;
	uint8_t di;
	uint8_t hr;
	uint8_t ir;
	uint16_t total;
}poll_req_num_t;
#pragma pack()



#pragma pack(1)
typedef struct mb_param_char_s{
	char p_ch[6];
}mb_param_char_t;
#pragma pack()

#pragma pack(1)
typedef struct sampling_tstamp{
	uint32_t current_alarm;
	uint32_t current_high;
	uint32_t current_low;
	uint32_t current_pva;
}sampling_tstamp_t;
#pragma pack()


#pragma pack(1)
typedef struct values_buffer_s{
	uint16_t 	alias;
	long double value;
	uint8_t		info_err;
	uint8_t     data_type;
	uint32_t 	t;
}values_buffer_t;
#pragma pack()



#pragma pack(1)
typedef struct values_buffer_timing_s{
	uint32_t 	t_current;
	uint16_t 	index;
}values_buffer_timing_t;
#pragma pack()



enum{
	CURRENT = 0,
	PREVIOUS,
};


enum{
	ERROR = 0,
	CHANGED,
};

enum{
	DEACTIVATED = 0,
	ACTIVATED,
};




enum{
	STOPPED = 0,
	INITIALIZED,
	RUNNING,
	NOT_INITIALIZED,
};

enum{
	NOT_RECEIVED = 0,
	RECEIVED,
	IN_PROGRESS,
	EXECUTED,
};


typedef enum{
	NOT_CREATED = 0,
	CREATED,
} table_st;

#pragma pack(1)
typedef struct table_status_s{
	table_st buffer_values;
	table_st cid_table;
}table_status_t;
#pragma pack()

#pragma pack(1)
typedef enum{
	START_TIMER = 0,
	WAIT_MQTT_CMD,
	RESET_TIMER,
	EXECUTE_CMD,
	DEACTIVATE_PASS_MODE,
}passing_mode_fsm_t;
#pragma pack()

#pragma pack(1)
typedef struct poll_engine_flags_s{
	 uint8_t engine;
	 uint8_t polling;
	 uint8_t passing_mode;
}poll_engine_flags_t;
#pragma pack()

#pragma pack(1)
typedef struct passing_mode_Param_s{
	uint8_t cmd_received;
	uint8_t cmd;

}passing_mode_Param_t;
#pragma pack()


void PollEngine__CreateTables(void);
void create_modbus_tables(void);
mb_parameter_descriptor_t* PollEngine__GetParamVectPtr(void);
uint16_t PollEngine__GetParamNum(void);

void PollEngine__MBInit(void);

void create_values_buffers(void);

void PollEngine_StartEngine_CAREL(void);
void PollEngine_StopEngine_CAREL(void);
uint8_t PollEngine_GetEngineStatus_CAREL(void);
uint8_t PollEngine_GetPollingStatus_CAREL(void);
uint8_t PollEngine_GetStatusForSending_CAREL(void);

C_RES PollEngine__Read_HR_IR_Req(C_UINT16 func, C_UINT16 addr,C_BYTE dim , C_UINT16* read_value);
C_RES PollEngine__Read_COIL_DI_Req(C_UINT16 func, C_UINT16 addr, C_UINT16* read_value);
C_RES PollEngine__Write_COIL_Req(uint16_t write_value, uint16_t addr, C_UINT16 fun);
C_RES PollEngine__Write_HR_Req(C_FLOAT write_value, uint16_t addr, C_CHAR num, C_BYTE is_big_end, C_UINT16 fun);
C_RES PollEngine__Write_HR_Req_Int(C_INT32 write_value, uint16_t addr, C_CHAR num, C_BYTE is_big_end, C_UINT16 fun);

values_buffer_t* PollEngine__GetValuesBuffer(void);
uint16_t PollEngine__GetValuesBufferCount(void);
void PollEngine__ResetValuesBuffer(void);
uint32_t PollEngine__GetMBBaudrate(void);

float get_type_a(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
float get_type_b(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
int32_t get_type_c_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
uint32_t get_type_c_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
uint8_t get_type_d(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
int16_t get_type_e(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
int16_t get_type_f_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
uint16_t get_type_f_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind);


hr_ir_read_type_t check_hr_ir_reg_type(r_hr_ir info);


void DoPolling_CAREL(req_set_gw_config_t *polling_times);
void CarelEngineMB_Init(void);

void adu_test(void);
void scanline_test(void);
uint8_t PollEngine__GetPollEnginePrintMsgs(void);
void PollEngine__SetPollEnginePrintMsgs(uint8_t status);

void FlushValues(PollType_t type);
void ForceSending(void);
void ResetFirst(PollType_t type);
uint8_t IsFirst(PollType_t type);
void PollEngine__ReadBaudRateFromNVM(void);
#define	PRINTF_POLL_ENG(x)	\
		if(PollEngine__GetPollEnginePrintMsgs() == 1)\
		printf x;\

bool IsOffline(void);
bool IsRealOffline(void);

#define MB_RESPONSE_TIMEOUT(size) pdMS_TO_TICKS(30 + (2 * ((size << 1) + 8) * 11 * 1000 / PollEngine__GetMBBaudrate()))

C_TIME Get_SamplingTime(C_UINT16 index);
C_CHAR* Get_Alias(C_UINT16 index, char* alias);
C_CHAR* Get_Value(C_UINT16 index, char* value);


#endif
