/*
 * polling_CAREL.h
 *
 *  Created on:  9 Oct 2019
 *      Author: carel
 */

#ifndef _POLLING_CAREL_H_
#define _POLLING_CAREL_H_

#include "data_types_CAREL.h"


#include "common.h"
#include "binary_model.h"
#include "mbcontroller.h"

#include "CBOR_CAREL.h"
#include "mb_m.h"


//#include "mb_device_params.h"

#define SENSE_TRIGGER_TASK_STACK_SIZE   (1024 * 4)
#define SENSE_TRIGGER_TASK_PRIO         (6)

#define T_LOW_POLL	(30)   //(120)   //120
#define TSEND		(10*60)
#define T_HIGH_POLL	(10)   //(65)

//Register: Coil and DI low polling and high polling
#pragma pack(1)
typedef struct coil_di_low_high_s{
	r_coil_di		 	info;
	uint8_t 			c_value:1;
	uint8_t 			p_value:1;
	uint8_t				error:3;
}coil_di_low_high_t;
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
	uint8_t 	value:1;
	uint8_t		error:3;
	uint8_t 	send_flag:1;
	// TODO...DA PADDARE OCIO AI BUSI
	uint32_t	start_time;
	uint32_t	stop_time;
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
	uint8_t 	value:1;
	uint8_t		error:3;
	uint8_t 	send_flag:1;
	uint32_t	start_time;
	uint32_t	stop_time;
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
	//uint32_t previous_alarm;
	uint32_t current_high;
	//uint32_t previous_high;
	uint32_t current_low;
	//uint32_t previous_low;
}sampling_tstamp_t;
#pragma pack()


#pragma pack(1)
typedef struct values_buffer_s{
	uint16_t 	index;
	uint16_t 	alias;
	long double value;
	uint8_t		info_err;
}values_buffer_t;
#pragma pack()



#pragma pack(1)
typedef struct values_buffer_timing_s{
	uint32_t 	t_start;
	uint32_t 	t_stop;
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

// commands
//void PollEngine__MBDestroy(void);
void PollEngine_StartEngine_CAREL(void);
void PollEngine_StopEngine_CAREL(void);
uint8_t PollEngine_GetEngineStatus_CAREL(void);
uint8_t PollEngine_GetPollingStatus_CAREL(void);

void PollEngine__PassModeFSM(void);
void PollEngine__ActivatePassMode(void);
void PollEngine__DeactivatePassMode(void);
uint8_t PollEngine__GetPassModeStatus(void);
void PollEngine__SetPassModeCMD(uint8_t status);
uint8_t PollEngine__GetPassModeCMD(void);


//uint8_t PollEngine__SendMBAdu(c_cbor_send_mb_adu *send_mb_adu, uint8_t* data_rx);
C_RES PollEngine__Read_HR_IR_Req(C_UINT16 func, C_UINT16 addr,C_BYTE dim , C_UINT16* read_value);

C_RES PollEngine__Read_COIL_DI_Req(C_UINT16 func, C_UINT16 addr, C_UINT16* read_value);

C_RES PollEngine__Write_COIL_Req(uint16_t write_value, uint16_t addr);

//C_RES PollEngine__Write_HR_Req(char* alias, void* write_value);


values_buffer_t* PollEngine__GetValuesBuffer(void);
values_buffer_timing_t* PollEngine__GetTimeBuffer(void);
uint16_t PollEngine__GetValuesBufferIndex(void);
uint16_t PollEngine__GetTimerBufferIndex(void);
void PollEngine__ResetValuesBuffer(void);
uint32_t PollEngine__GetMBBaudrate(void);

float get_type_a(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
float get_type_b(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
int32_t get_type_c_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
uint32_t get_type_c_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
uint8_t get_type_d(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
int32_t get_type_e(hr_ir_low_high_poll_t *arr, uint8_t read_kind);
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

#define	PRINTF_POLL_ENG(x)	\
		if(PollEngine__GetPollEnginePrintMsgs() == 1)\
		printf x;\



#define MB_RESPONSE_TIMEOUT(size) pdMS_TO_TICKS(30 + (2 * ((size << 1) + 8) * 11 * 1000 / PollEngine__GetMBBaudrate()))


#endif
