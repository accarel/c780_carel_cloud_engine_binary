/**
 * @file   Polling_CAREL.c
 * @author carel
 * @date 9 Sep 2019
 * @brief  functions implementations specific related to the managment of the
 *         polling engine
 *         
 *  Description: This file contains the high level functions of polling engine :
 *  	1) Polling routine
 *  	2) Conversion functions
 *  	3) Buffering routine
 *  	4) The passing mode state machine
 *  	5) Polling tables creation
 *
 */
 

#include "string.h"
#include "MQTT_Interface_CAREL.h"
#include "data_types_CAREL.h"


//#include ""binary_model_CAREL.h  // TODO
#include "binary_model.h" 

#include "RTC_IS.h"
#include "modbus_IS.h"

#include "polling_IS.h"
#include "polling_CAREL.h"
#include "nvm.h"


#define RET_DIM(x,l)     (x == 16 ? (l = 1) : (l = 2))


#define OPTS(min_val, max_val, step_val) { .opt1 = min_val, .opt2 = max_val, .opt3 = step_val }
#define INIT_DELAY_TICS                 (100 / portTICK_RATE_MS)
#define TIMEOUT_UPDATE_CIDS_MS          (1000) 
#define TIMEOUT_UPDATE_CIDS_TICS        (TIMEOUT_UPDATE_CIDS_MS / portTICK_RATE_MS)


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
//static uint8_t test = 0;

//Engine flags
static passing_mode_fsm_t PassMode_FSM = START_TIMER;
static uint32_t PassModeTimer = 0;
static uint8_t PassMode_CmdStatus = NOT_RECEIVED;
static uint32_t MB_BaudRate = 0;
static uint16_t NullTimerAlarm = 0;

// usefull for a MODBUS READING AND QWRITING

USHORT param_buffer[2];				// max 32 bits
eMBErrorCode retError = MB_ENOREG;

static uint8_t PollEnginePrint = POLL_ENGINE_PRINTF_DEFAULT;
static uint8_t first_high = 1;
static uint8_t first_low = 1;

/*Static Function*/

static void check_increment_values_buff_len(uint16_t *values_buffer_idx);
static void check_hr_ir_read_val(hr_ir_poll_tables_t *arr, uint8_t arr_len, uint8_t first);
static void check_coil_di_read_val(coil_di_poll_tables_t *arr, uint8_t arr_len, uint8_t first);
static void compare_prev_curr_reads(PollType_t poll_type, uint8_t first);
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
	uint8_t temp=0;

	temp = DeviceParamCount[LOW_POLLING][COIL];
	if(0 != temp){
		COILLowPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&COILLowPollTab.reg[0] , 0, temp*sizeof(coil_di_low_high_t));
		uint8_t  *p_coil_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILLowPollTab.reg[i].info =  *((r_coil_di*)(p_coil_low_sect + (i * sizeof(r_coil_di))));
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][COIL];
	if(0 != temp){
		COILHighPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&COILHighPollTab.reg[0] , 0, temp*sizeof(coil_di_low_high_t));
		uint8_t  *p_coil_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILHighPollTab.reg[i].info =  *((r_coil_di*)(p_coil_high_sect + (i * sizeof(r_coil_di))));
		}

	}


	temp = DeviceParamCount[ALARM_POLLING][COIL];
	if(0 != temp){
		COILAlarmPollTab = malloc(temp * sizeof(coil_di_alarm_tables_t));
		memset((void*)&COILAlarmPollTab[0] , 0, temp*sizeof(coil_di_alarm_tables_t));
		uint8_t  *p_coil_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILAlarmPollTab[i].info =  *((r_coil_di_alarm*)(p_coil_alarm_sect + (i * sizeof(r_coil_di_alarm))));
		}
	}

	//Descrete Input

	temp = DeviceParamCount[LOW_POLLING][DI];
	if(0 != temp){
		DILowPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&DILowPollTab.reg[0] , 0, temp*sizeof(coil_di_low_high_t));
		uint8_t  *p_di_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, DI);
		for(int i=0;i<temp;i++){
			DILowPollTab.reg[i].info =  *((r_coil_di*)(p_di_low_sect + (i * sizeof(r_coil_di))));
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][DI];
	if(0 != temp){
		DIHighPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&DIHighPollTab.reg[0] , 0, temp*sizeof(coil_di_low_high_t));
		uint8_t  *p_di_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, DI);
		for(int i=0;i<temp;i++){
			DIHighPollTab.reg[i].info =  *((r_coil_di*)(p_di_high_sect + (i * sizeof(r_coil_di))));
		}
	}


	temp = DeviceParamCount[ALARM_POLLING][DI];
	if(0 != temp){
		DIAlarmPollTab = malloc(temp * sizeof(coil_di_alarm_tables_t));
		memset((void*)&DIAlarmPollTab[0] , 0, temp*sizeof(coil_di_alarm_tables_t));
		uint8_t  *p_di_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, DI);
		for(int i=0;i<temp;i++){
			DIAlarmPollTab[i].info =  *((r_coil_di_alarm*)(p_di_alarm_sect + (i * sizeof(r_coil_di_alarm))));
		}
	}

	//Holding Register

	temp = DeviceParamCount[LOW_POLLING][HR];
	if(0 != temp){
		HRLowPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&HRLowPollTab.tab[0] , 0, temp*sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_hr_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRLowPollTab.tab[i].info =  *((r_hr_ir*)(p_hr_low_sect + (i * sizeof(r_hr_ir))));
			HRLowPollTab.tab[i].read_type = check_hr_ir_reg_type(HRLowPollTab.tab[i].info);
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][HR];
	if(0 != temp){
		HRHighPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&HRHighPollTab.tab[0] , 0, temp*sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_hr_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRHighPollTab.tab[i].info =  *((r_hr_ir*)(p_hr_high_sect + (i * sizeof(r_hr_ir))));
			HRHighPollTab.tab[i].read_type = check_hr_ir_reg_type(HRHighPollTab.tab[i].info);
		}
	}


	temp = DeviceParamCount[ALARM_POLLING][HR];
	if(0 != temp){
		HRAlarmPollTab = malloc(temp * sizeof(hr_ir_alarm_tables_t));
		memset((void*)&HRAlarmPollTab[0] , 0, temp*sizeof(hr_ir_alarm_tables_t));
		uint8_t  *p_hr_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRAlarmPollTab[i].info =  *((r_hr_ir_alarm*)(p_hr_alarm_sect + (i * sizeof(r_hr_ir_alarm))));
		}
	}

	//Input Register


	temp = DeviceParamCount[LOW_POLLING][IR];
	if(0 != temp){
		IRLowPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&IRLowPollTab.tab[0] , 0, temp*sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_ir_low_sect = BinaryModel__GetPtrSec(LOW_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRLowPollTab.tab[i].info =  *((r_hr_ir*)(p_ir_low_sect + (i * sizeof(r_hr_ir))));
			IRLowPollTab.tab[i].read_type = check_hr_ir_reg_type(IRLowPollTab.tab[i].info);
		}
	}


	temp = DeviceParamCount[HIGH_POLLING][IR];
	if(0 != temp){
		IRHighPollTab.tab = malloc(temp * sizeof(hr_ir_low_high_poll_t));
		memset((void*)&IRHighPollTab.tab[0] , 0, temp*sizeof(hr_ir_low_high_poll_t));
		uint8_t  *p_ir_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRHighPollTab.tab[i].info =  *((r_hr_ir*)(p_ir_high_sect + (i * sizeof(r_hr_ir))));
			IRHighPollTab.tab[i].read_type = check_hr_ir_reg_type(IRHighPollTab.tab[i].info);
		}
	}


	temp = DeviceParamCount[ALARM_POLLING][IR];
	if(0 != temp){
		IRAlarmPollTab = malloc(temp * sizeof(hr_ir_alarm_tables_t));
		memset((void*)&IRAlarmPollTab[0] , 0, temp*sizeof(hr_ir_alarm_tables_t));
		uint8_t  *p_ir_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRAlarmPollTab[i].info =  *((r_hr_ir_alarm*)(p_ir_alarm_sect + (i * sizeof(r_hr_ir_alarm))));
		}
	}

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
	
	// total variable
	cid_counter = low_n.total + high_n.total + alarm_n.total;
}


void create_values_buffers(void){
	//Allocate Time buffer for values buffer
	time_values_buff_len = (uint16_t)(ceil(TSEND / T_LOW_POLL) + ceil(TSEND / T_HIGH_POLL));

	time_values_buff = malloc(time_values_buff_len * sizeof(values_buffer_timing_t));           // malloc
	memset((void*)time_values_buff, 0, time_values_buff_len * sizeof(values_buffer_timing_t));
	//Allocate values buffer
	uint32_t  freespace = uxTaskGetStackHighWaterMark(NULL);


	freespace -= 1000;
	values_buffer_len = (uint16_t)(freespace/((uint32_t)sizeof(values_buffer_t)));

	values_buffer = malloc(values_buffer_len * sizeof(values_buffer_t));						// malloc
	memset((void*)values_buffer, 0, values_buffer_len * sizeof(values_buffer_t));

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
static void check_hr_ir_read_val(hr_ir_poll_tables_t *arr, uint8_t arr_len, uint8_t first_run)
{
	bool to_values_buff = false;
	long double value = 0;
	for(uint8_t i=0; i<arr_len; i++){
		if(0 != arr->tab[i].error && arr->tab[i].error != arr->tab[i].p_error){
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
				if(temp > arr->tab[i].info.Hyster){
					to_values_buff = true;

					arr->tab[i].p_value = arr->tab[i].c_value;

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

					arr->tab[i].p_value = arr->tab->c_value;

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

					arr->tab[i].p_value = arr->tab[i].c_value;

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

					arr->tab[i].p_value = arr->tab[i].c_value;

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

					arr->tab[i].p_value = arr->tab[i].c_value;

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

					arr->tab[i].p_value = arr->tab[i].c_value;

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

					arr->tab[i].p_value = arr->tab[i].c_value;

					value = (long double)c_read;
				}
			}
				break;

			default:
				break;
			}
			if(value != 0 || (first_run)){
				values_buffer[values_buffer_index].alias = arr->tab[i].info.Alias;
				values_buffer[values_buffer_index].value = value;
				values_buffer[values_buffer_index].info_err = 0;
				values_buffer[values_buffer_index].data_type = arr->tab[i].info.dim;
				check_increment_values_buff_len(&values_buffer_index);
			}
		}
	}
}


/*	Descriptions: Comparing the current read with previous COIL and IR reads
 * 					accoring to its type (Look carel registers type).
 *					If there is a diff >= hysteresis, writes the read value + reg info
 *					in values buffer, then increment the values buffer index
 *
 *					arr: is the COIL or DI table
 *					arr_len: the table length
 */
static void check_coil_di_read_val(coil_di_poll_tables_t *arr, uint8_t arr_len, uint8_t first_run)
{
	for(uint8_t i=0; i<arr_len; i++){
		//error?
		if(0 != arr->reg[i].error && arr->reg[i].error != arr->reg[i].p_error){
			//send values to values buffer as error
			values_buffer[values_buffer_index].alias = arr->reg[i].info.Alias;
			values_buffer[values_buffer_index].value = 0;
			values_buffer[values_buffer_index].info_err = arr->reg[i].error;
			check_increment_values_buff_len(&values_buffer_index);

		}
		//value changed
		else if(arr->reg[i].c_value != arr->reg[i].p_value || (first_run)){
			//send values to values buffer
			values_buffer[values_buffer_index].alias = arr->reg[i].info.Alias;
			values_buffer[values_buffer_index].value = (long double)arr->reg[i].c_value;
			values_buffer[values_buffer_index].info_err = 0;
			check_increment_values_buff_len(&values_buffer_index);
		}
	}
}


/*	Descriptions: Routine that manages the hole comparison phase of CURRENT / PREVIOUS reads
 * 					according to polling type if HIGH or LOW
 * 					Then updates the values and time buffers
 * 					Should be called directly after finishing the polling routine
 */

static void compare_prev_curr_reads(PollType_t poll_type, uint8_t first)
{
	//get current index of values buffer
	uint16_t index_temp =  values_buffer_index;
	PRINTF_DEBUG("START index_temp = %d, values_buffer_index = %d\n",index_temp,values_buffer_index);

	switch(poll_type){
	case LOW_POLLING:
		check_coil_di_read_val(&COILLowPollTab, low_n.coil, first);
		check_coil_di_read_val(&DILowPollTab, low_n.di, first);
		check_hr_ir_read_val(&HRLowPollTab,  low_n.hr, first);
		check_hr_ir_read_val(&IRLowPollTab,  low_n.ir, first);

		break;

	case HIGH_POLLING:
		check_coil_di_read_val(&COILHighPollTab, high_n.coil, first);
		check_coil_di_read_val(&DIHighPollTab, high_n.di, first);
		check_hr_ir_read_val(&HRHighPollTab,  high_n.hr, first);
		check_hr_ir_read_val(&IRHighPollTab,  high_n.ir, first);
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
			COILLowPollTab.reg[i].p_error = COILLowPollTab.reg[i].error;
		}
		//DI
		for(i=0;i<low_n.di;i++){
			DILowPollTab.reg[i].p_value = DILowPollTab.reg[i].c_value;
			DILowPollTab.reg[i].c_value = 0;
			DILowPollTab.reg[i].p_error = DILowPollTab.reg[i].error;
		}
		//HR
		for(int i=0;i<low_n.hr;i++){
			//HRLowPollTab.tab[i].p_value.value = HRLowPollTab.tab[i].c_value.value;
			HRLowPollTab.tab[i].c_value.value = 0;
			HRLowPollTab.tab[i].p_error = HRLowPollTab.tab[i].error;

		}
		//IR
		for(int i=0;i<high_n.ir;i++){
			//IRLowPollTab.tab[i].p_value.value = IRLowPollTab.tab[i].c_value.value;
			IRLowPollTab.tab[i].c_value.value = 0;
			IRLowPollTab.tab[i].p_error = IRLowPollTab.tab[i].error;
		}
		break;

	case HIGH_POLLING:
		//Coil
		for(i=0;i<high_n.coil;i++){
			COILHighPollTab.reg[i].p_value = COILHighPollTab.reg[i].c_value;
			COILHighPollTab.reg[i].c_value = 0;
			COILHighPollTab.reg[i].p_error = COILHighPollTab.reg[i].error;
		}
		//DI
		for(i=0;i<high_n.di;i++){
			DIHighPollTab.reg[i].p_value = DIHighPollTab.reg[i].c_value;
			DIHighPollTab.reg[i].c_value = 0;
			DIHighPollTab.reg[i].p_error = DIHighPollTab.reg[i].error;
		}
		//HR
		for(int i=0;i<high_n.hr;i++){
			//HRHighPollTab.tab[i].p_value.value = HRHighPollTab.tab[i].c_value.value;
			HRHighPollTab.tab[i].c_value.value = 0;
			HRHighPollTab.tab[i].p_error = HRHighPollTab.tab[i].error;
		}

		//IR
		for(int i=0;i<high_n.ir;i++){
			//IRHighPollTab.tab[i].p_value.value = IRHighPollTab.tab[i].c_value.value;
			IRHighPollTab.tab[i].c_value.value = 0;
			IRHighPollTab.tab[i].p_error = IRHighPollTab.tab[i].error;
		}
		break;

	default:
		break;
	}

}

static void SetAllErrors(eMBMasterReqErrCode error){
	int i=0;

	//Coil
	for(i=0;i<low_n.coil;i++)
		COILLowPollTab.reg[i].error = error;
	//DI
	for(i=0;i<low_n.di;i++)
		DILowPollTab.reg[i].error = error;
	//HR
	for(int i=0;i<low_n.hr;i++)
		HRLowPollTab.tab[i].error = error;
	//IR
	for(int i=0;i<high_n.ir;i++)
		IRLowPollTab.tab[i].error = error;
	//Coil
	for(i=0;i<high_n.coil;i++)
		COILHighPollTab.reg[i].error = error;
	//DI
	for(i=0;i<high_n.di;i++)
		DIHighPollTab.reg[i].error = error;
	//HR
	for(int i=0;i<high_n.hr;i++)
		HRHighPollTab.tab[i].error = error;
	//IR
	for(int i=0;i<high_n.ir;i++)
		IRHighPollTab.tab[i].error = error;
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
	uint8_t  bit=0;

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

static void send_cbor_alarm(uint16_t alias, alarm_read_t *data){
	c_cboralarms cbor_al;
	cbor_al.st = data->start_time;
	cbor_al.et = data->stop_time;
	cbor_al.aty = 1;
	cbor_al.aco = 0;
	itoa(alias,(char*)cbor_al.ali,10);
	MQTT_Alarms(cbor_al);
}


static void send_cbor_offalarm(uint16_t alias, uint32_t st, uint32_t et){
	c_cboralarms cbor_al;

	cbor_al.st = st;
	cbor_al.et = et;
	cbor_al.aty = 2;
	cbor_al.aco = 1;
	strcpy((char*)cbor_al.ali, "");

	MQTT_Alarms(cbor_al);
}

/* Description: Check if any alarm's value is changed, activated or deactivated */

static void check_alarms_change(void)
{
	uint16_t i;

	for(i=0; i<alarm_n.coil; i++){
		if (1 == COILAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(COILAlarmPollTab[i].info.Alias, &COILAlarmPollTab[i].data);

			if(COILAlarmPollTab[i].data.value == 1)
			   printf("Coil Alarm rise num %d \n ",i);
			else
			   printf("Coil Alarm fall num %d \n ",i);

			COILAlarmPollTab[i].data.send_flag = 0;
		}
	}

	for(i=0; i<alarm_n.di; i++){
		if (1 == DIAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(DIAlarmPollTab[i].info.Alias, &DIAlarmPollTab[i].data);

			PRINTF_POLL_ENG(("DI Alarm changed num %d \n ",i))
			DIAlarmPollTab[i].data.send_flag = 0;
		}
	}

	for(i=0; i<alarm_n.hr; i++){
		if (1 == HRAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(HRAlarmPollTab[i].info.Alias,(alarm_read_t*) &HRAlarmPollTab[i].data);

			PRINTF_POLL_ENG(("HR Alarm changed num %d \n ",i))
			HRAlarmPollTab[i].data.send_flag = 0;
		}
	}

	for(i=0; i<alarm_n.ir; i++){
		if (1 == IRAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(IRAlarmPollTab[i].info.Alias,(alarm_read_t*) &IRAlarmPollTab[i].data);

			PRINTF_POLL_ENG(("IR Alarm changed num %d \n ",i))
			IRAlarmPollTab[i].data.send_flag = 0;
		}
	}
}

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


static C_RES DoPolling (coil_di_poll_tables_t *Coil, coil_di_poll_tables_t *Di, hr_ir_poll_tables_t *Hr, hr_ir_poll_tables_t *Ir, PollType_t type)
{
	uint8_t addr = 0;
    uint8_t numOf = 0;
    uint8_t retry = 0;
    uint8_t is_offline = 0;
    eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;
    uint8_t ncoil, ndi, nhr, nir;
    if (type == 0) {
    	ncoil = low_n.coil;
    	ndi = low_n.di;
    	nhr = low_n.hr;
    	nir = low_n.di;
    }
    else {
    	ncoil = high_n.coil;
    	ndi = high_n.di;
    	nhr = high_n.hr;
    	nir = high_n.di;
    }
	// Polling the Coil register
	for (uint16_t i = 0; i < ncoil; i++)
	{
		errorReq = MB_MRE_NO_REG;
		addr = (Coil->reg[i].info.Addr);

		do {
			errorReq = app_coil_read(1, 1, addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);

		Coil->reg[i].error = errorReq;
		if(errorReq == 0) {
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;
			save_coil_di_value(&Coil->reg[i] , param_buffer);
		}
		else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL; //this is the start of offline

		param_buffer[0] = param_buffer[1] = 0;

	}

	// Polling the Di register
	for (uint16_t i = 0; i < ndi; i++)
	{
		errorReq = MB_MRE_NO_REG;
		retry = 0;
		addr = (Di->reg[i].info.Addr);

		do {
			errorReq = app_coil_discrete_input_read(1, 1, addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);

		Di->reg[i].error = errorReq;
		if(errorReq == 0) {
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;
			save_coil_di_value(&Di->reg[i] , param_buffer);
		}
		else
			is_offline++;
		if(is_offline == 2)
			return C_FAIL;
		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Hr register
	for (uint16_t i = 0; i < nhr; i++)
	{
		errorReq = MB_MRE_NO_REG;
		retry = 0;
		addr = Hr->tab[i].info.Addr;

		if((Hr->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;

		do {
			errorReq = app_holding_register_read(1, 1, addr, numOf);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);

		Hr->tab[i].error = errorReq;
		if(errorReq == 0) {
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;
			save_hr_ir_value(&Hr->tab[i], param_buffer);   // &HRLowPollTab.tab[i]
		}
		else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL;
		param_buffer[0] = param_buffer[1] = 0;
	}

	// POlling the Ir register
	for (uint16_t i = 0; i < nir; i++)
	{
		errorReq = MB_MRE_NO_REG;
		retry = 0;
		addr = Ir->tab[i].info.Addr;

		if((Ir->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;

		do {
			errorReq = app_input_register_read(1, 1, addr, numOf);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);

		Ir->tab[i].error = errorReq;
		if(errorReq == 0) {
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;
			save_hr_ir_value(&Ir->tab[i], param_buffer);
		}
		else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL;
		param_buffer[0] = param_buffer[1] = 0;
	}
	return C_SUCCESS;
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


static C_RES DoAlarmPolling(coil_di_alarm_tables_t *Coil, coil_di_alarm_tables_t *Di, hr_ir_alarm_tables_t *Hr, hr_ir_alarm_tables_t *Ir)
{
	uint8_t addr = 0;
	uint8_t retry = 0;
	uint8_t is_offline = 0;
	eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

	// Polling the Coil register
	for (uint16_t i = 0; i < alarm_n.coil; i++)
	{
		errorReq = MB_MRE_NO_REG;

		addr = (Coil[i].info.Addr);
		do {
			errorReq = app_coil_read(1, 1, addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);

        Coil->data.error = errorReq;
		if(errorReq == 0)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;
			is_offline = 0;
			save_alarm_coil_di_value(&Coil[i], param_buffer);
		}
		else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL; //this is an offline

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Di register
	for (uint16_t i = 0; i < alarm_n.di; i++)
	{
		errorReq = MB_MRE_NO_REG;
		retry = 0;
		addr = (Di[i].info.Addr);

		do {
			errorReq = app_coil_discrete_input_read(1, 1, addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);

		Di->data.error = errorReq;

		if(errorReq == 0)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;

			save_alarm_coil_di_value(&Di[i], param_buffer);
		}
		else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL; //this is an offline

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Hr register
	for (uint16_t i = 0; i < alarm_n.hr; i++)
	{
		errorReq = MB_MRE_NO_REG;
		retry = 0;
		addr = (Hr[i].info.Addr);

		do {
			errorReq = app_holding_register_read(1, 1, addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);
		Hr->data.error = errorReq;
		if(errorReq == 0)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;

			save_alarm_hr_ir_value(&Hr[i], param_buffer);
		}else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL; //this is an offline

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Ir register
	for (uint16_t i = 0; i < alarm_n.ir; i++)
	{
		errorReq = MB_MRE_NO_REG;
		retry = 0;
		addr = (Ir[i].info.Addr);

		do {
			errorReq = app_input_register_read(1, 1, addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_REG && retry < 3);
		Ir->data.error = errorReq;
		if(errorReq == 0)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOREG);
			errorReq = MB_MRE_NO_REG;

			save_alarm_hr_ir_value(&Ir[i], param_buffer);
		}else
			is_offline++;

		if(is_offline == 2)
			return C_FAIL; //this is an offline

		param_buffer[0] = param_buffer[1] = 0;
	}
	return C_SUCCESS;

}

void ForceSending(void)
{
	first_low = 1;
	first_high = 1;
}

void ResetForced(PollType_t type)
{
	if (type == LOW_POLLING)
		first_low = 0;
	else if (type == HIGH_POLLING)
		first_high = 0;
}

uint8_t IsForced(PollType_t type)
{
	if (type == LOW_POLLING)
		return first_low;
	else if (type == HIGH_POLLING)
		return first_high;
	return 0;
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
static uint32_t start_offline = 0;
static uint32_t end_offline = 0;

void SendOffline(C_RES poll_done) {
	if (poll_done == C_FAIL) {
		if (start_offline == 0) {
			start_offline = RTC_Get_UTC_Current_Time();
			send_cbor_offalarm("", start_offline, 0);
			SetAllErrors(MB_MRE_TIMEDOUT);
			timestamp.current_high = RTC_Get_UTC_Current_Time();
			timestamp.current_low = RTC_Get_UTC_Current_Time();
			ForceSending();
			FlushValues(HIGH_POLLING);
			FlushValues(LOW_POLLING);

			//  TESTATA CHIEBAO
			vMBMasterRunResRelease();
		}
	}else{
		if (start_offline != 0) {
			end_offline = RTC_Get_UTC_Current_Time();
			send_cbor_offalarm("", start_offline, end_offline);
			start_offline = end_offline = 0;
			ForceSending();
		}
	}
}

bool IsOffline(void) {
	if (start_offline != 0 && end_offline == 0)
		return true;
	else return false;
}

void DoPolling_CAREL(req_set_gw_config_t * polling_times)
{
	C_RES poll_done = C_FAIL;
		if(RUNNING == PollEngine_Status.engine)
		{

			PollEngine_Status.polling = RUNNING;

			timeout = RTC_Get_UTC_Current_Time();


			if(timeout > (timestamp.current_alarm) && alarm_n.total > 0) {  // + ALARM_POLLING_TIME
			   //ALARM POLLING
				printf("ALLARMI \n");
				timestamp.current_alarm = RTC_Get_UTC_Current_Time();

				poll_done = DoAlarmPolling(COILAlarmPollTab, DIAlarmPollTab, HRAlarmPollTab, IRAlarmPollTab);
				SendOffline(poll_done);

				check_alarms_change();
			}

			if ((timeout > (timestamp.current_high + polling_times->hispeedsamplevalue)   &&   high_n.total > 0) || IsForced(HIGH_POLLING)) {
				//HIGH POLLING
				printf("HIGH \n");
				timestamp.current_high = RTC_Get_UTC_Current_Time();
				poll_done = DoPolling(&COILHighPollTab, &DIHighPollTab, &HRHighPollTab, &IRHighPollTab, HIGH_POLLING);
				SendOffline(poll_done);

				FlushValues(HIGH_POLLING);

			}
			if ((timeout > (timestamp.current_low + polling_times->lowspeedsamplevalue)   &&   low_n.total > 0) || IsForced(LOW_POLLING)) {
				//LOW POLLING
				printf("LOW \n");
				timestamp.current_low = RTC_Get_UTC_Current_Time();
				poll_done = DoPolling(&COILLowPollTab, &DILowPollTab, &HRLowPollTab, &IRLowPollTab, LOW_POLLING);
				SendOffline(poll_done);

				FlushValues(LOW_POLLING);
			}
		}
		PollEngine_Status.polling = STOPPED;
}

void FlushValues(PollType_t type){
	compare_prev_curr_reads(type, IsForced(type));
	ResetForced(type);
	update_current_previous_tables(type);
	MQTT_FlushValues();
}
// CHIEBAO A.

C_RES PollEngine__Read_HR_IR_Req(C_UINT16 func, C_UINT16 addr, C_BYTE dim, C_UINT16* read_value)
{
	eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

	C_CHAR len = 0;

	RET_DIM(dim,len);

	// TODO read from nvm the device address!!!!
	if(func == mbR_HR)
		errorReq = app_holding_register_read(1, NULL, addr, len);
	else // mbR_IR
		errorReq = app_input_register_read(1, NULL, addr, len);

	if(errorReq == MB_MRE_NO_ERR)
	{
		*read_value 	  = param_buffer[0];
		*(read_value + 1) = param_buffer[1];

		return C_SUCCESS;
	}
	else
	  	return C_FAIL;
}




C_RES PollEngine__Read_COIL_DI_Req(C_UINT16 func, C_UINT16 addr, C_UINT16* read_value){

	eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

	// TODO read from nvm the device address!!!!
	if(func == mbR_COIL)
	   errorReq = app_coil_read(1, 1, addr, 1);
	else // mbR_DI
	   errorReq = app_coil_discrete_input_read(1, 1, addr, 1);

	if(errorReq == MB_MRE_NO_ERR)
	{
		*read_value = param_buffer[0];
		return C_SUCCESS;
	}
	else
	  	return C_FAIL;
}


/**
 * @brief PollEngine__Write_HR_Req
 *        function that write a holding register via Modbus
 *
 * @param  C_FLOAT write_value, uint16_t addr, C_CHAR num
 * @return C_RES
 */
C_RES PollEngine__Write_HR_Req(C_FLOAT write_value, uint16_t addr, C_CHAR num)
{
	eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

    data_f data;
    C_UINT16 val[2];

	data.reg.high = 0;
	data.reg.low = 0;

	data.value = write_value;

	if(num == 1) {   val[0] = write_value; }
	else{
		val[1] =  data.reg.low;
		val[0] =  data.reg.high;
	}

	errorReq = app_hr_write(1, addr, num, &val);

	if(errorReq == MB_MRE_NO_ERR)
	{
		return C_SUCCESS;
	}
	else
		return C_FAIL;
}

/**
 * @brief PollEngine__Write_COIL_Req
 *        function that write a Coil register via Modbus
 *
 * @param  uint16_t write_value, uint16_t addr
 * @return C_RES
 */
C_RES PollEngine__Write_COIL_Req(uint16_t write_value, uint16_t addr){

	eMBMasterReqErrCode errorReq = MB_MRE_NO_REG;

	uint16_t reg_to_write = 0;
	uint16_t bit = 0;  // ??

	uint16_t value = 0;

	bit = addr % 16; // ??

	if(write_value == 1)
	  value = 0xFF00;
	else
	  value = 0x0000;

	errorReq = app_coil_write(1, addr, value);

	if(errorReq == MB_MRE_NO_ERR)
	{
		return C_SUCCESS;
	}
	else
		return C_FAIL;
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
#if 0
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
#endif
}


void PollEngine_StartEngine_CAREL(void){
	PollEngine_Status.engine = RUNNING;
}

void PollEngine_StopEngine_CAREL(void){
	PollEngine_Status.engine = STOPPED;
}

uint8_t PollEngine_GetEngineStatus_CAREL(void){
	return PollEngine_Status.engine;
}

uint8_t PollEngine_GetPollingStatus_CAREL(void){
	return PollEngine_Status.polling;
}

/*
 * PASS MODE
 */

void PollEngine__ActivatePassMode(void){
	PollEngine_Status.passing_mode = ACTIVATED;
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
	memset((void*)values_buffer, 0, values_buffer_len * sizeof(values_buffer_t));
	values_buffer_index = 0;
	//Reset Time Buffer
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

void PollEngine__RecoverBaudRate(void){
	C_UINT32 baud_rate;
	if(C_SUCCESS == NVM__ReadU32Value(MB_BAUDRATE_NVM, &baud_rate))
	    MB_BaudRate = baud_rate;
	else
	    MB_BaudRate = 115200;
}
