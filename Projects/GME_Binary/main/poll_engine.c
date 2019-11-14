/*
 * poll_engine.c
 *
 *  Created on: Jul 25, 2019
 *      Author: ataayoub
 *
 *  Description: This file contains the high level functions of polling engine :
 *  	1) Polling routine
 *  	2) Conversion functions
 *  	3) Buffering routine
 *  	4) The passing mode state machine
 *  	5) Polling tables creation
 *
 */
#include "poll_engine.h"
#include "binary_model.h"
#include "utilities.h"

#include "RTC_IS.h"
#include "modbus_IS.h"
#include "nvm.h"
#include "string.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "mb_m.h"

#include "MQTT_Interface_CAREL.h"
#include "data_types_CAREL.h"
//#define __DEBUG_POLL_ENGINE_CAREL


static const char *TAG = "sense_main";

#define OPTS(min_val, max_val, step_val) { .opt1 = min_val, .opt2 = max_val, .opt3 = step_val }


#define INIT_DELAY_TICS                 (100 / portTICK_RATE_MS)
#define TIMEOUT_UPDATE_CIDS_MS          (1000) // 1000
#define TIMEOUT_UPDATE_CIDS_TICS        (TIMEOUT_UPDATE_CIDS_MS / portTICK_RATE_MS)

#define SENSE_TRIGGER_TASK_STACK_SIZE   (1024 * 4)
#define SENSE_TRIGGER_TASK_PRIO         (6)

#define SENSE_MAIN_TAG "SENSE_MAIN"

#define SENS_MAIN_CHECK(a, ret_val, str, ...) \
    if (!(a)) { \
        ESP_LOGE(SENSE_MAIN_TAG, "%s(%u): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        return (ret_val);\
    }



static poll_engine_flags_t PollEngine_Status = {
	.engine = NOT_INITIALIZED,
	.polling = STOPPED,
	.passing_mode = DEACTIVATED,
};


static uint8_t DeviceParamCount[MAX_POLLING][MAX_REG] = {0};

static coil_di_poll_tables_t 	COILLowPollTab = {.reg = NULL,};
static coil_di_poll_tables_t 	COILHighPollTab = {.reg = NULL,};
static coil_di_alarm_tables_t 	*COILAlarmPollTab = NULL;

static coil_di_poll_tables_t 	DILowPollTab = {.reg = NULL,};
static coil_di_poll_tables_t 	DIHighPollTab = {.reg = NULL,};
static coil_di_alarm_tables_t 	*DIAlarmPollTab = NULL;

static mb_parameter_descriptor_t *MBParameters = NULL;
//static mb_param_char_t			 MBParamChar = {.p_ch = NULL,};
static mb_param_char_t			 *MBParamChar = NULL;


static poll_req_num_t low_n, high_n, alarm_n;
static uint16_t  cid_counter=0;

static hr_ir_poll_tables_t 		HRLowPollTab;
static hr_ir_poll_tables_t 		HRHighPollTab;
static hr_ir_alarm_tables_t 	*HRAlarmPollTab;


static hr_ir_poll_tables_t 		IRLowPollTab;
static hr_ir_poll_tables_t 		IRHighPollTab;
static hr_ir_alarm_tables_t		*IRAlarmPollTab;

static sampling_tstamp_t timestamp = {0};

//Values and time buffers
static uint16_t values_buffer_index = 0;
static uint16_t values_buffer_read_section = 1;
static uint16_t values_buffer_len = 0;
static values_buffer_t *values_buffer = NULL;
static values_buffer_timing_t *time_values_buff = NULL;
static uint16_t time_values_buff_len = 0;


static uint8_t test = 0;

//Engine flags
static passing_mode_fsm_t PassMode_FSM = START_TIMER;
static uint32_t PassModeTimer = 0;
static uint8_t PassMode_CmdStatus = NOT_RECEIVED;
static uint32_t MB_BaudRate = 0;
static uint16_t NullTimerAlarm = 0;
static xTaskHandle xPollingEngine;



USHORT param_buffer[2];				// max 32 bits
eMBErrorCode retError = MB_ENOREG;

static uint8_t PollEnginePrint = POLL_ENGINE_PRINTF_DEFAULT;

/*Static Function*/
static void create_values_buffers(void);
static void check_increment_values_buff_len(uint16_t *values_buffer_idx);
static void check_hr_ir_read_val(hr_ir_poll_tables_t *arr, uint8_t arr_len);
static void check_coil_di_read_val(coil_di_poll_tables_t *arr, uint8_t arr_len);
static void compare_prev_curr_reads(PollType_t poll_type);
static void save_coil_di_value(coil_di_low_high_t *arr, void* instance_ptr);
static void save_hr_ir_value(hr_ir_low_high_poll_t *arr, void* instance_ptr);
static void save_alarm_coil_di_value(coil_di_alarm_tables_t *alarm,  void* instance_ptr);
static void save_alarm_hr_ir_value(hr_ir_alarm_tables_t *alarm, void* instance_ptr);


/**
 * @brief create_tables
 *        this function creates the Coil, Di, Hr and Ir buffers
 *        starting from the file system table
 *
 * @param  none
 * @return none
 */
void PollEngine__CreateTables(void){

	BinaryModel__GetNum(DeviceParamCount);

	//Coil
#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("\n\nCREATE TABLES %d\n\n",test++);
#endif
	uint8_t temp=0;
#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("COIL LOW TABLE:\n");
#endif
	temp = DeviceParamCount[LOW_POLLING][COIL];
	if(0 != temp){
		COILLowPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&COILLowPollTab.reg[0] , 0, sizeof(coil_di_low_high_t));
		uint8_t  *p_coil_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILLowPollTab.reg[i].info =  *((r_coil_di*)(p_coil_low_sect + (i * sizeof(r_coil_di))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%d, %d\n",COILLowPollTab.reg[i].info.Alias, COILLowPollTab.reg[i].info.Addr);
#endif
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][COIL];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("COIL HIGH TABLE:\n");
#endif
		COILHighPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&COILHighPollTab.reg[0] , 0, sizeof(coil_di_low_high_t));
		uint8_t  *p_coil_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILHighPollTab.reg[i].info =  *((r_coil_di*)(p_coil_high_sect + (i * sizeof(r_coil_di))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%d, %d\n",COILHighPollTab.reg[i].info.Alias, COILHighPollTab.reg[i].info.Addr);
#endif
		}

	}


	temp = DeviceParamCount[ALARM_POLLING][COIL];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("COIL ALARM TABLE:\n");
#endif
		COILAlarmPollTab = malloc(temp * sizeof(coil_di_alarm_tables_t));
		memset((void*)&COILAlarmPollTab[0] , 0, sizeof(coil_di_alarm_tables_t));
		uint8_t  *p_coil_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILAlarmPollTab[i].info =  *((r_coil_di_alarm*)(p_coil_alarm_sect + (i * sizeof(r_coil_di_alarm))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d\n",COILAlarmPollTab[i].info.Alias, COILAlarmPollTab[i].info.Addr);
#endif
		}
	}

	//Descrete Input

	temp = DeviceParamCount[LOW_POLLING][DI];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("DI LOW TABLE:\n");
#endif
		DILowPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&DILowPollTab.reg[0] , 0, sizeof(coil_di_low_high_t));
		uint8_t  *p_di_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, DI);
		for(int i=0;i<temp;i++){
			DILowPollTab.reg[i].info =  *((r_coil_di*)(p_di_low_sect + (i * sizeof(r_coil_di))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%d, %d\n",DILowPollTab.reg[i].info.Alias, DILowPollTab.reg[i].info.Addr);
#endif
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][DI];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("DI HIGH TABLE:\n");
#endif
		DIHighPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&DIHighPollTab.reg[0] , 0, sizeof(coil_di_low_high_t));
		uint8_t  *p_di_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, DI);
		for(int i=0;i<temp;i++){
			DIHighPollTab.reg[i].info =  *((r_coil_di*)(p_di_high_sect + (i * sizeof(r_coil_di))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%d, %d\n",DIHighPollTab.reg[i].info.Alias, DIHighPollTab.reg[i].info.Addr);
#endif
		}
	}


	temp = DeviceParamCount[ALARM_POLLING][DI];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("DI ALARM TABLE:\n");
#endif
		DIAlarmPollTab = malloc(temp * sizeof(coil_di_alarm_tables_t));
		memset((void*)&DIAlarmPollTab[0] , 0, sizeof(coil_di_alarm_tables_t));
		uint8_t  *p_di_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, DI);
		for(int i=0;i<temp;i++){
			DIAlarmPollTab[i].info =  *((r_coil_di_alarm*)(p_di_alarm_sect + (i * sizeof(r_coil_di_alarm))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%d, %d\n",DIAlarmPollTab[i].info.Alias, DIAlarmPollTab[i].info.Addr);
#endif
		}
	}

	//Holding Register

	temp = DeviceParamCount[LOW_POLLING][HR];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("HR LOW TABLE:\n");
#endif
		HRLowPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&HRLowPollTab.tab[0] , 0, sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_hr_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRLowPollTab.tab[i].info =  *((r_hr_ir*)(p_hr_low_sect + (i * sizeof(r_hr_ir))));
			HRLowPollTab.tab[i].read_type = check_hr_ir_reg_type(HRLowPollTab.tab[i].info);
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d, %4x, %4d, %4d, %4d, %4f, %4f, %4f, \n",
					HRLowPollTab.tab[i].info.Alias,
					HRLowPollTab.tab[i].info.Addr,
					HRLowPollTab.tab[i].info.flag.byte,
					HRLowPollTab.tab[i].info.dim,
					HRLowPollTab.tab[i].info.bitposition,
					HRLowPollTab.tab[i].info.len,
					HRLowPollTab.tab[i].info.linA,
					HRLowPollTab.tab[i].info.linB,
					HRLowPollTab.tab[i].info.Hyster);
#endif
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][HR];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("HR HIGH TABLE:\n");
#endif

		HRHighPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&HRHighPollTab.tab[0] , 0, sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_hr_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRHighPollTab.tab[i].info =  *((r_hr_ir*)(p_hr_high_sect + (i * sizeof(r_hr_ir))));
			HRHighPollTab.tab[i].read_type = check_hr_ir_reg_type(HRHighPollTab.tab[i].info);
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d, %4d, %4d, %4d, %4d, %4f, %4f, %4f, \n",
					HRHighPollTab.tab[i].info.Alias,
					HRHighPollTab.tab[i].info.Addr,
					HRHighPollTab.tab[i].info.flag.byte,
					HRHighPollTab.tab[i].info.dim,
					HRHighPollTab.tab[i].info.bitposition,
					HRHighPollTab.tab[i].info.len,
					HRHighPollTab.tab[i].info.linA,
					HRHighPollTab.tab[i].info.linB,
					HRHighPollTab.tab[i].info.Hyster);
#endif
		}
	}


	temp = DeviceParamCount[ALARM_POLLING][HR];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("HR ALARM TABLE:\n");
#endif
		HRAlarmPollTab = malloc(temp * sizeof(hr_ir_alarm_tables_t));
		memset((void*)&HRAlarmPollTab[0] , 0, sizeof(hr_ir_alarm_tables_t));
		uint8_t  *p_hr_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRAlarmPollTab[i].info =  *((r_hr_ir_alarm*)(p_hr_alarm_sect + (i * sizeof(r_hr_ir_alarm))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d, %4d, \n",
					HRAlarmPollTab[i].info.Alias,
					HRAlarmPollTab[i].info.Addr,
					HRAlarmPollTab[i].info.dim);
#endif
		}
	}

	//Input Register


	temp = DeviceParamCount[LOW_POLLING][IR];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("IR LOW TABLE:\n");
#endif
		IRLowPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&IRLowPollTab.tab[0] , 0, sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_ir_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRLowPollTab.tab[i].info =  *((r_hr_ir*)(p_ir_low_sect + (i * sizeof(r_hr_ir))));
			IRLowPollTab.tab[i].read_type = check_hr_ir_reg_type(IRLowPollTab.tab[i].info);
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d, %4x, %4d, %4d, %4d, %4f, %4f, %4f, \n",
					IRLowPollTab.tab[i].info.Alias,
					IRLowPollTab.tab[i].info.Addr,
					IRLowPollTab.tab[i].info.flag.byte,
					IRLowPollTab.tab[i].info.dim,
					IRLowPollTab.tab[i].info.bitposition,
					IRLowPollTab.tab[i].info.len,
					IRLowPollTab.tab[i].info.linA,
					IRLowPollTab.tab[i].info.linB,
					IRLowPollTab.tab[i].info.Hyster);
#endif
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][IR];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("IR HIGH TABLE:\n");
#endif
		IRHighPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&IRHighPollTab.tab[0] , 0, sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_ir_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRHighPollTab.tab[i].info =  *((r_hr_ir*)(p_ir_high_sect + (i * sizeof(r_hr_ir))));
			IRHighPollTab.tab[i].read_type = check_hr_ir_reg_type(IRHighPollTab.tab[i].info);
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d, %4d, %4d, %4d, %4d, %4f, %4f, %4f, \n",
					IRHighPollTab.tab[i].info.Alias,
					IRHighPollTab.tab[i].info.Addr,
					IRHighPollTab.tab[i].info.flag.byte,
					IRHighPollTab.tab[i].info.dim,
					IRHighPollTab.tab[i].info.bitposition,
					IRHighPollTab.tab[i].info.len,
					IRHighPollTab.tab[i].info.linA,
					IRHighPollTab.tab[i].info.linB,
					IRHighPollTab.tab[i].info.Hyster);
#endif
		}
	}


	temp = DeviceParamCount[ALARM_POLLING][IR];
	if(0 != temp){
#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("IR ALARM TABLE:\n");
#endif
		IRAlarmPollTab = malloc(temp * sizeof(hr_ir_alarm_tables_t));
		memset((void*)&IRAlarmPollTab[0] , 0, sizeof(hr_ir_alarm_tables_t));
		uint8_t  *p_ir_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRAlarmPollTab[i].info =  *((r_hr_ir_alarm*)(p_ir_alarm_sect + (i * sizeof(r_hr_ir_alarm))));
#ifdef __DEBUG_POLL_ENGINE_CAREL
			printf("%4d, %4d, %4d, \n",
					IRAlarmPollTab[i].info.Alias,
					IRAlarmPollTab[i].info.Addr,
					IRAlarmPollTab[i].info.dim);
#endif
		}
	}

#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("Finish Table\n");
#endif


	create_modbus_tables();
}


/**
 * @brief create_modbus_tables
 *        this function extracts the information of how many
 *        coil has to be polled with low frequency, how many
 *        coil has to be polled with low high frequency,
 *        and so on...
 *
 * @param  none
 * @return none
 */
void create_modbus_tables(void)
{
	low_n.coil	=  	DeviceParamCount[LOW_POLLING][COIL];
	low_n.di 	=	DeviceParamCount[LOW_POLLING][DI];
	low_n.hr 	=	DeviceParamCount[LOW_POLLING][HR];
	low_n.ir 	=	DeviceParamCount[LOW_POLLING][IR];
	low_n.total	=  	low_n.coil + low_n.di + low_n.hr + low_n.ir;


	high_n.coil 	=	DeviceParamCount[HIGH_POLLING][COIL];
	high_n.di 	=	DeviceParamCount[HIGH_POLLING][DI];
	high_n.hr 	=	DeviceParamCount[HIGH_POLLING][HR];
	high_n.ir 	=	DeviceParamCount[HIGH_POLLING][IR];
	high_n.total =  	high_n.coil + high_n.di + high_n.hr + high_n.ir;

	alarm_n.coil =	DeviceParamCount[ALARM_POLLING][COIL];
	alarm_n.di 	=	DeviceParamCount[ALARM_POLLING][DI];
	alarm_n.hr 	=	DeviceParamCount[ALARM_POLLING][HR];
	alarm_n.ir 	=	DeviceParamCount[ALARM_POLLING][IR];
	alarm_n.total =  alarm_n.coil + alarm_n.di + alarm_n.hr + alarm_n.ir;


	cid_counter = low_n.total + high_n.total + alarm_n.total;
}


static void create_values_buffers(void){
	//Allocate Time buffer for values buffer
	time_values_buff_len = (uint16_t)(ceil(TSEND / T_LOW_POLL) + ceil(TSEND / T_HIGH_POLL));
#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("\nTIME BUFF VALUES LEN = %d\n",time_values_buff_len);
#endif

	time_values_buff = malloc(time_values_buff_len * sizeof(values_buffer_timing_t));           // malloc
	memset((void*)time_values_buff, 0, time_values_buff_len * sizeof(values_buffer_timing_t));
	//Allocate values buffer
	uint32_t  freespace = uxTaskGetStackHighWaterMark(NULL);

#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("\nTask stack remaining free memory = %d\n", freespace);
#endif

	freespace -= 1000;
	values_buffer_len = (uint16_t)(freespace/((uint32_t)sizeof(values_buffer_t)));

#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("\nBUFF VALUES LEN = %d\n",values_buffer_len);
#endif

	values_buffer = malloc(values_buffer_len * sizeof(values_buffer_t));						// malloc
	memset((void*)values_buffer, 0, values_buffer_len * sizeof(values_buffer_t));

#ifdef __DEBUG_POLL_ENGINE_CAREL
	printf("Creation Done\n");
#endif

}



float get_type_a(hr_ir_low_high_poll_t *arr, uint8_t read_kind){

	float temp, read= 0.0;
	read_kind == CURRENT ? (temp = *((float*)(&arr->c_value.value))) : (temp = *((float*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


float get_type_b(hr_ir_low_high_poll_t *arr, uint8_t read_kind){

	float temp, read= 0.0;
	read_kind == CURRENT ? (temp = *((float*)(&arr->c_value.value))) : (temp = *((float*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


int32_t get_type_c_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	int32_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((int32_t*)(&arr->c_value.value))) : (temp = *((int32_t*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


uint32_t get_type_c_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint32_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((uint32_t*)(&arr->c_value.value))) : (temp = *((uint32_t*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


uint8_t get_type_d(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint16_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((uint16_t*)(&arr->c_value.value))) : (temp = *((uint16_t*)(&arr->p_value.value)));
	read = temp & ((uint16_t) (1 << arr->info.bitposition)) ;
	return (uint8_t)read;
}


int32_t get_type_e(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	int32_t read= 0;
	read_kind == CURRENT ? (read = *((uint16_t*)(&arr->c_value.value))) : (read = *((uint16_t*)(&arr->p_value.value)));
	return (int32_t)read;
}


int16_t get_type_f_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	int16_t temp, read= 0;

	read_kind == CURRENT ? (temp = *((int16_t*)(&arr->c_value.value))) : (temp = *((int16_t*)(&arr->p_value.value)));
	//printf("get_type_f_signed\n");
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


uint16_t get_type_f_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint16_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((uint16_t*)(&arr->c_value.value))) : (temp = *((uint16_t*)(&arr->p_value.value)));
	//printf("get_type_f_unsigned\n");
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


/*	Descriptions: Routine to check if the values buffer has a free space or not
 *					If yes, increments the index, otherwise flush the buffer via mqtt
 */
static void check_increment_values_buff_len(uint16_t *values_buffer_idx){
	if(*values_buffer_idx < (values_buffer_len - 1)){
		(*values_buffer_idx)++;
	}else{
		*values_buffer_idx = 0;
	//	MQTT__FlushValues();
	}

}

/*	Descriptions: Comparing the current read with previous IR and HR registers reads
 * 					accoring to its type (Look carel registers type).
 *					If there is a diff >= hysteresis, writes the read value + reg info
 *					in values buffer, then increment the values buffer index
 *
 *					arr: is the HR or IR table
 *					arr_len: the table length
 */
static void check_hr_ir_read_val(hr_ir_poll_tables_t *arr, uint8_t arr_len)
{
	static int first_run_h=1;
bool to_values_buff = false;
long double value = 0;
	for(uint8_t i=0; i<arr_len; i++){
		if(0 != arr->tab[i].error){
			values_buffer[values_buffer_index].alias = arr->tab[i].info.Alias;
			values_buffer[values_buffer_index].value = 0;
			values_buffer[values_buffer_index].info_err = arr->tab[i].error;
			check_increment_values_buff_len(&values_buffer_index);
		}
		else{
			// reinit value otherwise all variables will be considered changed
			value = 0;
			switch(arr->tab[i].read_type){
			case TYPE_A:
			{
				PRINTF_DEBUG("check_hr_ir_read_val A \n");
				float temp, c_read, p_read= 0.0;
				c_read = get_type_a(&arr->tab[i], CURRENT);
				p_read = get_type_a(&arr->tab[i], PREVIOUS);
				temp = fabs(c_read - p_read);
				PRINTF_DEBUG("c_read: %f, p_read: %f, temp: %f\n",c_read, p_read, temp);
				if(to_values_buff > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
					PRINTF_DEBUG("TYPE_A c_read = %f\n",c_read);
					PRINTF_DEBUG("TYPE_A Value = %Lf\n",value);
				}
			}
				break;

			case TYPE_B:
			{
				float temp, c_read, p_read= 0.0;
				c_read = get_type_b(&arr->tab[i], CURRENT);
				p_read = get_type_b(&arr->tab[i], PREVIOUS);
				temp = fabs(c_read - p_read);
				PRINTF_DEBUG("c_read: %f, p_read: %f, temp: %f\n",c_read, p_read, temp);
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
					PRINTF_DEBUG("TYPE_B REG low = %d\n",arr->tab[i].c_value.reg.low);
					PRINTF_DEBUG("TYPE_B REG high = %d\n",arr->tab[i].c_value.reg.high);
					PRINTF_DEBUG("TYPE_B REG val = %d\n",arr->tab[i].c_value.value);
					PRINTF_DEBUG("TYPE_B c_read = %f\n",c_read);
					PRINTF_DEBUG("TYPE_B Value = %Lf\n",value);
				}
			}
				break;

			case TYPE_C_SIGNED:
			{
				int32_t temp, c_read, p_read= 0;
				c_read = get_type_c_signed(&arr->tab[i], CURRENT);
				p_read = get_type_c_signed(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
				}
			}
				break;

			case TYPE_C_UNSIGNED:
			{
				uint32_t temp, c_read, p_read= 0;
				c_read = get_type_c_unsigned(&arr->tab[i], CURRENT);
				p_read = get_type_c_unsigned(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
				}
			}
				break;

			case TYPE_D:
			{
				uint8_t c_read, p_read= 0;
				c_read = get_type_d(&arr->tab[i], CURRENT);
				p_read = get_type_d(&arr->tab[i], PREVIOUS);
				if(c_read != p_read){
					to_values_buff = true;
					value = (long double)c_read;
				}
				PRINTF_DEBUG("c_read: %d, p_read: %d\n",c_read, p_read);
			}
				break;

			case TYPE_E:
			{
				int32_t temp, c_read, p_read= 0;
				c_read = get_type_e(&arr->tab[i], CURRENT);
				p_read = get_type_e(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
				}
				PRINTF_DEBUG("c_read: %d, p_read: %d\n",c_read, p_read);

			}
				break;

			case TYPE_F_SIGNED:
			{
				int16_t temp, c_read, p_read= 0;
				c_read = get_type_f_signed(&arr->tab[i], CURRENT);
				p_read = get_type_f_signed(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
				}
			}
					break;

			case TYPE_F_UNSIGNED:
			{
				uint16_t temp, c_read, p_read= 0;
				c_read = get_type_f_unsigned(&arr->tab[i], CURRENT);
				p_read = get_type_f_unsigned(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;
					value = (long double)c_read;
				}
			}
				break;

			default:
				break;
			}
			if(value != 0 || (first_run_h <= 4)){
				values_buffer[values_buffer_index].alias = arr->tab[i].info.Alias;
				values_buffer[values_buffer_index].value = value;
				values_buffer[values_buffer_index].info_err = 0;
				check_increment_values_buff_len(&values_buffer_index);
			}
		}
	}
	first_run_h++;
}

/*	Descriptions: Comparing the current read with previous COIL and IR reads
 * 					accoring to its type (Look carel registers type).
 *					If there is a diff >= hysteresis, writes the read value + reg info
 *					in values buffer, then increment the values buffer index
 *
 *					arr: is the COIL or DI table
 *					arr_len: the table length
 */
static void check_coil_di_read_val(coil_di_poll_tables_t *arr, uint8_t arr_len)
{
	static uint8_t first_run_c = 1;
	for(uint8_t i=0; i<arr_len; i++){
		//error?
		if(0 != arr->reg[i].error){
			//send values to values buffer as error
			values_buffer[values_buffer_index].alias = arr->reg[i].info.Alias;
			values_buffer[values_buffer_index].value = 0;
			values_buffer[values_buffer_index].info_err = arr->reg[i].error;
			check_increment_values_buff_len(&values_buffer_index);

		}
		//value changed
		else if(arr->reg[i].c_value != arr->reg[i].p_value || (first_run_c <= 4)){
			//send values to values buffer
			values_buffer[values_buffer_index].alias = arr->reg[i].info.Alias;
			values_buffer[values_buffer_index].value = (long double)arr->reg[i].c_value;
			values_buffer[values_buffer_index].info_err = 0;
			check_increment_values_buff_len(&values_buffer_index);
		}
	}
	first_run_c++;// = 0;
}


/*	Descriptions: Routine that manages the hole comparison phase of CURRENT / PREVIOUS reads
 * 					according to polling type if HIGH or LOW
 * 					Then updates the values and time buffers
 * 					Should be called directly after finishing the polling routine
 */

static void compare_prev_curr_reads(PollType_t poll_type)
{
	//get current index of values buffer
	uint16_t index_temp =  values_buffer_index;
	PRINTF_DEBUG("START index_temp = %d, values_buffer_index = %d\n",index_temp,values_buffer_index);

	switch(poll_type){
	case LOW_POLLING:
		check_coil_di_read_val(&COILLowPollTab, low_n.coil);
		check_coil_di_read_val(&DILowPollTab, low_n.di);
		check_hr_ir_read_val(&HRLowPollTab,  low_n.hr);
		check_hr_ir_read_val(&IRLowPollTab,  low_n.ir);

		break;

	case HIGH_POLLING:
		check_coil_di_read_val(&COILHighPollTab, high_n.coil);
		check_coil_di_read_val(&DIHighPollTab, high_n.di);
		check_hr_ir_read_val(&HRHighPollTab,  high_n.hr);
		check_hr_ir_read_val(&IRHighPollTab,  high_n.ir);
		break;

	default:
		break;
	}

	PRINTF_DEBUG("END index_temp = %d, values_buffer_index = %d\n",index_temp,values_buffer_index);

	//Update values buffer idx
	if (index_temp != values_buffer_index){


		 //Update Values Buffer
		 if(index_temp < values_buffer_index){
			//assign idx to the new added values in the buffer
			 for(uint8_t i = index_temp; i < values_buffer_index; i++){
				 values_buffer[i].index = values_buffer_read_section;
			 }
		 }


		 //Update Time Buffer
		 switch(poll_type){
			case LOW_POLLING:
				//assign idx to time buffer
			time_values_buff[values_buffer_read_section-1].index = values_buffer_read_section;
				//assign time in time buffer
//				time_values_buff[values_buffer_read_section-1].t_start = timestamp.previous_low;
				time_values_buff[values_buffer_read_section-1].t_stop = timestamp.current_low;
				break;

			case HIGH_POLLING:
				//assign idx to time buffer
	time_values_buff[values_buffer_read_section-1].index = values_buffer_read_section;
				//assign time in time buffer
//				time_values_buff[values_buffer_read_section-1].t_start = timestamp.previous_high;
				time_values_buff[values_buffer_read_section-1].t_stop = timestamp.current_high;
				break;

			default:
				break;
		}

		 //Add new section of values in buffer values and time buffer
		 values_buffer_read_section++;
	}




}


hr_ir_read_type_t check_hr_ir_reg_type(r_hr_ir info)
{
	hr_ir_read_type_t type;
	if(info.dim > 16){
		if(1 == info.flag.bit.ieee){
			type = TYPE_A;
		}else{
			if(1 == info.flag.bit.signed_f){
				type = TYPE_C_SIGNED;
			}else{
				type = TYPE_C_UNSIGNED;
			}
		}
	}else{
		if((1 == info.len) && (0 != info.bitposition)){
			type = TYPE_D;
		}else if(1 == info.flag.bit.fixedpoint){
			type = TYPE_B;
		}else if(info.dim < 16 && 0 != info.bitposition){
			type = TYPE_E;
		}else{
			if(1 == info.flag.bit.signed_f){
				type = TYPE_F_SIGNED;
			}else{
				type = TYPE_F_UNSIGNED;
			}
		}
	}
	return type;

}



/*
 *	Description: Updating the previous read with the current one.
 */
static void update_current_previous_tables(RegType_t poll_type){
	int i=0;
	switch(poll_type){
	case LOW_POLLING:
		//Coil
		for(i=0;i<low_n.coil;i++){
			COILLowPollTab.reg[i].p_value = COILLowPollTab.reg[i].c_value;
			COILLowPollTab.reg[i].c_value = 0;
		}
		//DI
		for(i=0;i<low_n.di;i++){
			DILowPollTab.reg[i].p_value = DILowPollTab.reg[i].c_value;
			DILowPollTab.reg[i].c_value = 0;
		}
		//HR
		for(int i=0;i<low_n.hr;i++){
			HRLowPollTab.tab[i].p_value.value = HRLowPollTab.tab[i].c_value.value;
			HRLowPollTab.tab[i].c_value.value = 0;
		}
		//IR
		for(int i=0;i<high_n.ir;i++){
			IRLowPollTab.tab[i].p_value.value = IRLowPollTab.tab[i].c_value.value;
			IRLowPollTab.tab[i].c_value.value = 0;
		}
		break;

	case HIGH_POLLING:
		//Coil
		for(i=0;i<high_n.coil;i++){
			COILHighPollTab.reg[i].p_value = COILHighPollTab.reg[i].c_value;
			COILHighPollTab.reg[i].c_value = 0;
		}
		//DI
		for(i=0;i<high_n.di;i++){
			DIHighPollTab.reg[i].p_value = DIHighPollTab.reg[i].c_value;
			DIHighPollTab.reg[i].c_value = 0;
		}
		//HR
		for(int i=0;i<high_n.hr;i++){
			HRHighPollTab.tab[i].p_value.value = HRHighPollTab.tab[i].c_value.value;
			HRHighPollTab.tab[i].c_value.value = 0;
		}

		//IR
		for(int i=0;i<high_n.ir;i++){
			IRHighPollTab.tab[i].p_value.value = IRHighPollTab.tab[i].c_value.value;
			IRHighPollTab.tab[i].c_value.value = 0;
		}
		break;

	default:
		break;
	}

}



/**
 * @brief save_coil_di_value
 *        Save the value into the relative tab
 *
 * @param coil_di_low_high_t *arr
 *        void* instance_ptr
 * @return void
 */
static void save_coil_di_value(coil_di_low_high_t *arr, void* instance_ptr){
	uint16_t temp, read_val = 0;
	uint8_t bit=0;

	bit = arr->info.Addr % 16;
	read_val = *((uint16_t*)(instance_ptr));
	temp = read_val & (uint16_t)(1 << bit);

	temp == 0 ? (temp = 0) : (temp = 1);
	arr->c_value = temp;
}



/**
 * @brief save_hr_ir_value
 *        Save the value into the relative tab
 *
 * @param hr_ir_low_high_poll_t *arr, void* instance_ptr
 * @return void
 */
static void save_hr_ir_value(hr_ir_low_high_poll_t *arr, void* instance_ptr){
	if(arr->info.dim > 16){
	int32_t temp = 0;
		if(1 == arr->info.flag.bit.bigendian){
			temp = (*(int32_t*)(instance_ptr));
			arr->c_value.reg.high = (uint16_t)temp;
			arr->c_value.reg.low = 	(uint16_t)(temp >> 16);

		}else{
			arr->c_value.value = (*(int32_t*)(instance_ptr));
		}
	}else{

		arr->c_value.value =(uint32_t)(*(int16_t*)(instance_ptr));
	}
}


/**
 * @brief save_alarm_coil_di_value
 *        Save the value into the relative tab
 *
 * @param coil_di_alarm_tables_t *alarm
 *        void* instance_ptr
 * @return void
 */
static void save_alarm_coil_di_value(coil_di_alarm_tables_t *alarm,  void* instance_ptr)
{
	uint16_t temp, read_val = 0;
	uint8_t reg, bit=0;

	bit = alarm->info.Addr % 16;
	read_val = *((uint16_t*)(instance_ptr));
	temp = read_val & (uint16_t)(1 << bit);
	temp == 0 ? (temp = 0) : (temp = 1);

	if(temp != alarm->data.value){
		if(0 != temp){
			alarm->data.start_time = RTC_Get_UTC_Current_Time();
			alarm->data.stop_time = 0;
			alarm->data.value = 1;
			alarm->data.send_flag = 1;
		}else{
			alarm->data.stop_time = RTC_Get_UTC_Current_Time();
			alarm->data.value = 0;
			alarm->data.send_flag = 1;
		}
	}
}

/**
 * @brief save_alarm_hr_ir_value
 *        Save the value into the relative tab
 *
 * @param hr_ir_alarm_tables_t *alarm
 *        void* instance_ptr
 * @return void
 */
static void save_alarm_hr_ir_value(hr_ir_alarm_tables_t *alarm, void* instance_ptr)
{
	uint16_t temp, read_val = 0;
	uint8_t reg, bit=0;

	read_val = *((uint16_t*)(instance_ptr));
	temp = read_val & (uint16_t)(1 << alarm->info.dim);

	temp == 0 ? (temp = 0) : (temp = 1);

	if(temp != alarm->data.value){
		//temp is not shifted, so it can have the entire value in uint16_t
		if(0 != temp){
			alarm->data.start_time = RTC_Get_UTC_Current_Time();
			alarm->data.stop_time = 0;
			alarm->data.value = 1;
			alarm->data.send_flag = 1;
		}else{
			alarm->data.stop_time = RTC_Get_UTC_Current_Time();
			alarm->data.value = 0;
			alarm->data.send_flag = 1;
		}
	}
}

/* Description: send JSON msg via MQTT if any alarm's value is changed */

static void send_cbor_alarm(uint16_t alias, alarm_read_t *data, uint8_t alarm_issue){
	c_cboralarms cbor_al;
	cbor_al.st = data->start_time;
	cbor_al.et = data->stop_time;
	if (alarm_issue == 0){
		cbor_al.aty = 2;
		cbor_al.aco = 1;
		strcpy((char*)cbor_al.ali, "");
	}
	else {
		cbor_al.aty = 1;
		cbor_al.aco = 0;
		itoa(alias,(char*)cbor_al.ali,10);
	}
	MQTT_Alarms(cbor_al);// alias, data->start_time,  data->stop_time, alarm_issue);
};


static void set_null_alarm_timer(){
	NullTimerAlarm = NULL_ALARM_TIMER;
	PRINTF_DEBUG("set NullTimerAlarm = %d\n",NullTimerAlarm);
}

static void clear_null_alarm_timer(){
	NullTimerAlarm = 0;
}

static uint16_t get_null_alarm_timer(){
	return NullTimerAlarm;
}


/* Description: Check if any alarm's value is changed, activated or deactivated */

static void check_alarms_change(void)
{
	uint16_t i;
	for(i=0; i<alarm_n.coil; i++){
		if(0 != COILAlarmPollTab[i].data.error){
			//send_js_alarm(COILAlarmPollTab[i].info.Alias, &COILAlarmPollTab[i].data, ERROR);
			send_cbor_alarm(COILAlarmPollTab[i].info.Alias, &COILAlarmPollTab[i].data, ERROR);

			PRINTF_POLL_ENG(("Coil Alarm error num %d \n ",i))
			set_null_alarm_timer();
			COILAlarmPollTab[i].data.error = 0;
		}
		else if (1 == COILAlarmPollTab[i].data.send_flag){
			//send_js_alarm(COILAlarmPollTab[i].info.Alias, &COILAlarmPollTab[i].data, CHANGED);
			send_cbor_alarm(COILAlarmPollTab[i].info.Alias, &COILAlarmPollTab[i].data, CHANGED);

			if(COILAlarmPollTab[i].data.value == 1)
			   printf("Coil Alarm rise num %d \n ",i);
			else
			   printf("Coil Alarm fall num %d \n ",i);

			COILAlarmPollTab[i].data.send_flag = 0;
		};
	}

	for(i=0; i<alarm_n.di; i++){
		if(0 != DIAlarmPollTab[i].data.error){
			//send_js_alarm(DIAlarmPollTab[i].info.Alias, &DIAlarmPollTab[i].data, ERROR);
			send_cbor_alarm(DIAlarmPollTab[i].info.Alias, &DIAlarmPollTab[i].data, ERROR);

			printf("DI Alarm error num %d \n ",i);
			set_null_alarm_timer();
			DIAlarmPollTab[i].data.error = 0;
		}
		else if (1 == DIAlarmPollTab[i].data.send_flag){
			//send_js_alarm(DIAlarmPollTab[i].info.Alias, &DIAlarmPollTab[i].data, CHANGED);
			send_cbor_alarm(DIAlarmPollTab[i].info.Alias, &DIAlarmPollTab[i].data, CHANGED);

			PRINTF_POLL_ENG(("DI Alarm changed num %d \n ",i))
			DIAlarmPollTab[i].data.send_flag = 0;
		};
	}

	for(i=0; i<alarm_n.hr; i++){
		if(0 != HRAlarmPollTab[i].data.error){
			//send_js_alarm(HRAlarmPollTab[i].info.Alias,(alarm_read_t*) &HRAlarmPollTab[i].data, ERROR);
			send_cbor_alarm(HRAlarmPollTab[i].info.Alias,(alarm_read_t*) &HRAlarmPollTab[i].data, ERROR);

			PRINTF_POLL_ENG(("HR Alarm error num %d \n ",i))
			set_null_alarm_timer();
			HRAlarmPollTab[i].data.error = 0;
		}
		else if (1 == HRAlarmPollTab[i].data.send_flag){
	//		send_js_alarm(HRAlarmPollTab[i].info.Alias,(alarm_read_t*) &HRAlarmPollTab[i].data, CHANGED);
			send_cbor_alarm(HRAlarmPollTab[i].info.Alias,(alarm_read_t*) &HRAlarmPollTab[i].data, CHANGED);

			PRINTF_POLL_ENG(("HR Alarm changed num %d \n ",i))
			HRAlarmPollTab[i].data.send_flag = 0;
		};
	}

	for(i=0; i<alarm_n.ir; i++){
		if(0 != IRAlarmPollTab[i].data.error){
			//send_js_alarm(IRAlarmPollTab[i].info.Alias,(alarm_read_t*) &IRAlarmPollTab[i].data, ERROR);
			send_cbor_alarm(IRAlarmPollTab[i].info.Alias,(alarm_read_t*) &IRAlarmPollTab[i].data, ERROR);

	PRINTF_POLL_ENG(("IR Alarm error num %d \n ",i))
			set_null_alarm_timer();
			IRAlarmPollTab[i].data.error = 0;
		}
		else if (1 == IRAlarmPollTab[i].data.send_flag){
			//send_js_alarm(IRAlarmPollTab[i].info.Alias,(alarm_read_t*) &IRAlarmPollTab[i].data, CHANGED);
			send_cbor_alarm(IRAlarmPollTab[i].info.Alias,(alarm_read_t*) &IRAlarmPollTab[i].data, CHANGED);

			PRINTF_POLL_ENG(("IR Alarm changed num %d \n ",i))
			IRAlarmPollTab[i].data.send_flag = 0;
		};
	}

};

void print_ValuesTable(void){
	int i;
	if(PollEngine__GetPollEnginePrintMsgs() == 1){
	printf("Values Buffer\n");
	for(i = 0; i<values_buffer_index;	i++){
		printf("id: %4d,  alias: %4d,  value: %4Lf,  error: %d\n" ,
																values_buffer[i].index,
																values_buffer[i].alias,
																values_buffer[i].value,
																values_buffer[i].info_err);
	}
	printf("TIME Values Buffer\n");
		for(i = 0; i<values_buffer_read_section-1;	i++){
			printf("id: %4d,  t_start: %d,  t_stop: %d\n" ,
																	time_values_buff[i].index,
																	time_values_buff[i].t_start,
																	time_values_buff[i].t_stop);
		}
	}
}
























/****************************
* MODBUS POLLING
*****************************/

eMBErrorCode GetResult(void) 	 {  return 	retError; }
void SetResult(eMBErrorCode val) { retError = val;    }

/**
 * @brief DoLowPolling
 *        Check the low poll variable (Coil, Di, Hr, Ir)
 *
 * @param coil_di_poll_tables_t *Coil
 *        coil_di_poll_tables_t *Di
 *        hr_ir_poll_tables_t *Hr
 *	      hr_ir_poll_tables_t *Ir
 * @return none
 */
void DoLowPolling (coil_di_poll_tables_t *Coil, coil_di_poll_tables_t *Di, hr_ir_poll_tables_t *Hr, hr_ir_poll_tables_t *Ir)
{
	uint32_t value = 0;
	uint8_t addr = 0;
	uint8_t retry = 0;
    uint8_t numOf = 0;

    eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

	// Polling the Coil register
	for (uint16_t i = 0; i < low_n.coil; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Coil->reg[i].info.Addr);

		errorReq = app_coil_read(1, 1, addr, 1);
		Coil->reg[i].error = errorReq;
		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto coil address %d: %f, error: %d\n", addr, (float)param_buffer[0], Coil->reg[i].error);
#endif

		save_coil_di_value(&Coil->reg[i] , param_buffer);

		param_buffer[0] = param_buffer[1] = 0;

	}

	// Polling the Di register
	for (uint16_t i = 0; i < low_n.di; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Di->reg[i].info.Addr);

		errorReq = app_coil_discrete_input_read(1, 1, addr, 1);
		Di->reg[i].error = errorReq;
		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto di address %d: %f, error: %d\n", addr, (float)param_buffer[0], Di->reg[i].error);
#endif
		save_coil_di_value(&Di->reg[i] , param_buffer);

		param_buffer[0] = param_buffer[1] = 0;
	}


	// Polling the Hr register
	for (uint16_t i = 0; i < low_n.hr; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = Hr->tab[i].info.Addr;

		if((Hr->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;


		errorReq = app_holding_register_read(1, 1, addr, numOf);
		Hr->tab[i].error = errorReq;


		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto hr address %d: %f, error: %d\n", addr, (float)param_buffer[0], Hr->tab[i].error);
#endif
		save_hr_ir_value(&Hr->tab[i], param_buffer);   // &HRLowPollTab.tab[i]

		param_buffer[0] = param_buffer[1] = 0;

	}


	// POlling the Ir register
	for (uint16_t i = 0; i < low_n.ir; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = Ir->tab[i].info.Addr;

		if((Ir->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;

		errorReq = app_input_register_read(1, 1, addr, numOf);
		Ir->tab[i].error = errorReq;

		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto ir address %d: %f, error: %d\n", addr, (float)param_buffer[0], Ir->tab[i].error);
#endif
		save_hr_ir_value(&Ir->tab[i], param_buffer);

		param_buffer[0] = param_buffer[1] = 0;
	}

}

// CHIEBAO A.

/**
 * @brief DoHighPolling
 *        Check the high poll variable (Coil, Di, Hr, Ir)
 *
 * @param coil_di_poll_tables_t *Coil
 *        coil_di_poll_tables_t *Di
 *        hr_ir_poll_tables_t *Hr
 *	      hr_ir_poll_tables_t *Ir
 * @return none
 */
void DoHighPolling (coil_di_poll_tables_t *Coil, coil_di_poll_tables_t *Di, hr_ir_poll_tables_t *Hr, hr_ir_poll_tables_t *Ir)
{
	uint32_t value = 0;
	uint8_t addr = 0;
	uint8_t retry = 0;
    uint8_t numOf = 0;

    eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

	// Polling the Coil register
	for (uint16_t i = 0; i < high_n.coil; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Coil->reg[i].info.Addr);


		errorReq = app_coil_read(1, 1, addr, 1);


		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_coil_di_value(&Coil->reg[i] , param_buffer);

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Di register
	for (uint16_t i = 0; i < high_n.di; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Di->reg[i].info.Addr);

		errorReq = app_coil_discrete_input_read(1, 1, addr, 1);

		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_coil_di_value(&Di->reg[i] , param_buffer);

		param_buffer[0] = param_buffer[1] = 0;
	}


	// Polling the Hr register
	for (uint16_t i = 0; i < high_n.hr; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = Hr->tab[i].info.Addr;

		if((Hr->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;

		errorReq = app_holding_register_read(1, 1, addr, numOf);

		// reset to the default for the next reading
		SetResult(MB_ENOREG);

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_hr_ir_value(&Hr->tab[i], param_buffer);   // &HRLowPollTab.tab[i]
		param_buffer[0] = param_buffer[1] = 0;
	}


	// Polling the Ir register
	for (uint16_t i = 0; i < high_n.ir; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = Ir->tab[i].info.Addr;

		if((Ir->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;


	    errorReq = app_input_register_read(1, 1, addr, numOf);

		// reset to the default for the next reading
		SetResult(MB_ENOREG);

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_hr_ir_value(&Ir->tab[i], param_buffer);
		param_buffer[0] = param_buffer[1] = 0;

	}
}



// CHIEBAO A.

/**
 * @brief DoAlarmPolling
 *        Check the allarm poll variable (Coil, Di, Hr, Ir)
 *
 * @param coil_di_poll_tables_t *Coil
 *        coil_di_poll_tables_t *Di
 *        hr_ir_poll_tables_t *Hr
 *	      hr_ir_poll_tables_t *Ir
 * @return none
 */
void DoAlarmPolling(coil_di_alarm_tables_t *Coil, coil_di_alarm_tables_t *Di, hr_ir_alarm_tables_t *Hr, hr_ir_alarm_tables_t *Ir)
{
	uint32_t value = 0;
	uint8_t addr = 0;
	uint8_t retry = 0;
	uint8_t numOf = 0;

	eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;


	// Polling the Coil register
	for (uint16_t i = 0; i < alarm_n.coil; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Coil[i].info.Addr);

        errorReq = app_coil_read(1, 1, addr, 1);

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("errore  %d\n", errorReq);
#endif

		if(errorReq == 0)
		{
		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;


#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_alarm_coil_di_value(&Coil[i], param_buffer);
		}

		param_buffer[0] = param_buffer[1] = 0;
	}


	// Polling the Di register
	for (uint16_t i = 0; i < alarm_n.di; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Di[i].info.Addr);

		errorReq = app_coil_discrete_input_read(1, 1, addr, 1);

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("errore  %d\n", errorReq);
#endif

		if(errorReq == 0)
		{
		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_alarm_coil_di_value(&Di[i], param_buffer);
		}

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Hr register
	for (uint16_t i = 0; i < alarm_n.hr; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Hr[i].info.Addr);

		errorReq = app_holding_register_read(1, 1, addr, 1);

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("errore  %d\n", errorReq);
#endif

		if(errorReq == 0)
		{
		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_alarm_hr_ir_value(&Hr[i], param_buffer);
		}
		param_buffer[0] = param_buffer[1] = 0;
	}


	// Polling the Ir register
	for (uint16_t i = 0; i < alarm_n.ir; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Ir[i].info.Addr);


		errorReq = app_input_register_read(1, 1, addr, 1);

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("errore  %d\n", errorReq);
#endif


		if(errorReq == 0)
		{
		// reset to the default for the next reading
		SetResult(MB_ENOREG);
		errorReq = MB_MRE_NO_REG;

#ifdef __DEBUG_POLL_ENGINE_CAREL
		printf("letto %f\n", (float)param_buffer[0]);
#endif
		save_alarm_hr_ir_value(&Ir[i], param_buffer);
		}

		param_buffer[0] = param_buffer[1] = 0;
	}


}



//CHIEBAO A.

/**
 * @brief DoPolling
 *        function with the timing to apply for low check and high check
 *		  check also the allarm
 *
 * @param  req_set_gw_config_t* polling_times
 * @return none
 */
static uint32_t timeout = 0;
void DoPolling(req_set_gw_config_t* polling_times)
{

		if(RUNNING == PollEngine_Status.engine)
		{

			PollEngine_Status.polling = RUNNING;

			timeout = RTC_Get_UTC_Current_Time();


			if(timeout > (timestamp.current_alarm) && alarm_n.total > 0) {  // + ALARM_POLLING_TIME
			   //ALARM POLLING
				printf("ALLARMI \n");
				//clear_null_alarm_timer();

				DoAlarmPolling(COILAlarmPollTab, DIAlarmPollTab, HRAlarmPollTab, IRAlarmPollTab);

				//timestamp.previous_alarm = timestamp.current_alarm;
				timestamp.current_alarm = RTC_Get_UTC_Current_Time();

				//print_Alarmtables();
				//if(rete =! 0)
				check_alarms_change();    // CHIEBAO
				// else
				// metto in fifo
			}

			if ((timeout > (timestamp.current_low + polling_times->lowspeedsamplevalue)   &&   low_n.total > 0)) {
				//LOW POLLING
				printf("LOW \n");
				DoLowPolling(&COILLowPollTab, &DILowPollTab, &HRLowPollTab, &IRLowPollTab);

				//timestamp.previous_low = timestamp.current_low;
				timestamp.current_low = RTC_Get_UTC_Current_Time();

				compare_prev_curr_reads(LOW_POLLING);
				update_current_previous_tables(LOW_POLLING);

				MQTT_FlushValues();

			}
			else if (timeout > (timestamp.current_high + polling_times->hispeedsamplevalue)   &&   high_n.total > 0) {
				//HIGH POLLING
				printf("HIGH \n");
				DoHighPolling(&COILHighPollTab, &DIHighPollTab, &HRHighPollTab, &IRHighPollTab);

				//timestamp.previous_high = timestamp.current_high;
				timestamp.current_high = RTC_Get_UTC_Current_Time();

				//print_Hightables();
				compare_prev_curr_reads(HIGH_POLLING);
				update_current_previous_tables(HIGH_POLLING);
				//print_ValuesTable();
				MQTT_FlushValues();

		    }
		}

		//PollEngine_Status.polling = STOPPED;
}





// CHIEBAO A.

/**
 * @brief Polling_Engine_Init
 *        init function of the polling engine machine
 *
 * @param  none
 * @return none
 */
void Polling_Engine_Init(void)
{
	//Create Values Buffer
	create_values_buffers();


	req_set_gw_config_t polling_times;

	PollEngine_Status.engine = RUNNING;   //  at the beginning are INITIALIZED....when a mqtt message arrive start running


	// temporarily
	polling_times.lowspeedsamplevalue = T_LOW_POLL;
	polling_times.hispeedsamplevalue = T_HIGH_POLL;


	while(1)
	{
		DoPolling(&polling_times);
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
}


/**
 * @brief CarelEngineMB_Init
 *        task to run Polling_Engine_Init
 *        TO DO...depends on the operating system in use
 *
 * @param  none
 * @return none
 */
void CarelEngineMB_Init(void){
	// Starts operation task to check values and trigger an event


	xTaskCreate(&Polling_Engine_Init, "Poll_engine_init",SENSE_TRIGGER_TASK_STACK_SIZE, NULL, SENSE_TRIGGER_TASK_PRIO, &xPollingEngine);
}

void PollEngine__MBResume(void){
	// Starts operation task to check values and trigger an event
	PRINTF_DEBUG("PE RESUMED\n");
	vTaskResume(xPollingEngine);

}


void PollEngine__MBSuspend(void){
	//mbc_master_destroy();

	PRINTF_DEBUG("PE SUSPENDED, free heap : %d\n",esp_get_free_heap_size());
	vTaskSuspend(xPollingEngine);
}




C_RES PollEngine__Read_HR_IR_Req(char* alias, void* read_value){
	uint16_t cid;
	//char temp[6] = {0};
	uint16_t max_reg = low_n.total + high_n.total + alarm_n.total;
	//characteristic_descriptor_t cid_data = { 0 };

	for(cid=0; cid < max_reg; cid++){

		PRINTF_POLL_ENG(("MBParameters[%d].param_key = %s\n",cid,MBParameters[cid].param_key))
		if(strcmp(MBParameters[cid].param_key, alias) == 0){

			//ESP_ERROR_CHECK_WITHOUT_ABORT (sense_modbus_get_cid_data(cid, &cid_data));
//			sense_modbus_read_value(cid, read_value);    // CHIEBAO
			PRINTF_POLL_ENG(("value is = %0X\n\n",*((uint32_t*)read_value)))
			break;

		}else if(cid == (max_reg - 1)){
			return C_FAIL;
		}
	}
	return C_SUCCESS;
}

C_RES PollEngine__Read_COIL_DI_Req(char* alias, uint16_t* read_value){
	uint16_t cid;
	//char temp[6] = {0};
	uint16_t max_reg = low_n.total + high_n.total + alarm_n.total;
	//characteristic_descriptor_t cid_data = { 0 };

	for(cid=0; cid < max_reg; cid++){

		PRINTF_POLL_ENG(("MBParameters[%d].param_key = %s\n",cid,MBParameters[cid].param_key))
		if(strcmp(MBParameters[cid].param_key, alias) == 0){

			//ESP_ERROR_CHECK_WITHOUT_ABORT (sense_modbus_get_cid_data(cid, &cid_data));
//			sense_modbus_read_value(cid, (void*)read_value);       // CHIEBAO
			PRINTF_POLL_ENG(("value is = %d\n\n",*((uint16_t*)read_value)))
			break;

		}else if(cid == (max_reg - 1)){
			return C_FAIL;
		}
	}
	return C_SUCCESS;
}

C_RES PollEngine__Write_HR_Req(char* alias, void* write_value){
	uint16_t cid;
	//char temp[6] = {0};
	uint16_t max_reg = low_n.total + high_n.total + alarm_n.total;
	//characteristic_descriptor_t cid_data = { 0 };
	uint32_t read_value = 0;
	for(cid=0; cid < max_reg; cid++){

		//PRINTF_DEBUG("MBParameters[%d].param_key = %s\n",cid,MBParameters[cid].param_key);
		if(strcmp(MBParameters[cid].param_key, alias) == 0){

//			ESP_ERROR_CHECK_WITHOUT_ABORT (sense_modbus_get_cid_data(cid, &cid_data));          // CHIEBAO
//			sense_modbus_send_value(cid, write_value);											// CHIEBAO
			//PRINTF_DEBUG("Write value is = %0X\n\n",*((uint32_t*)write_value));
//			sense_modbus_read_value(cid, (void*)&read_value);									// CHIEBAO
			//PRINTF_DEBUG("Read value is = %0X\n\n",*((uint32_t*)&read_value));
			break;

		}else if(cid == (max_reg - 1)){
			return C_FAIL;
		}
	}
	return C_SUCCESS;
}


C_RES PollEngine__Write_COIL_Req(char* alias, uint16_t write_value, uint16_t addr){
	uint16_t cid;
	uint16_t reg_to_write = 0;
	uint16_t bit = 0;
	uint16_t max_reg = low_n.total + high_n.total + alarm_n.total;
//	characteristic_descriptor_t cid_data = { 0 };
	uint16_t read_value = 0;
	for(cid=0; cid < max_reg; cid++){

		PRINTF_POLL_ENG(("MBParameters[%d].param_key = %s\n",cid,MBParameters[cid].param_key))
		if(strcmp(MBParameters[cid].param_key, alias) == 0){

			//ESP_ERROR_CHECK_WITHOUT_ABORT (sense_modbus_get_cid_data(cid, &cid_data));

//			sense_modbus_read_value(cid, (void*)&read_value);      // CHIEBAO
			PRINTF_POLL_ENG(("Read value is = %04X\n\n",*((uint16_t*)&read_value)))

			bit = addr % 16;
			printf("read_coil bit = %d\n",bit);

			if(write_value  ==  1){
				reg_to_write = read_value | (uint16_t)(1 << bit);
				printf("val reg to write 1 : %X\n",reg_to_write);
			}else if (write_value  ==  0){
				reg_to_write = read_value & ~((uint16_t)(1 << bit));
				printf("val reg to write 0 : %X\n",reg_to_write);
			}

			printf("val reg to write : %X\n",reg_to_write);
//			sense_modbus_send_value(cid, (void*)&reg_to_write);     // CHIEBAO

			break;

		}else if(cid == (max_reg - 1)){
			return C_FAIL;
		}
	}
	return C_SUCCESS;
}


uint8_t PollEngine__SendMBAdu(c_cbor_send_mb_adu *send_mb_adu, uint8_t* data_rx){

//	mbc_master_suspend();   // CHIEBAO

	uint8_t data_rx_len;

	PRINTF_POLL_ENG(("ADU Request Packet:  "))

	for(int i=0; i<send_mb_adu->adu_len; i++)
	{
		uart_write_bytes(MB_PORTNUM, (const char *) &send_mb_adu->adu[i], 1);
	}

	data_rx_len = uart_read_bytes(MB_PORTNUM, data_rx, 255,  MB_RESPONSE_TIMEOUT(send_mb_adu->adu_len));


	if(PollEngine__GetPollEnginePrintMsgs() == 1){
		printf("\nuart_read_bytes len = %d\n\n",data_rx_len);

		for(int i=0; i<data_rx_len; i++){
			printf("[%d]=%02X  ",i,data_rx[i]);
		}
		printf("\n");
	}

	uart_flush_input(MB_PORTNUM);
	uart_flush(MB_PORTNUM);

//	ClearQueueMB();			// CHIEBAO
//	mbc_master_resume();	// CHIEBAO

	vTaskDelay(1000 / portTICK_PERIOD_MS);

	return data_rx_len;


}



static uint8_t test1=0;
static uint8_t test2=0;
static uint8_t test3=0;
static uint8_t test4=0;

void PollEngine__PassModeFSM(void){

	//PRINTF_DEBUG("PollEngine_Status.engine = %d, PassMode_FSM = %d\n", PollEngine_Status.engine,PassMode_FSM );
	if(STOPPED == PollEngine_Status.engine){// && STOPPED == PollEngine_Status.polling

		switch(PassMode_FSM){
		case START_TIMER:
			{

				PassModeTimer =	RTC_Get_UTC_Current_Time();
				PassMode_FSM = WAIT_MQTT_CMD;

				PRINTF_DEBUG("\nPassModeFSM 	START_TIMER\n");
			}
			break;

		case WAIT_MQTT_CMD:
			{
				if(RECEIVED == PollEngine__GetPassModeCMD()){
					PassMode_FSM = RESET_TIMER;
				}else if(RTC_Get_UTC_Current_Time() > (PassModeTimer + PASS_MODE_TIMER)){
						PassMode_FSM = DEACTIVATE_PASS_MODE;
				}

				if(test1==0){PRINTF_DEBUG("\nPassModeFSM 	WAIT_MQTT_CMD\n"); test1=1;}
			}
			break;

		case RESET_TIMER:
			{
				PassModeTimer =	RTC_Get_UTC_Current_Time();
				PassMode_FSM = EXECUTE_CMD;

				if(test2==0){PRINTF_DEBUG("\nPassModeFSM	RESET_TIMER\n"); test2=1;}
			}
			break;

		case EXECUTE_CMD:
			{
				if(EXECUTED == PollEngine__GetPassModeCMD() ||
					RTC_Get_UTC_Current_Time() > (PassModeTimer + PASS_MODE_TIMER)){

					PassMode_FSM = DEACTIVATE_PASS_MODE;
				}

				if(test3==0){PRINTF_DEBUG("\nPassModeFSM 	EXECUTE_CMD\n"); test3=3;}
			}
			break;

		case DEACTIVATE_PASS_MODE:
			{
				PassMode_FSM = START_TIMER;
				PollEngine_Status.passing_mode = DEACTIVATED;
				PollEngine_Status.engine = RUNNING;
				PollEngine__SetPassModeCMD(NOT_RECEIVED);


				if(test4==0){PRINTF_DEBUG("PassModeFSM 	DEACTIVATE_PASS_MODE\n"); test4=3;}
			}
			break;
		default:
			break;
		}

	}
}


void PollEngine__StartEngine(void){
	PollEngine_Status.engine = RUNNING;
}

void PollEngine__StopEngine(void){
	PollEngine_Status.engine = STOPPED;
}

uint8_t PollEngine__GetEngineStatus(void){
	return PollEngine_Status.engine;
}

uint8_t PollEngine__GetPollingStatus(void){
	return PollEngine_Status.polling;
}

void PollEngine__ActivatePassMode(void){
	PollEngine_Status.passing_mode = ACTIVETED;
}

void PollEngine__DeactivatePassMode(void){
	PollEngine_Status.passing_mode = DEACTIVATED;
}

uint8_t PollEngine__GetPassModeStatus(void){
	return PollEngine_Status.passing_mode;
}


void PollEngine__SetPassModeCMD(uint8_t status){
	PassMode_CmdStatus = status;
}

uint8_t PollEngine__GetPassModeCMD(void){
	return PassMode_CmdStatus;
}


values_buffer_t* PollEngine__GetValuesBuffer(void){
	return values_buffer;
}

values_buffer_timing_t* PollEngine__GetTimeBuffer(void){
	return time_values_buff;
}

uint16_t PollEngine__GetValuesBufferIndex(void){
	return values_buffer_index;
}

uint16_t PollEngine__GetTimerBufferIndex(void){
	return values_buffer_read_section;
}

void PollEngine__ResetValuesBuffer(void){
	//Reset Values Buffer
	//memset((void*)values_buffer, 0, sizeof(values_buffer));
	memset((void*)values_buffer, 0, values_buffer_len * sizeof(values_buffer_t));
	values_buffer_index = 0;
	//Reset Time Buffer
	//memset((void*)time_values_buff, 0, sizeof(time_values_buff));
	memset((void*)time_values_buff, 0, time_values_buff_len * sizeof(values_buffer_timing_t));
	values_buffer_read_section = 1;
}

uint32_t PollEngine__GetMBBaudrate(void){
	assert(MB_BaudRate > 0);
	return MB_BaudRate;
}


uint8_t PollEngine__GetPollEnginePrintMsgs(void){
	return PollEnginePrint;
}


void PollEngine__SetPollEnginePrintMsgs(uint8_t status){
	PollEnginePrint = status;
}
