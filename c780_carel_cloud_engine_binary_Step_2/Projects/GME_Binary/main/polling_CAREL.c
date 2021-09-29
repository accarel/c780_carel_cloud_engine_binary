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

#include "SoftWDT.h"

#include "binary_model.h" 

#include "RTC_IS.h"
#include "modbus_IS.h"

#include "polling_IS.h"
#include "polling_CAREL.h"
#include "nvm_CAREL.h"
#include "sys_IS.h"
#include "utilities_CAREL.h"

#include "Led_Manager_IS.h"
#include "mobile.h"

#include "WebDebug.h"
#define RET_DIM(x,l)     (x == 16 ? (l = 1) : (l = 2))


#define OPTS(min_val, max_val, step_val) { .opt1 = min_val, .opt2 = max_val, .opt3 = step_val }
#define INIT_DELAY_TICS                 (100 / portTICK_RATE_MS)
#define TIMEOUT_UPDATE_CIDS_MS          (1000) 
#define TIMEOUT_UPDATE_CIDS_TICS        (TIMEOUT_UPDATE_CIDS_MS / portTICK_RATE_MS)

#define REDUCE_SPEED_ALARM 				(5)  // seconds

#define FILTER_OFFLINE                  (1)

static const char *TAG = "POLLING_CAREL";

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

static poll_req_num_t low_n, high_n, alarm_n;
static uint16_t  cid_counter=0;

static hr_ir_poll_tables_t 		HRLowPollTab;
static hr_ir_poll_tables_t 		HRHighPollTab;
static hr_ir_alarm_tables_t 	*HRAlarmPollTab;


static hr_ir_poll_tables_t 		IRLowPollTab;
static hr_ir_poll_tables_t 		IRHighPollTab;
static hr_ir_alarm_tables_t		*IRAlarmPollTab;

static sampling_tstamp_t timestamp = {0};

// Values and time buffers
// Variable storing the number of values to be sent currently in buffer
static uint16_t values_buffer_count = 0;
// Variable storing the index of the last inserted variable
static uint16_t values_buffer_index = 0;


// Size of the values buffer (calculated on free memory available)
static uint16_t values_buffer_len = 0;
// Pointer to the values buffer
static values_buffer_t *values_buffer = NULL;

static uint32_t MB_BaudRate = 0;

// useful for a MODBUS READING AND QWRITING

USHORT param_buffer[2];				// max 32 bits
eMBErrorCode retError = MB_ENOERR;

static uint8_t PollEnginePrint = POLL_ENGINE_PRINTF_DEFAULT;
static uint8_t first_high = 1;
static uint8_t first_low = 1;

// this helps us in deciding whether we should send an empty values payload
static C_BYTE something_sent = 0;

static C_INT32 modbus_error = 0;
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
 * @brief SetAllErrors
 *        this function manage the error and set the error field
 *        if during the read somenthing happens
 *
 * @param  eMBMasterReqErrCode error
 * @return none
 */
static void SetAllErrors(eMBMasterReqErrCode error){
	int i=0;

	//Coil
	for(i=0;i<low_n.coil;i++)
		COILLowPollTab.reg[i].error = error;
	//DI
	for(i=0;i<low_n.di;i++)
		DILowPollTab.reg[i].error = error;
	//HR
	for(i=0;i<low_n.hr;i++)
		HRLowPollTab.tab[i].error = error;
	//IR
	for(i=0;i<low_n.ir;i++)
		IRLowPollTab.tab[i].error = error;
	//Coil
	for(i=0;i<high_n.coil;i++)
		COILHighPollTab.reg[i].error = error;
	//DI
	for(i=0;i<high_n.di;i++)
		DIHighPollTab.reg[i].error = error;
	//HR
	for(i=0;i<high_n.hr;i++)
		HRHighPollTab.tab[i].error = error;
	//IR
	for(i=0;i<high_n.ir;i++)
		IRHighPollTab.tab[i].error = error;
}

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
		P_COV_LN;
	}


	temp = DeviceParamCount[HIGH_POLLING][COIL];
	if(0 != temp){
		COILHighPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&COILHighPollTab.reg[0] , 0, temp*sizeof(coil_di_low_high_t));
		uint8_t  *p_coil_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILHighPollTab.reg[i].info =  *((r_coil_di*)(p_coil_high_sect + (i * sizeof(r_coil_di))));
		}
		P_COV_LN;
	}


	temp = DeviceParamCount[ALARM_POLLING][COIL];
	if(0 != temp){
		COILAlarmPollTab = malloc(temp * sizeof(coil_di_alarm_tables_t));
		memset((void*)&COILAlarmPollTab[0] , 0, temp*sizeof(coil_di_alarm_tables_t));
		uint8_t  *p_coil_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, COIL);
		for(int i=0;i<temp;i++){
			COILAlarmPollTab[i].info =  *((r_coil_di_alarm*)(p_coil_alarm_sect + (i * sizeof(r_coil_di_alarm))));
		}
		P_COV_LN;
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
		P_COV_LN;
	}


	temp = DeviceParamCount[HIGH_POLLING][DI];
	if(0 != temp){
		DIHighPollTab.reg = malloc(temp * sizeof(coil_di_low_high_t));
		memset((void*)&DIHighPollTab.reg[0] , 0, temp*sizeof(coil_di_low_high_t));
		uint8_t  *p_di_high_sect = BinaryModel__GetPtrSec(HIGH_POLLING, DI);
		for(int i=0;i<temp;i++){
			DIHighPollTab.reg[i].info =  *((r_coil_di*)(p_di_high_sect + (i * sizeof(r_coil_di))));
		}
		P_COV_LN;
	}


	temp = DeviceParamCount[ALARM_POLLING][DI];
	if(0 != temp){
		DIAlarmPollTab = malloc(temp * sizeof(coil_di_alarm_tables_t));
		memset((void*)&DIAlarmPollTab[0] , 0, temp*sizeof(coil_di_alarm_tables_t));
		uint8_t  *p_di_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, DI);
		for(int i=0;i<temp;i++){
			DIAlarmPollTab[i].info =  *((r_coil_di_alarm*)(p_di_alarm_sect + (i * sizeof(r_coil_di_alarm))));
		}
		P_COV_LN;
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
		P_COV_LN;
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
		P_COV_LN;
	}


	temp = DeviceParamCount[ALARM_POLLING][HR];
	if(0 != temp){
		HRAlarmPollTab = malloc(temp * sizeof(hr_ir_alarm_tables_t));
		memset((void*)&HRAlarmPollTab[0] , 0, temp*sizeof(hr_ir_alarm_tables_t));
		uint8_t  *p_hr_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, HR);
		for(int i=0;i<temp;i++){
			HRAlarmPollTab[i].info =  *((r_hr_ir_alarm*)(p_hr_alarm_sect + (i * sizeof(r_hr_ir_alarm))));
		}
		P_COV_LN;
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
		P_COV_LN;
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
		P_COV_LN;
	}


	temp = DeviceParamCount[ALARM_POLLING][IR];
	if(0 != temp){
		IRAlarmPollTab = malloc(temp * sizeof(hr_ir_alarm_tables_t));
		memset((void*)&IRAlarmPollTab[0] , 0, temp*sizeof(hr_ir_alarm_tables_t));
		uint8_t  *p_ir_alarm_sect = BinaryModel__GetPtrSec(ALARM_POLLING, IR);
		for(int i=0;i<temp;i++){
			IRAlarmPollTab[i].info =  *((r_hr_ir_alarm*)(p_ir_alarm_sect + (i * sizeof(r_hr_ir_alarm))));
		}
		P_COV_LN;
	}
	SetAllErrors(MB_MRE_TIMEDOUT);
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

/**
 * @brief create_values_buffers
 *
 * @param  none
 * @return none
 */
void create_values_buffers(void){
	//Allocate values buffer
	uint32_t  freespace = uxTaskGetStackHighWaterMark(NULL);
	freespace -= 1000;
	values_buffer_len = (uint16_t)(freespace/((uint32_t)sizeof(values_buffer_t)));

	// to test buffering TEMPORARY
	// values_buffer_len = 20;

	PRINTF_DEBUG("create_values_buffers %d \n",values_buffer_len);

	values_buffer = malloc(values_buffer_len * sizeof(values_buffer_t));						// malloc
	memset((void*)values_buffer, 0, values_buffer_len * sizeof(values_buffer_t));
}


/**
 * @brief get_type_a
 *		  return a data of type A
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
float get_type_a(hr_ir_low_high_poll_t *arr, uint8_t read_kind){

	float temp, read= 0.0;
	read_kind == CURRENT ? (temp = *((float*)(&arr->c_value.value))) : (temp = *((float*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


/**
 * @brief get_type_b
 *		  return a data of type B
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
float get_type_b(hr_ir_low_high_poll_t *arr, uint8_t read_kind){

	float temp, read= 0.0;
	read_kind == CURRENT ? (temp = *((int16_t*)(&arr->c_value.value))) : (temp = *((int16_t*)(&arr->p_value.value)));  //(float*)
	read = (temp * arr->info.linA) + arr->info.linB;
	return (float)read;
}

/**
 * @brief get_type_c_signed
 *		  return a data of type c signed
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
int32_t get_type_c_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	int32_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((int32_t*)(&arr->c_value.value))) : (temp = *((int32_t*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}


/**
 * @brief get_type_c_unsigned
 *		  return a data of type c unsigned
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
uint32_t get_type_c_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint32_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((uint32_t*)(&arr->c_value.value))) : (temp = *((uint32_t*)(&arr->p_value.value)));
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}

/**
 * @brief get_type_d
 *		  return a data of type d signed
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
uint8_t get_type_d(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint16_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((uint16_t*)(&arr->c_value.value))) : (temp = *((uint16_t*)(&arr->p_value.value)));
	read = (temp & ((uint16_t) (1 << arr->info.bitposition))) >> (arr->info.bitposition) ;


	return (uint8_t)read;
}

/**
 * @brief get_type_e
 *		  return a data of type e
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
int16_t get_type_e(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint16_t temp , read= 0;
	read_kind == CURRENT ? (temp = *((uint16_t*)(&arr->c_value.value))) : (temp = *((uint16_t*)(&arr->p_value.value)));
	read = (temp &  ((0x000F)<< (arr->info.bitposition))) >> (arr->info.bitposition);
    return (int16_t)read;
}

/**
 * @brief get_type_f_signed
 *		  return a data of type f signed
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
int16_t get_type_f_signed(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	int16_t temp, read= 0;

	read_kind == CURRENT ? (temp = *((int16_t*)(&arr->c_value.value))) : (temp = *((int16_t*)(&arr->p_value.value)));
	//PRINTF_DEBUG("get_type_f_signed\n");
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}

/**
 * @brief get_type_f_unsigned
 *		  return a data of type f unsigned
 *
 * @param  hr_ir_low_high_poll_t *arr
 * @param  uint8_t read_kind
 * @return none
 */
uint16_t get_type_f_unsigned(hr_ir_low_high_poll_t *arr, uint8_t read_kind){
	uint16_t temp, read= 0;
	read_kind == CURRENT ? (temp = *((uint16_t*)(&arr->c_value.value))) : (temp = *((uint16_t*)(&arr->p_value.value)));
	//PRINTF_DEBUG("get_type_f_unsigned\n");
	read = (temp * arr->info.linA) + arr->info.linB;
	return read;
}



/**
 * @brief check_increment_values_buff_len
 *		  Routine to check if the values buffer has a free space or not
 *		  If yes, increments the index, otherwise flush the buffer via mqtt
 *
 * @param  uint16_t *values_buffer_idx
 * @return none
 */
static void check_increment_values_buff_len(uint16_t *values_buffer_idx){
	if(*values_buffer_idx < (values_buffer_len - 1)){
		(*values_buffer_idx)++;
	}else{
		*values_buffer_idx = 0;
	//	MQTT__FlushValues();
	}

}


/**
 * @brief check_hr_ir_read_val
 *		  Comparing the current read with previous IR and HR registers reads
 * 					accoring to its type (Look carel registers type).
 *					If there is a diff >= hysteresis, writes the read value + reg info
 *					in values buffer, then increment the values buffer index
 *
 * @param  hr_ir_poll_tables_t *arr (is the HR or IR table)
 * @param  uint8_t arr_len          (the table length)
 * @param  uint8_t first_run
 *
 * @return none
 */
static void check_hr_ir_read_val(hr_ir_poll_tables_t *arr, uint8_t arr_len, uint8_t first_run)
{
	long double value = 0;
	int changed;		// set to 1 when a variable changes
	for(uint8_t i=0; i<arr_len; i++){
		changed = 0;
		if( arr->tab[i].error != arr->tab[i].p_error && ( (arr->tab[i].error != 0) )){
			values_buffer[values_buffer_index].alias = arr->tab[i].info.Alias;
			values_buffer[values_buffer_index].value = 0;
			values_buffer[values_buffer_index].info_err = arr->tab[i].error;
			values_buffer[values_buffer_index].t = timestamp.current_high;
			check_increment_values_buff_len(&values_buffer_index);
			values_buffer_count++;
			if (values_buffer_count > values_buffer_len)
				values_buffer_count = values_buffer_len;
			P_COV_LN;
		}
		else if (arr->tab[i].error == 0){	// manage read values only if there is no error
			// reinit value otherwise all variables will be considered changed
			value = 0;
			switch(arr->tab[i].read_type){
			case TYPE_A:
			{
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val A \n");
                #endif

				float temp, c_read, p_read= 0.0;
				c_read = get_type_a(&arr->tab[i], CURRENT);
				p_read = get_type_a(&arr->tab[i], PREVIOUS);
				temp = fabs(c_read - p_read);

                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %f, p_read: %f, temp: %f\n",c_read, p_read, temp);
                #endif

				if(temp > arr->tab[i].info.Hyster || first_run){
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;

                    #ifdef __DEBUG_POLLING_CAREL_LEV_2
					PRINTF_DEBUG("TYPE_A c_read = %f\n",c_read);
					PRINTF_DEBUG("TYPE_A Value = %Lf\n",value);
                    #endif
					P_COV_LN;
				}
				P_COV_LN;
			}
				break;

			case TYPE_B:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val B \n");
				#endif
				float temp, c_read, p_read= 0.0;
				c_read = get_type_b(&arr->tab[i], CURRENT);
				p_read = get_type_b(&arr->tab[i], PREVIOUS);
				temp = fabs(c_read - p_read);

                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %f, p_read: %f, temp: %f\n",c_read, p_read, temp);
                #endif

				if(temp > arr->tab[i].info.Hyster || first_run){
					arr->tab[i].p_value = arr->tab->c_value;
					value = (long double)c_read;
					changed = 1;
			        #ifdef __DEBUG_POLLING_CAREL_LEV_2
					PRINTF_DEBUG("TYPE_B REG low = %d\n",arr->tab[i].c_value.reg.low);
					PRINTF_DEBUG("TYPE_B REG high = %d\n",arr->tab[i].c_value.reg.high);
					PRINTF_DEBUG("TYPE_B REG val = %d\n",arr->tab[i].c_value.value);
					PRINTF_DEBUG("TYPE_B c_read = %f\n",c_read);
					PRINTF_DEBUG("TYPE_B Value = %Lf\n",value);
                    #endif
					P_COV_LN;
				}
				P_COV_LN;
			}
				break;

			case TYPE_C_SIGNED:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val C_SIGNED \n");
				#endif
				int32_t temp, c_read, p_read= 0;
				c_read = get_type_c_signed(&arr->tab[i], CURRENT);
				p_read = get_type_c_signed(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);

                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
                #endif

				if(temp > arr->tab[i].info.Hyster || first_run){
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;
					P_COV_LN;
				}
				P_COV_LN;
			}
				break;

			case TYPE_C_UNSIGNED:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val C_UNSIGNED \n");
				#endif
				uint32_t temp, c_read, p_read= 0;
				c_read = get_type_c_unsigned(&arr->tab[i], CURRENT);
				p_read = get_type_c_unsigned(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
                #endif
				if(temp > arr->tab[i].info.Hyster || first_run){
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;
					P_COV_LN;
				}
				P_COV_LN;
			}
				break;

			case TYPE_D:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val D \n");
				#endif
				uint8_t c_read, p_read= 0;
				c_read = get_type_d(&arr->tab[i], CURRENT);
				p_read = get_type_d(&arr->tab[i], PREVIOUS);
				if(c_read != p_read  || first_run)
				{
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;
					P_COV_LN;
				}
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %d, p_read: %d\n",c_read, p_read);
                #endif
				P_COV_LN;
			}
				break;

			case TYPE_E:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val E \n");
				#endif
				int32_t temp, c_read, p_read= 0;
				c_read = get_type_e(&arr->tab[i], CURRENT);
				p_read = get_type_e(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
				if(temp > arr->tab[i].info.Hyster || first_run)
				{
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;
					P_COV_LN;
				}
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %d, p_read: %d\n",c_read, p_read);
                #endif
				P_COV_LN;
			}
				break;

			case TYPE_F_SIGNED:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val F_SIGNED \n");
				#endif
				int16_t temp, c_read, p_read= 0;
				c_read = get_type_f_signed(&arr->tab[i], CURRENT);
				p_read = get_type_f_signed(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
                #endif
				if(temp > arr->tab[i].info.Hyster || first_run)
				{
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;
					P_COV_LN;
				}
				P_COV_LN;
			}
					break;

			case TYPE_F_UNSIGNED:
			{
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("check_hr_ir_read_val F_UNSIGNED \n");
				#endif
				uint16_t temp, c_read, p_read= 0;
				c_read = get_type_f_unsigned(&arr->tab[i], CURRENT);
				p_read = get_type_f_unsigned(&arr->tab[i], PREVIOUS);
				temp = abs(c_read - p_read);
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("c_read: %d, p_read: %d, temp: %d\n",c_read, p_read, temp);
                #endif
				if(temp > arr->tab[i].info.Hyster || first_run)
				{
					arr->tab[i].p_value = arr->tab[i].c_value;
					value = (long double)c_read;
					changed = 1;
					P_COV_LN;
				}
				P_COV_LN;
			}
				break;

			default:
				break;
			}
			if(changed != 0 || (first_run)){
				values_buffer[values_buffer_index].alias = arr->tab[i].info.Alias;
				values_buffer[values_buffer_index].value = value;
				values_buffer[values_buffer_index].info_err = 0;
				values_buffer[values_buffer_index].data_type = arr->tab[i].info.dim;
				values_buffer[values_buffer_index].t = timestamp.current_high;
				check_increment_values_buff_len(&values_buffer_index);
				values_buffer_count++;
				if (values_buffer_count > values_buffer_len)
					values_buffer_count = values_buffer_len;
				P_COV_LN;
			}
		}
	}
}


/**
 * @brief check_coil_di_read_val
 *		 Comparing the current read with previous COIL and IR reads
 * 					accoring to its type (Look carel registers type).
 *					If there is a diff >= hysteresis, writes the read value + reg info
 *					in values buffer, then increment the values buffer index
 *
 * @param  coil_di_poll_tables_t *arr (is the COIL or DI table)
 * @param  uint8_t arr_len            (the table length)
 * @param  uint8_t first_run
 *
 * @return none
 */

static void check_coil_di_read_val(coil_di_poll_tables_t *arr, uint8_t arr_len, uint8_t first_run)
{
	for(uint8_t i=0; i<arr_len; i++){
		//error?
		if( arr->reg[i].error != arr->reg[i].p_error && ( (arr->reg[i].error != 0)) ){
			//send values to values buffer as error
			values_buffer[values_buffer_index].alias = arr->reg[i].info.Alias;
			values_buffer[values_buffer_index].value = 0;
			values_buffer[values_buffer_index].info_err = arr->reg[i].error;
			values_buffer[values_buffer_index].t = timestamp.current_high;
			check_increment_values_buff_len(&values_buffer_index);
			values_buffer_count++;
			if (values_buffer_count > values_buffer_len)
				values_buffer_count = values_buffer_len;
		}
		//value changed and no error
		else if((arr->reg[i].error == 0) && (arr->reg[i].c_value != arr->reg[i].p_value || (first_run))){
			//send values to values buffer
			values_buffer[values_buffer_index].alias = arr->reg[i].info.Alias;
			values_buffer[values_buffer_index].value = (long double)arr->reg[i].c_value;
			values_buffer[values_buffer_index].info_err = 0;
			values_buffer[values_buffer_index].t = timestamp.current_high;
			check_increment_values_buff_len(&values_buffer_index);
			values_buffer_count++;
			if (values_buffer_count > values_buffer_len)
				values_buffer_count = values_buffer_len;
		}
	}
}



/**
 * @brief compare_prev_curr_reads
 *		 Routine that manages the hole comparison phase of CURRENT / PREVIOUS reads
 * 		 according to polling type if HIGH or LOW
 * 		 Then updates the values and time buffers
 * 		 Should be called directly after finishing the polling routine
 *
 * @param  PollType_t poll_type
 * @param  uint8_t first
 *
 * @return none
 */
static void compare_prev_curr_reads(PollType_t poll_type, uint8_t first)
{
	//get current index of values buffer
    #ifdef __DEBUG_POLLING_CAREL_LEV_2
	uint16_t index_temp =  values_buffer_count;

	PRINTF_DEBUG("START index_temp = %d, values_buffer_index = %d\n", index_temp, values_buffer_index);
    #endif

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

    #ifdef __DEBUG_POLLING_CAREL_LEV_2
	PRINTF_DEBUG("END index_temp = %d, values_buffer_index = %d\n",index_temp,values_buffer_index);
    #endif

}


/**
 * @brief check_hr_ir_reg_type
 *
 * @param  r_hr_ir info
 *
 * @return hr_ir_read_type_t
 */
hr_ir_read_type_t check_hr_ir_reg_type(r_hr_ir info)
{
	hr_ir_read_type_t type;
	if(info.dim > 16){
		if(1 == info.flag.bit.ieee){
			type = TYPE_A;
			P_COV_LN;
		}else{
			if(1 == info.flag.bit.signed_f){
				type = TYPE_C_SIGNED;
				P_COV_LN;
			}else{
				type = TYPE_C_UNSIGNED;
				P_COV_LN;
			}
		}
	}else{
		if((1 == info.len) && ((info.bitposition >= 0) && (info.bitposition < 16))){
			type = TYPE_D;
			P_COV_LN;
		}else if(1 == info.flag.bit.fixedpoint){
			type = TYPE_B;
			P_COV_LN;
		}else if(info.len == 4 && (info.bitposition >= 0)){
			type = TYPE_E;
			P_COV_LN;
		}else{
			if(1 == info.flag.bit.signed_f){
				type = TYPE_F_SIGNED;
				P_COV_LN;
			}else{
				type = TYPE_F_UNSIGNED;
				P_COV_LN;
			}
		}
	}
	return type;
}


/**
 * @brief update_current_previous_tables
 *		  Updating the previous read with the current one.
 *
 * @param  RegType_t poll_type
 *
 * @return none
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
		for(i=0;i<low_n.hr;i++){
			//HRLowPollTab.tab[i].p_value.value = HRLowPollTab.tab[i].c_value.value;
			HRLowPollTab.tab[i].c_value.value = 0;
			HRLowPollTab.tab[i].p_error = HRLowPollTab.tab[i].error;

		}
		//IR
		for(i=0;i<low_n.ir;i++){
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
		for(i=0;i<high_n.hr;i++){
			//HRHighPollTab.tab[i].p_value.value = HRHighPollTab.tab[i].c_value.value;
			HRHighPollTab.tab[i].c_value.value = 0;
			HRHighPollTab.tab[i].p_error = HRHighPollTab.tab[i].error;
		}

		//IR
		for(i=0;i<high_n.ir;i++){
			//IRHighPollTab.tab[i].p_value.value = IRHighPollTab.tab[i].c_value.value;
			IRHighPollTab.tab[i].c_value.value = 0;
			IRHighPollTab.tab[i].p_error = IRHighPollTab.tab[i].error;
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
 * @param  coil_di_low_high_t *arr
 * @param  void* instance_ptr
 *
 * @return void
 */
static void save_coil_di_value(coil_di_low_high_t *arr, void* instance_ptr){
	uint16_t temp, read_val = 0;
	uint8_t bit=0;

	//bit = arr->info.Addr % 16;
	read_val = *((uint16_t*)(instance_ptr));
	temp = (0x000F)&read_val;                         //& (uint16_t)(1 << bit);

	temp == 0 ? (temp = 0) : (temp = 1);
	arr->c_value = temp;
	P_COV_LN;
}



/**
 * @brief save_hr_ir_value
 *        Save the value into the relative tab
 *
 * @param hr_ir_low_high_poll_t *arr
 * @param void* instance_ptr
 *
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
 * @param void* instance_ptr
 *
 * @return void
 */
static void save_alarm_coil_di_value(coil_di_alarm_tables_t *alarm,  void* instance_ptr)
{
	uint16_t temp, read_val = 0;
	uint8_t  bit=0;

	//bit = alarm->info.Addr % 16;

	read_val = *((uint16_t*)(instance_ptr));

	temp = (0x000F)&read_val;    //& (uint16_t)(1 << bit);
	temp == 0 ? (temp = 0) : (temp = 1);

	if(temp != alarm->data.value){
		if(0 != temp){
			alarm->data.start_time = RTC_Get_UTC_Current_Time();
			alarm->data.stop_time = 0;
			alarm->data.value = 1;
			alarm->data.send_flag = 1;
			P_COV_LN;
		}else{
			alarm->data.stop_time = RTC_Get_UTC_Current_Time();
			alarm->data.value = 0;
			alarm->data.send_flag = 1;
			P_COV_LN;
		}
	}
}

/**
 * @brief save_alarm_hr_ir_value
 *        Save the value into the relative tab
 *
 * @param hr_ir_alarm_tables_t *alarm
 * @param void* instance_ptr
 *
 * @return void
 */
static void save_alarm_hr_ir_value(hr_ir_alarm_tables_t *alarm, void* instance_ptr)
{
	uint16_t temp, read_val = 0;

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
			P_COV_LN;
		}else{
			alarm->data.stop_time = RTC_Get_UTC_Current_Time();
			alarm->data.value = 0;
			alarm->data.send_flag = 1;
			P_COV_LN;
		}
	}
}

/**
 * @brief send_cbor_alarm
 *          Description: send JSON msg via MQTT if any alarm's value is changed
 *
 * @param uint16_t alias
 * @param alarm_read_t *data
 *
 * @return void
 */
static void send_cbor_alarm(uint16_t alias, alarm_read_t *data){
	c_cboralarms cbor_al;
	cbor_al.st = data->start_time;
	cbor_al.et = data->stop_time;
	cbor_al.aty = 1;
	cbor_al.aco = 0;
	itoa(alias,(char*)cbor_al.ali,10);
	MQTT_Alarms(cbor_al);
}


/**
 * @brief send_cbor_offalarm
 *          Description: send JSON msg via MQTT to turn-off the allarm
 *
 * @param uint16_t alias
 * @param uint32_t st
 * @param uint32_t et
 *
 * @return void
 */
static void send_cbor_offalarm(uint16_t alias, uint32_t st, uint32_t et){
	c_cboralarms cbor_al;

	cbor_al.st = st;
	cbor_al.et = et;
	cbor_al.aty = 2;
	cbor_al.aco = 1;
	strcpy((char*)cbor_al.ali, "");

	MQTT_Alarms(cbor_al);
}


/**
 * @brief check_alarms_change
*          Description: Check if any alarm's value is changed, activated or deactivated
*
 * @param void
 *
 * @return void
 */
static void check_alarms_change(void)
{
	uint16_t i;

	for(i=0; i<alarm_n.coil; i++){
		if (1 == COILAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(COILAlarmPollTab[i].info.Alias, &COILAlarmPollTab[i].data);

            #ifdef __DEBUG_POLLING_CAREL_LEV_2
			if(COILAlarmPollTab[i].data.value == 1)
			   printf("Coil Alarm rise num %d \n ",i);
			else
			   printf("Coil Alarm fall num %d \n ",i);
            #endif

			COILAlarmPollTab[i].data.send_flag = 0;
		}
	}

	for(i=0; i<alarm_n.di; i++){
		if (1 == DIAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(DIAlarmPollTab[i].info.Alias, &DIAlarmPollTab[i].data);

            #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_POLL_ENG(("DI Alarm changed num %d \n ",i))	  
            #endif
			DIAlarmPollTab[i].data.send_flag = 0;
		}
	}

	for(i=0; i<alarm_n.hr; i++){
		if (1 == HRAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(HRAlarmPollTab[i].info.Alias,(alarm_read_t*) &HRAlarmPollTab[i].data);

            #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_POLL_ENG(("HR Alarm changed num %d \n ",i))		
            #endif
			HRAlarmPollTab[i].data.send_flag = 0;
		}
	}

	for(i=0; i<alarm_n.ir; i++){
		if (1 == IRAlarmPollTab[i].data.send_flag){
			send_cbor_alarm(IRAlarmPollTab[i].info.Alias,(alarm_read_t*) &IRAlarmPollTab[i].data);

            #ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_POLL_ENG(("IR Alarm changed num %d \n ",i))
            #endif
			IRAlarmPollTab[i].data.send_flag = 0;
		}
	}
}

#ifdef __DEBUG_POLLING_CAREL_LEV_2
void print_ValuesTable(void){
	int i;
	if(PollEngine__GetPollEnginePrintMsgs() == 1){
	PRINTF_DEBUG("Values Buffer\n");
	for(i = 0; i<values_buffer_index;	i++){
		PRINTF_DEBUG("alias: %4d,  value: %4Lf,  error: %d\n" ,
																values_buffer[i].alias,
																values_buffer[i].value,
																values_buffer[i].info_err);
	}
	}
}
#endif


/****************************
* MODBUS POLLING
*****************************/

eMBErrorCode GetResult(void) 	 {  return 	retError; }
void SetResult(eMBErrorCode val) { retError = val;    }

/**
 * @brief DoPolling
*         Execute the modbus reading function (Coil, Di, Hr, Ir) based on a model table
*
 * @param coil_di_poll_tables_t *Coil
 * @param coil_di_poll_tables_t *Di
 * @param hr_ir_poll_tables_t *Hr
 * @param hr_ir_poll_tables_t *Ir
 * @param PollType_t type
 *
 * @return C_RES
 */
static C_RES DoPolling (coil_di_poll_tables_t *Coil, coil_di_poll_tables_t *Di, hr_ir_poll_tables_t *Hr, hr_ir_poll_tables_t *Ir, PollType_t type)
{
	uint8_t addr = 0;
    uint8_t numOf = 0;
    uint8_t retry = 0;
    uint8_t is_offline = 0;
    eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;
    uint8_t ncoil, ndi, nhr, nir;
    if (type == LOW_POLLING) {
    	ncoil = low_n.coil;
    	ndi = low_n.di;
    	nhr = low_n.hr;
    	nir = low_n.ir;
    }
    else {
    	ncoil = high_n.coil;
    	ndi = high_n.di;
    	nhr = high_n.hr;
    	nir = high_n.ir;
    }
	// Polling the Coil register
	for (uint16_t i = 0; i < ncoil; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = (Coil->reg[i].info.Addr);

		do {
			errorReq = app_coil_read(Modbus__GetAddress(), addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);

		Coil->reg[i].error = errorReq;
		if(errorReq == MB_MRE_NO_ERR) {
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_coil_di_value(&Coil->reg[i] , param_buffer);
		}
		else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
            PRINTF_DEBUG("DoPolling Coil i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2){
			SetAllErrors(MB_MRE_TIMEDOUT);
			P_COV_LN;
			//P_COV_LN_OFF("P_COIL");
			return C_FAIL; //this is the start of offline
		}
		param_buffer[0] = param_buffer[1] = 0;

	}

	// Polling the Di register
	for (uint16_t i = 0; i < ndi; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = (Di->reg[i].info.Addr);

		do {
			errorReq = app_coil_discrete_input_read(Modbus__GetAddress(), addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);

		Di->reg[i].error = errorReq;
		if(errorReq == MB_MRE_NO_ERR) {
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_coil_di_value(&Di->reg[i] , param_buffer);
		}
		else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
            PRINTF_DEBUG("DoPolling DI i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}


		if(is_offline == 2){
			SetAllErrors(MB_MRE_TIMEDOUT);
			P_COV_LN;
			//P_COV_LN_OFF("P_DI");
			return C_FAIL;
		}
		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Hr register
	for (uint16_t i = 0; i < nhr; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = Hr->tab[i].info.Addr;

		if((Hr->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;

		do {
			errorReq = app_holding_register_read(Modbus__GetAddress(), addr, numOf);  // ,1
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);

		Hr->tab[i].error = errorReq;
		if(errorReq == MB_MRE_NO_ERR) {
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_hr_ir_value(&Hr->tab[i], param_buffer);   // &HRLowPollTab.tab[i]
		}
		else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
            PRINTF_DEBUG("DoPolling HR i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2){
			SetAllErrors(MB_MRE_TIMEDOUT);
			P_COV_LN;
			//P_COV_LN_OFF("P_HR");
			return C_FAIL;
		}
		param_buffer[0] = param_buffer[1] = 0;
	}

	// POlling the Ir register
	for (uint16_t i = 0; i < nir; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = Ir->tab[i].info.Addr;

		if((Ir->tab[i].info.dim) == 16)
		  numOf = 1;
		else
		  numOf = 2;

		do {
			errorReq = app_input_register_read(Modbus__GetAddress(), addr, numOf);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);

		Ir->tab[i].error = errorReq;
		if(errorReq == MB_MRE_NO_ERR) {
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_hr_ir_value(&Ir->tab[i], param_buffer);
		}
		else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
            PRINTF_DEBUG("DoPolling IR i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2){
			SetAllErrors(MB_MRE_TIMEDOUT);
			P_COV_LN;
			//P_COV_LN_OFF("P_IR");
			return C_FAIL;
		}
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
 * @return C_RES
 */

static C_RES DoAlarmPolling(coil_di_alarm_tables_t *Coil, coil_di_alarm_tables_t *Di, hr_ir_alarm_tables_t *Hr, hr_ir_alarm_tables_t *Ir)
{
	uint8_t addr = 0;
	uint8_t retry = 0;
	uint8_t is_offline = 0;
	eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

	// Polling the Coil register
	for (uint16_t i = 0; i < alarm_n.coil; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = (Coil[i].info.Addr);
		do {
			errorReq = app_coil_read(Modbus__GetAddress(), addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);

        Coil->data.error = errorReq;
		if(errorReq == MB_MRE_NO_ERR)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			//is_offline = 0;
			save_alarm_coil_di_value(&Coil[i], param_buffer);
		}
		else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
			PRINTF_DEBUG("DoAlarmPolling Coil i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2)
		{
			P_COV_LN;
			//P_COV_LN_OFF("A_COIL");
			return C_FAIL; //this is an offline
		}

		param_buffer[0] = param_buffer[1] = 0;
	}



	// Polling the Di register
	for (uint16_t i = 0; i < alarm_n.di; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = (Di[i].info.Addr);

		do {
			errorReq = app_coil_discrete_input_read(Modbus__GetAddress(), addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);

		Di->data.error = errorReq;

		if(errorReq == MB_MRE_NO_ERR)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_alarm_coil_di_value(&Di[i], param_buffer);
		}
		else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
			PRINTF_DEBUG("DoAlarmPolling DI i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2)
		{
			P_COV_LN;
		    //P_COV_LN_OFF("A_DI");
			return C_FAIL; //this is an offline
		}

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Hr register
	for (uint16_t i = 0; i < alarm_n.hr; i++)
	{
		errorReq = MB_MRE_NO_ERR;
		retry = 0;
		addr = (Hr[i].info.Addr);

		do {
			errorReq = app_holding_register_read(Modbus__GetAddress(), addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);
		Hr->data.error = errorReq;
		if(errorReq == MB_MRE_NO_ERR)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_alarm_hr_ir_value(&Hr[i], param_buffer);
		}else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
			PRINTF_DEBUG("DoAlarmPolling HR i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2)
		{
			P_COV_LN;
			//P_COV_LN_OFF("A_HR");
			return C_FAIL; //this is an offline
		}

		param_buffer[0] = param_buffer[1] = 0;
	}

	// Polling the Ir register
	for (uint16_t i = 0; i < alarm_n.ir; i++)
	{
		errorReq = MB_MRE_NO_ERR;  //TODO CPPCHECK di fatto non serve a nulla viene assegnata sotto
		retry = 0;
		addr = (Ir[i].info.Addr);

		do {
			errorReq = app_input_register_read(Modbus__GetAddress(), addr, 1);
			retry++;
		} while(errorReq != MB_MRE_NO_ERR && retry < 3);
		Ir->data.error = errorReq;
		if(errorReq == MB_MRE_NO_ERR)
		{
			// reset to the default for the next reading
			SetResult(MB_ENOERR);
			save_alarm_hr_ir_value(&Ir[i], param_buffer);
		}else
		{
			modbus_error++; // only for web debug

			is_offline++;
            #ifdef __DEBUG_POLLING_CAREL_LEV_1
			PRINTF_DEBUG("DoAlarmPolling IR i=%X errorReq %X \r\n", i, errorReq);
            #endif
		}

		if(is_offline == 2)
		{
			P_COV_LN;
			//P_COV_LN_OFF("A_IR");
			return C_FAIL; //this is an offline
		}

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


static uint32_t timeout = 0;
static uint32_t start_offline = 0;
static uint32_t end_offline = 0;
static C_BYTE   real_offline = 0;

void SendOffline(C_RES poll_done) {

	uint32_t time_tmp;

	if (poll_done == C_FAIL) {
		if (start_offline == 0) {
			start_offline = RTC_Get_UTC_Current_Time();
			//send_cbor_offalarm("", start_offline, 0);
			real_offline = 0;
			// avoid Modbus engine to stop
			//vMBMasterRunResRelease();
			P_COV_LN;
		}
		else if(real_offline == 0) // we enter here after a previously C_FAIL
		{
			time_tmp = RTC_Get_UTC_Current_Time();

			// now we check if we are in a spurious offline or in a real offline
			if((time_tmp - start_offline) > FILTER_OFFLINE)
			{
			  Update_Led_Status(LED_STAT_RS485, LED_STAT_OFF);

			  send_cbor_offalarm("", start_offline, 0);

			  real_offline = 1;

			  // avoid Modbus engine to stop
			  vMBMasterRunResRelease();
			  P_COV_LN;
			}
		}
	}else{
		Update_Led_Status(LED_STAT_RS485, LED_STAT_ON);
		if (start_offline != 0 && real_offline) {
			end_offline = RTC_Get_UTC_Current_Time();
			send_cbor_offalarm("", start_offline, end_offline);
			start_offline = end_offline = 0;
			ForceSending();
			P_COV_LN;
		}
		else
		{
			//fake alarm
			start_offline = end_offline = 0;
			real_offline = 0;
		}
	}
}

bool IsOffline(void) {
	if (start_offline != 0 && end_offline == 0)
		return true;
	else return false;
}

bool IsRealOffline(void)
{
	return real_offline;
}

/**
 * @brief DoPolling_CAREL
 *        function with the timing to apply for low check and high check
 *		  check also the alarm
 *
 * @param  req_set_gw_config_t * polling_times
 * @return none
 */
void DoPolling_CAREL(req_set_gw_config_t * polling_times)
{
	C_RES poll_done = C_FAIL;

	C_BYTE low_trigger = 0;
	C_BYTE high_trigger = 0;
	C_BYTE pva_trigger = 0;
	C_BYTE relax_alarm_polling = 0;


	#ifdef __DEBUG_POLLING_CAREL_LEV_1
	C_UINT32 cronometro;
    #endif

		mb_rw_call_execute();

		if(RUNNING == PollEngine_Status.engine && Mobile_GetCommandMode() == 0)
		{
		    SoftWDT_Reset(SWWDT_POLLING);

			PollEngine_Status.polling = RUNNING;

			timeout = RTC_Get_UTC_Current_Time();

			if(timeout > (timestamp.current_alarm) && alarm_n.total > 0) {
			   //ALARM POLLING
                #ifdef __DEBUG_POLLING_CAREL_LEV_2
	//			PRINTF_DEBUG("ALR %X\n", timeout);
                #endif

				relax_alarm_polling = (get_relax() == true ? 10 : 0);
#ifdef __DEBUG_POLLING_CAREL_LEV_1
				PRINTF_DEBUG("relax time %d \r\n", relax_alarm_polling);
#endif
				if((Dev_LogFile_GetSM() == LOGFILE_INIT) || (Dev_LogFile_GetSM() == LOGFILE_IDLE))
				   timestamp.current_alarm = RTC_Get_UTC_Current_Time() + relax_alarm_polling;  // Polling allarm best effort
				else
					timestamp.current_alarm = RTC_Get_UTC_Current_Time() + REDUCE_SPEED_ALARM;  // reduce the polling of alarm during download log

                #ifdef __DEBUG_POLLING_CAREL_LEV_1
				cronometro = RTC_Get_UTC_Current_Time();
                #endif

				poll_done = DoAlarmPolling(COILAlarmPollTab, DIAlarmPollTab, HRAlarmPollTab, IRAlarmPollTab);

                #ifdef __DEBUG_POLLING_CAREL_LEV_1
				cronometro = RTC_Get_UTC_Current_Time() - cronometro;
			//	PRINTF_DEBUG("ALR POLL TIME %X\n", cronometro);
                #endif

				#ifdef __DEBUG_POLLING_CAREL_LEV_1
				PRINTF_DEBUG("%s DoAlarmPolling poll_done = %d \n", TAG, poll_done);
				#endif
				SendOffline(poll_done);

				check_alarms_change();

				set_relax(false);
			}

			timeout = RTC_Get_UTC_Current_Time();

			if(timeout > (timestamp.current_high + polling_times->hispeedsamplevalue)   &&   high_n.total > 0) { high_trigger = 1; }
			if(timeout > (timestamp.current_low + polling_times->lowspeedsamplevalue)   &&   low_n.total > 0) { low_trigger = 1; }
			if(timeout > (timestamp.current_pva + Utilities__GetGWConfigData()->valuesPeriod)  &&  high_trigger == 1) { pva_trigger = 1; }

			if((high_trigger && low_trigger)) {

				mb_rw_call_execute();

				timestamp.current_high = timeout;
				timestamp.current_low = timeout;
				//HIGH POLLING
                #ifdef __DEBUG_POLLING_CAREL_LEV_1
				cronometro = RTC_Get_UTC_Current_Time();
                #endif

				poll_done = DoPolling(&COILHighPollTab, &DIHighPollTab, &HRHighPollTab, &IRHighPollTab, HIGH_POLLING);
				SendOffline(poll_done);
				PRINTF_DEBUG("%s COILHighPollTab poll_done = %d \n", TAG, poll_done);

				poll_done = DoPolling(&COILLowPollTab, &DILowPollTab, &HRLowPollTab, &IRLowPollTab, LOW_POLLING);
				PRINTF_DEBUG("%s COILLowPollTab poll_done = %d \n", TAG, poll_done);

                #ifdef __DEBUG_POLLING_CAREL_LEV_1
				cronometro = RTC_Get_UTC_Current_Time() - cronometro;
				PRINTF_DEBUG("H+L POLL TIME %X\n", cronometro);
                #endif


				SendOffline(poll_done);
				FlushValues(LOW_POLLING);
				FlushValues(HIGH_POLLING);
				if (PollEngine__GetValuesBufferCount()) {
					MQTT_FlushValues();
					something_sent = 1;
				}
				high_trigger = 0;
				low_trigger = 0;
			}

			if (high_trigger) {
				//LOW POLLING

				mb_rw_call_execute();

				timestamp.current_high = timeout;

                #ifdef __DEBUG_POLLING_CAREL_LEV_1
				cronometro = RTC_Get_UTC_Current_Time();
                #endif

				poll_done = DoPolling(&COILHighPollTab, &DIHighPollTab, &HRHighPollTab, &IRHighPollTab, HIGH_POLLING);

                #ifdef __DEBUG_POLLING_CAREL_LEV_1
				cronometro = RTC_Get_UTC_Current_Time() - cronometro;
				PRINTF_DEBUG("H POLL TIME %X\n", cronometro);
                #endif

				SendOffline(poll_done);
				PRINTF_DEBUG("%s COILHighPollTab poll_done = %d \n", TAG, poll_done);

				FlushValues(HIGH_POLLING);
				if (PollEngine__GetValuesBufferCount()) {
					MQTT_FlushValues();
					something_sent = 1;
				}
				high_trigger = 0;
			}

			if (pva_trigger) {
				#ifdef __DEBUG_POLLING_CAREL_LEV_2
				PRINTF_DEBUG("PVA %d\n", timeout);
				#endif
				timestamp.current_pva = timeout;
				pva_trigger = 0;
				if(something_sent == 0) {
				// in previous pva seconds no values message was sent, then send an empty one now
					MQTT_FlushValues();
				}
				else if(something_sent == 1)
				// in previous pva seconds at least one values message was sent, then send nothing
					something_sent = 0;
			}
		}
		PollEngine_Status.polling = STOPPED;
		RetriveDataDebug(WEBDBG_POLLING, PollEngine_GetEngineStatus_CAREL());
		RetriveDataDebug(WEBDBG_MODBUS_RTU, modbus_error);
}

void FlushValues(PollType_t type){
	compare_prev_curr_reads(type, IsForced(type));
	ResetForced(type);
	update_current_previous_tables(type);
}
// CHIEBAO A.

/**
 * @brief PollEngine__Read_HR_IR_Req
 *        function that read a HR/IR register via Modbus
 *
 * @param  C_FLOAT write_value
 * @param  uint16_t addr
 * @param  C_BYTE dim
 * @param  C_UINT16* read_value
 *
 * @return C_RES
 */
C_RES PollEngine__Read_HR_IR_Req(C_UINT16 func, C_UINT16 addr, C_BYTE dim, C_UINT16* read_value)
{
	eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

	C_CHAR len = 0;

	RET_DIM(dim,len);

	if(func == mbR_HR)
		errorReq = app_holding_register_read(Modbus__GetAddress(), addr, len);
	else // mbR_IR
		errorReq = app_input_register_read(Modbus__GetAddress(), addr, len);

	if(errorReq == MB_MRE_NO_ERR)
	{
		*read_value 	  = param_buffer[0];
		*(read_value + 1) = param_buffer[1];

		return C_SUCCESS;
	}
	else
	{
		P_COV_LN;
	  	return C_FAIL;
	}
}



/**
 * @brief PollEngine__Read_COIL_DI_Req
 *        function that read a Coil/Di register via Modbus
 *
 * @param  C_FLOAT write_value
 * @param  uint16_t addr
 * @param  C_UINT16* read_value
 *
 * @return C_RES
 */
C_RES PollEngine__Read_COIL_DI_Req(C_UINT16 func, C_UINT16 addr, C_UINT16* read_value){

	eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

	if(func == mbR_COIL)
	   errorReq = app_coil_read(Modbus__GetAddress(), addr, 1);
	else // mbR_DI
	   errorReq = app_coil_discrete_input_read(Modbus__GetAddress(), addr, 1);

	if(errorReq == MB_MRE_NO_ERR)
	{
		*read_value = param_buffer[0];
		return C_SUCCESS;
	}
	else
	{
		P_COV_LN;
	  	return C_FAIL;
	}
}


/**
 * @brief PollEngine__Write_HR_Req
 *        function that write a holding register via Modbus
 *
 * @param  C_FLOAT write_value
 * @param  uint16_t addr
 * @param  C_CHAR num
 * @param  C_BYTE is_big_end
 * @param  C_UINT16 fun
 *
 * @return C_RES
 */
C_RES PollEngine__Write_HR_Req(C_FLOAT write_value, uint16_t addr, C_CHAR num, C_BYTE is_big_end, C_UINT16 fun){

	eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

    data_f data;
    C_UINT16 val[2];

	data.reg.high = 0;
	data.reg.low = 0;

	data.value = write_value;

	if(num == 1) {   val[0] = write_value; }
	else{

		if (is_big_end == 0)
		{
		  val[1] =  data.reg.high;
		  val[0] =  data.reg.low;
		}
		else
		{
		  val[1] =  data.reg.low;
		  val[0] =  data.reg.high;
		}
	}

	if (fun == mbW_HR)
		errorReq = app_hr_write(Modbus__GetAddress(), addr, num, &val, SINGLE);
	else if (fun == mbW_HRS)
		errorReq = app_hr_write(Modbus__GetAddress(), addr, num, &val, MULTI);
	else
		errorReq = MB_MRE_ILL_ARG;	// invalid fun

	if(errorReq == MB_MRE_NO_ERR)
	{
		return C_SUCCESS;
	}
	else
	{
		P_COV_LN;
		return C_FAIL;
	}
}


/**
 * @brief PollEngine__Write_HR_Req_Int
 *        function that write a holding register via Modbus (integer)
 *
 * @param  C_INT32 write_value
 * @param  uint16_t addr
 * @param  C_CHAR num
 * @param  C_BYTE is_big_end
 * @param  C_UINT16 fun
 *
 * @return C_RES
 */
C_RES PollEngine__Write_HR_Req_Int(C_INT32 write_value, uint16_t addr, C_CHAR num, C_BYTE is_big_end, C_UINT16 fun){

	eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

	data_int_f data;
    C_UINT16 val[2];

	data.reg.high = 0;
	data.reg.low = 0;

	data.value = write_value;

	if(num == 1) {   val[0] = write_value; }
	else{

		if (is_big_end == 0)
		{
		  val[1] =  data.reg.high;
		  val[0] =  data.reg.low;
		}
		else
		{
		  val[1] =  data.reg.low;
		  val[0] =  data.reg.high;
		}
	}

	if (fun == mbW_HR)
		errorReq = app_hr_write(Modbus__GetAddress(), addr, num, &val, SINGLE);
	else if (fun == mbW_HRS)
		errorReq = app_hr_write(Modbus__GetAddress(), addr, num, &val, MULTI);
	else
		errorReq = MB_MRE_ILL_ARG; 	// invalid fun

	if(errorReq == MB_MRE_NO_ERR)
	{
		P_COV_LN;
		return C_SUCCESS;
	}
	else
	{
		P_COV_LN;
		return C_FAIL;
	}
}

/**
 * @brief PollEngine__Write_COIL_Req
 *        function that write a Coil register via Modbus
 *
 * @param  uint16_t write_value
 * @param  uint16_t addr
 * @param  uint16_t fun
 *
 * @return C_RES
 */
C_RES PollEngine__Write_COIL_Req(uint16_t write_value, uint16_t addr, C_UINT16 fun){

	eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

	uint16_t value = 0;

	if(write_value == 1)
	  value = 0xFF00;
	else
	  value = 0x0000;

	if (fun == mbW_COIL)
		errorReq = app_coil_write(Modbus__GetAddress(), addr, value, SINGLE);
	else if (fun == mbW_COILS)
		errorReq = app_coil_write(Modbus__GetAddress(), addr, write_value, MULTI);
	else
		errorReq = MB_MRE_ILL_ARG;	// invalid fun

	if(errorReq == MB_MRE_NO_ERR)
	{
		P_COV_LN;
		return C_SUCCESS;
	}
	else
	{
		P_COV_LN;
		return C_FAIL;
	}
}

/**
 * @brief PollEngine_StartEngine_CAREL
 *        Set the polling engine to RUNNING
 *
 * @param  none
 * @return none
 */
void PollEngine_StartEngine_CAREL(void){
	PollEngine_Status.engine = RUNNING;
}

/**
 * @brief PollEngine_StopEngine_CAREL
 *        Set the polling engine to STOPPED
 *
 * @param  none
 * @return none
 */
void PollEngine_StopEngine_CAREL(void){
	PollEngine_Status.engine = STOPPED;
}

/**
 * @brief PollEngine_GetEngineStatus_CAREL
 *        Get the polling engine status of the engine
 *
 * @param  none
 * @return uint8_t
 */
uint8_t PollEngine_GetEngineStatus_CAREL(void){
	return PollEngine_Status.engine;
}

/**
 * @brief PollEngine_GetStatusForSending_CAREL
 *        Get the polling engine status of the engine
 *        and translates it to the value required by cloud
 *
 * @param  none
 * @return uint8_t
 */
uint8_t PollEngine_GetStatusForSending_CAREL(void){
	uint8_t status = PollEngine_GetEngineStatus_CAREL();
	switch(status)
	{
		case STOPPED:
			return 2;
		case RUNNING:
			return 1;
		case NOT_INITIALIZED:
			P_COV_LN;
			return 3;
		default:
			P_COV_LN;
			return 0;
	}
}
/**
 * @brief PollEngine_GetEngineStatus_CAREL
 *        Get the polling engine status of the polling
 *
 * @param  none
 * @return uint8_t
 */
uint8_t PollEngine_GetPollingStatus_CAREL(void){
	return PollEngine_Status.polling;
}

/**
 * @brief PollEngine__GetValuesBuffer
 *
 *
 * @param  none
 * @return alues_buffer_t*
 */
values_buffer_t* PollEngine__GetValuesBuffer(void){
	return values_buffer;
}

/**
 * @brief PollEngine__GetValuesBufferCount
 *
 *
 * @param  none
 * @return uint16_t
 */
uint16_t PollEngine__GetValuesBufferCount(void){
//	PRINTF_DEBUG("values_buffer_count: %d\n", values_buffer_count);
	return values_buffer_count;
}

/**
 * @brief PollEngine__ResetValuesBuffer
 *
 *
 * @param  none
 * @return void
 */
void PollEngine__ResetValuesBuffer(void){
	//Reset Values Buffer
	memset((void*)values_buffer, 0, values_buffer_len * sizeof(values_buffer_t));
	values_buffer_count = 0;
	values_buffer_index = 0;
}

/**
 * @brief PollEngine__GetMBBaudrate
 *
 *
 * @param  none
 * @return uint32_t
 */
uint32_t PollEngine__GetMBBaudrate(void){
	assert(MB_BaudRate > 0);
	return MB_BaudRate;
}

/**
 * @brief PollEngine__GetPollEnginePrintMsgs
 *
 *
 * @param  none
 * @return uint8_t
 */
uint8_t PollEngine__GetPollEnginePrintMsgs(void){
	return PollEnginePrint;
}

/**
 * @brief PollEngine__SetPollEnginePrintMsgs
 *
 *
 * @param  uint8_t status
 * @return none
 */
void PollEngine__SetPollEnginePrintMsgs(uint8_t status){
	PollEnginePrint = status;
}

/**
 * @brief PollEngine__ReadBaudRateFromNVM
 *
 *
 * @param  none
 * @return none
 */
void PollEngine__ReadBaudRateFromNVM(void){
	C_UINT32 baud_rate;
	if(C_SUCCESS == NVM__ReadU32Value(MB_BAUDRATE_NVM, &baud_rate))
	    MB_BaudRate = baud_rate;
	else
	    MB_BaudRate = MB_BAUDRATE;
}

/**
 * @brief Get_SamplingTime
 *
 *
 * @param  C_UINT16 index
 * @return C_TIME
 */
C_TIME Get_SamplingTime(C_UINT16 index) {
    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("Get_SamplingTime index: %d, t:%d\n", index, values_buffer[index].t);
    #endif
	return values_buffer[index].t;
}

/**
 * @brief Get_Alias
 *
 *
 * @param  C_UINT16 index
 * @param  char* alias_tmp
 * @return C_CHAR*
 */
C_CHAR* Get_Alias(C_UINT16 index, char* alias_tmp) {
    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("Get_Alias index: %d, alias:%d\n", index, values_buffer[index].alias);
    #endif
	itoa(values_buffer[index].alias, (char*)alias_tmp, 10);

	return alias_tmp;
}

/**
 * @brief Get_Value
 *
 *
 * @param  C_UINT16 index
 * @param  char* value_tmp
 *
 * @return C_CHAR*
 */
C_CHAR* Get_Value(C_UINT16 index, char* value_tmp) {
    #ifdef __DEBUG_MQTT_INTERFACE_LEV_2
	PRINTF_DEBUG("Get_Value index: %d, vls:%Lf\n", index, values_buffer[index].value);
    #endif
	if(0 != values_buffer[index].info_err)
		memcpy(value_tmp, "", 1);
	else{
		if(values_buffer[index].data_type != 16)
			sprintf(value_tmp, "%.1Lf", values_buffer[index].value);
		else
		    itoa(values_buffer[index].value, value_tmp, 10);
	}
	return value_tmp;
}
