
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "types.h"
#include "binary_model.h"
#include "File_System_CAREL.h"

#include "polling_CAREL.h"
#include "nvm_CAREL.h"
#include "main_CAREL.h"

#include "data_types_CAREL.h"

// locale define
#define WANT_DUMP_MODEL	0

//#define __DEBUG_BYNARY_MODEL

#if WANT_DUMP_MODEL
static void dump_all_values(uint8_t* val);
#endif

static struct HeaderModel myHeaderModel, *ptmyHeaderModel;

static struct NumOfPoll	 myLowPoll,    *ptmyLowPoll;
static struct NumOfPoll	 myHighPoll,   *ptmyHighPoll;
static struct NumOfPoll  myAlarmPoll,  *ptmyAlarmPoll;

//static uint8_t DeviceRegNo [MAX_POLLING][MAX_REG] = {0};

static  uint8_t *p_coil_low_sect;
static  uint8_t *p_di_low_sect;
static  uint8_t   *p_hr_low_sect;
static  uint8_t   *p_ir_low_sect;


static  uint8_t *p_coil_high_sect;
static  uint8_t *p_di_high_sect;
static  uint8_t   *p_hr_high_sect;
static  uint8_t   *p_ir_high_sect;


static  uint8_t  *p_coil_alarm_sect;
static  uint8_t  *p_di_alarm_sect;
static  uint8_t   *p_hr_alarm_sect;
static  uint8_t   *p_ir_alarm_sect;

uint16_t Crc;
bool valid_model;

#if WANT_DUMP_MODEL

//----------------------COIL-------------------------
static void dump_coil_low(void)
{
	unsigned int count;
	struct record_coil_di* r_coil;

	printf("LOW Coil:\n");
	for (count = 0; count < ptmyLowPoll->numOfCOIL; count++)
	{
		r_coil = p_coil_low_sect + (count * sizeof(r_coil_di));

		printf("%4d, %4d", r_coil->Alias, r_coil->Addr);
		printf("\n");		
	}
}

static void dump_coil_high(void)
{
	unsigned int count;
	struct record_coil_di* r_coil;

	printf("HIGH Coil:\n");

	for (count = 0; count < ptmyHighPoll->numOfCOIL; count++)
	{
		r_coil = p_coil_high_sect + (count * sizeof(r_coil_di));

		printf("%4d, %4d", r_coil->Alias, r_coil->Addr);
		printf("\n");
	}
}

static void dump_coil_alarm(void)
{
	unsigned int count;
	struct record_coil_di* r_coil;

	printf("ALARM Coil:\n");

	for (count = 0; count < ptmyAlarmPoll->numOfCOIL; count++)
	{
		r_coil = p_coil_alarm_sect + (count * sizeof(r_coil_di));

		printf("%4d, %4d", r_coil->Alias, r_coil->Addr);
		printf("\n");
	}
}

//-------------------------DI----------------------
static void dump_di_low(void)
{
	unsigned int count;
	struct record_coil_di* r_di;

	printf("LOW Di:\n");

	for (count = 0; count < ptmyLowPoll->numOfDISC; count++)
	{
		r_di = p_di_low_sect + (count * sizeof(r_coil_di));

		printf("%4d, %4d", r_di->Alias, r_di->Addr);
		printf("\n");

	}
}

static void dump_di_high(void)
{
	unsigned int count;
	struct record_coil_di* r_di;

	printf("HIGH Di:\n");

	for (count = 0; count < ptmyHighPoll->numOfDISC; count++)
	{
		r_di = p_di_high_sect + (count * sizeof(r_coil_di));

		printf("%4d, %4d", r_di->Alias, r_di->Addr);
		printf("\n");

	}
}

static void dump_di_alarm(void)
{
	unsigned int count;
	struct record_coil_di* r_di;

	printf("ALARM Di:\n");

	for (count = 0; count < ptmyAlarmPoll->numOfDISC; count++)
	{
		r_di = p_di_alarm_sect + (count * sizeof(r_coil_di));

		printf("%4d, %4d", r_di->Alias, r_di->Addr);
		printf("\n");

	}
}

//------------------------HR--------------------------
static void dump_hr_low(void)
{
	unsigned int count;
	struct record_hr_ir* r_hr;

	printf("LOW Hr:\n");

	for (count = 0; count < ptmyLowPoll->numOfHR; count++)
	{
		r_hr = p_hr_low_sect + (count * sizeof(r_hr_ir));

		printf("%4d, %4d, %4d, %4d, %4d, %4d, %.2f, %.2f, %.2f", 
			             r_hr->Alias,
			             r_hr->Addr,
						 r_hr->flag.byte,
						 r_hr->dim,
						 r_hr->bitposition,
						 r_hr->len,
						 r_hr->linA,
						 r_hr->linB,
						 r_hr->Hyster
		);
		printf("\n");
	}
}

static void dump_hr_high(void)
{
	unsigned int count;
	struct record_hr_ir* r_hr;

	printf("HIGH Hr:\n");

	for (count = 0; count < ptmyHighPoll->numOfHR; count++)
	{
		r_hr = p_hr_high_sect + (count * sizeof(r_hr_ir));

		printf("%4d, %4d, %4d, %4d, %4d, %4d, %.2f, %.2f, %.2f",
			r_hr->Alias,
			r_hr->Addr,
			r_hr->flag.byte,
			r_hr->dim,
			r_hr->bitposition,
			r_hr->len,
			r_hr->linA,
			r_hr->linB,
			r_hr->Hyster
		);
		printf("\n");
	}
}

static void dump_hr_alarm(void)
{
	unsigned int count;
	struct record_hr_ir_alarm* r_hr;

	printf("ALARM Hr:\n");

	for (count = 0; count < ptmyAlarmPoll->numOfHR; count++)
	{
		r_hr = p_hr_alarm_sect + (count * sizeof(r_hr_ir_alarm));

		printf("%4d, %4d,",
			r_hr->Alias,
			r_hr->Addr
		);
		printf("\n");
	}
}

//------------------------IR--------------------------
static void dump_ir_low(void)
{
	unsigned int count;
	struct record_hr_ir* r_ir;

	printf("LOW Ir:\n");

	for (count = 0; count < ptmyLowPoll->numOfIR; count++)
	{
		r_ir = p_ir_low_sect + (count * sizeof(r_hr_ir));

		printf("%4d, %4d, %4d, %4d, %4d, %4d, %.2f, %.2f, %.2f", r_ir->Alias,
			r_ir->Addr,
			r_ir->flag.byte,
			r_ir->dim,
			r_ir->bitposition,
			r_ir->len,
			r_ir->linA,
			r_ir->linB,
			r_ir->Hyster
		);
		printf("\n");
	}
}

static void dump_ir_high(void)
{
	unsigned int count;
	struct record_hr_ir* r_ir;

	printf("HIGH Ir:\n");

	for (count = 0; count < ptmyHighPoll->numOfIR; count++)
	{
		r_ir = p_ir_high_sect + (count * sizeof(r_hr_ir));

		printf("%4d, %4d, %4d, %4d, %4d, %4d, %.2f, %.2f, %.2f", r_ir->Alias,
			r_ir->Addr,
			r_ir->flag.byte,
			r_ir->dim,
			r_ir->bitposition,
			r_ir->len,
			r_ir->linA,
			r_ir->linB,
			r_ir->Hyster
		);
		printf("\n");
	}
}

static void dump_ir_alarm(void)
{
	unsigned int count;
	struct record_hr_ir_alarm* r_ir;

	printf("ALARM Ir:\n");

	for (count = 0; count < ptmyAlarmPoll->numOfIR; count++)
	{
		r_ir = p_ir_alarm_sect + (count * sizeof(r_hr_ir_alarm));

		printf("%4d, %4d,",
			r_ir->Alias,
			r_ir->Addr
		);
		printf("\n");
	}
}

#endif

//-----------------------------------------------------



/*
 CRC Function (for Modbus RTU with 16bit)
*/
uint16_t CRC16(const uint8_t *nData, uint16_t wLength)
{
	static const u_int wCRCTable[] = {
	0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
	0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
	0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

	uint8_t nTemp;
	uint16_t wCRCWord = 0xFFFF;


	while (wLength)
	{
		nTemp = *nData++ ^ wCRCWord;
		wCRCWord >>= 8;
		wCRCWord ^= wCRCTable[nTemp];

		//printf("CRC %d - %X \r\n", wLength, wCRCWord);

		wLength--;
	}


	return wCRCWord;

}



static void get_model_pointers(uint8_t *val)
{

	uint8_t* model_data_begin;


	ptmyLowPoll   = (struct NumOfPoll*)(val + sizeof(H_HeaderModel));
	ptmyHighPoll  = (struct NumOfPoll*)(val + sizeof(H_HeaderModel) + sizeof(myNumOfPoll));
	ptmyAlarmPoll = (struct NumOfPoll*)(val + sizeof(H_HeaderModel) + 2 * sizeof(myNumOfPoll));


	/* --------------------------------------------------------------------------------- */

	model_data_begin = (val + sizeof(myHeaderModel) + (3 * sizeof(myNumOfPoll)));
	// COIL
	p_coil_low_sect  = model_data_begin;
	p_coil_high_sect =  (p_coil_low_sect  + ((ptmyLowPoll->numOfCOIL) * sizeof(r_coil_di)));
	p_coil_alarm_sect = (p_coil_high_sect + ((ptmyHighPoll->numOfCOIL) * sizeof(r_coil_di)));


	// DI
	p_di_low_sect    = (p_coil_alarm_sect + ((ptmyAlarmPoll->numOfCOIL) * sizeof(r_coil_di)));
	p_di_high_sect   = (p_di_low_sect	  + ((ptmyLowPoll->numOfDISC) * sizeof(r_coil_di)));
	p_di_alarm_sect  = (p_di_high_sect    + ((ptmyHighPoll->numOfDISC) * sizeof(r_coil_di)));


	//HR
	p_hr_low_sect    = (p_di_alarm_sect   + ((ptmyAlarmPoll->numOfDISC) * sizeof(r_coil_di)));
	p_hr_high_sect   = (p_hr_low_sect     + ((ptmyLowPoll->numOfHR)     * sizeof(r_hr_ir)));
	p_hr_alarm_sect  = (p_hr_high_sect    + ((ptmyHighPoll->numOfHR)    * sizeof(r_hr_ir)));


	//IR
	p_ir_low_sect    = (p_hr_alarm_sect   + ((ptmyAlarmPoll->numOfHR)  * sizeof(r_hr_ir_alarm)));
	p_ir_high_sect   = (p_ir_low_sect     + ((ptmyLowPoll->numOfIR)   * sizeof(r_hr_ir)));
	p_ir_alarm_sect  = (p_ir_high_sect    + ((ptmyHighPoll->numOfIR)   * sizeof(r_hr_ir)));

}


#if WANT_DUMP_MODEL
static void dump_all_values(uint8_t* val)
{
	//get_model_pointers(val);
	dump_coil_low();
	dump_coil_high();
	dump_coil_alarm();

	dump_di_low();
	dump_di_high();
	dump_di_alarm();

	dump_hr_low();
	dump_hr_high();
	dump_hr_alarm();

	dump_ir_low();
	dump_ir_high();
	dump_ir_alarm();
}
#endif

/* Print Model info on console */

static void GetDeviceInfo(uint8_t *val)
{
	ptmyHeaderModel = (struct HeaderModel *)val;
	myHeaderModel = *ptmyHeaderModel;
	size_t len = 0;

#ifdef __DEBUG_BYNARY_MODEL
	// show INFO
	printf("Header signature:  %s %s", myHeaderModel.signature, "\n");
	printf("Header version  :  %d %s", myHeaderModel.version  , "\n");
	printf("Model GUID      :");



	if(C_SUCCESS != NVM__ReadBlob(MODEL_GUID, myHeaderModel.guid, &len)){
		NVM__WriteBlob (MODEL_GUID, myHeaderModel.guid, 16);
	}

	printf("Model GUID      :");
	for (int i=0; i < sizeof(myHeaderModel.guid); i++)
		printf(" %02x", myHeaderModel.guid[i]);


	printf("\n");
#endif

	//printf("Model Version   :  %ul %s", myHeaderModel.modelVer , "\n\n");
	// end show


	ptmyLowPoll = (struct NumOfPoll*)(val + sizeof(myHeaderModel));
	myLowPoll = *ptmyLowPoll;			//point to the whole struct of NumOfPoll

	ptmyHighPoll = (struct NumOfPoll*)(val + sizeof(myHeaderModel)+ sizeof(myNumOfPoll));
	myHighPoll = *ptmyHighPoll;			//point to the whole struct of NumOfPoll

	ptmyAlarmPoll = (struct NumOfPoll*)(val + sizeof(myHeaderModel) + 2*sizeof(myNumOfPoll));
	myAlarmPoll = *ptmyAlarmPoll;		//point to the whole struct of NumOfPoll


	// show INFO
	for (int d = 0; d < 3; d++)
	{
		struct NumOfPoll *pt;

		if		(d == 0) { pt = &myLowPoll;		printf("LOW POLLING:     \n"); }
		else if (d == 1) { pt = &myHighPoll;    printf("HIGH POLLING:    \n"); }
		else		     { pt = &myAlarmPoll;  printf("ALARM POLLING:  \n"); }

		for (int c = 0; c < 4; c++)
		{
			if (c == 0)		 { printf("Coil   %d %s", pt->numOfCOIL, "\n");}
			else if (c == 1) { printf("Disc   %d %s", pt->numOfDISC, "\n");}
			else if (c == 2) { printf("HR     %d %s", pt->numOfHR,   "\n");}
			else if (c == 3) { printf("IR     %d %s", pt->numOfIR,   "\n\n");}
		}
	}
	// end show
}

uint8_t* BinaryModel_GetChunk(uint32_t sz){

	FILE *input_file_ptr;
	size_t sz_read;

	uint8_t* chunk = (uint8_t *)malloc(sz);
	if (chunk == NULL)
	{
		DEBUG_BINARY_MODEL("NO MEMORY FOR MODEL!!! \n");
		return NULL;
	}

	input_file_ptr = fopen(MODEL_FILE, "rb");
	if (input_file_ptr == NULL)
	{
		printf("Unable to open file! \n");
		return NULL;
	}

	sz_read = fread(chunk, sizeof(uint8_t), sz, input_file_ptr);  // double
//	chunk[sz+1]=0;
	if(sz_read != sz)
		printf("Read ERROR!!!! \n");

	// close streaming
	fclose(input_file_ptr);

	return chunk;
}

uint16_t BinaryModel_GetCrc(void){
	
	// TODO manage sz == 0     CHIEBAO
	uint32_t sz = filesize(MODEL_FILE);
	uint8_t* chunk = BinaryModel_GetChunk(sz);

	// calcolo del crc
	Crc = CRC16(chunk, sz-2);

	free(chunk);

	return Crc;
}

C_RES BinaryModel_CheckCrc(void){

	uint32_t sz = filesize(MODEL_FILE);
	uint8_t* chunk = BinaryModel_GetChunk(sz);

	// calcolo del crc
	Crc = CRC16(chunk, sz-2);
	//Read crc
	uint16_t ModelCrc = ((*(chunk + sz - 2)) & 0x00FF)|
               ((uint16_t)(*(chunk + sz - 1)))<<8;

	free(chunk);

	if (Crc == ModelCrc) 
		return C_SUCCESS;
	else
		return C_FAIL;
}

static uint8_t test = 0;

int BinaryModel_Init (void)
{
	uint8_t* chunk;
	uint32_t sz = 0;
	// check the existance of the files in the FileSystem
	FS_DisplayFiles();

	DEBUG_BINARY_MODEL("Start check GME MODEL\n");
	// Check model size
	sz = filesize(MODEL_FILE);
	if (sz > GME_MODEL_MAX_SIZE) {
		DEBUG_BINARY_MODEL("ERROR: Model too large!\n");
		valid_model = FALSE;
		return C_FAIL;
	}
	chunk = BinaryModel_GetChunk(sz);
	struct HeaderModel* tmpHeaderModel;
	tmpHeaderModel = (struct HeaderModel *)chunk;
	// Check model crc
	uint16_t Crc = CRC16((uint8_t*)chunk, sz - 2);
	uint16_t ModelCrc = ((*(chunk + sz - 2)) & 0x00FF)| ((uint16_t)(*(chunk + sz - 1)))<<8;
	if (Crc != ModelCrc) {
		DEBUG_BINARY_MODEL("ERROR: Wrong CRC Model!\n");
		valid_model = FALSE;
		return C_FAIL;
	}
	// Check model header
	if (memcmp(tmpHeaderModel->signature, GME_MODEL, sizeof(GME_MODEL)) || (tmpHeaderModel->version != HEADER_VERSION)) {
		DEBUG_BINARY_MODEL("ERROR: Wrong signature Model!\n");
		valid_model = FALSE;
		return C_FAIL;
	}

	// utility function
	GetDeviceInfo(chunk);

	// retrieve the useful pointers inside the model
	get_model_pointers(chunk);

	GME__ExtractHeaderInfo(tmpHeaderModel);

	free(chunk);
	valid_model = TRUE;
	return C_SUCCESS;

}

bool CheckModelValidity(void){
	return valid_model;
}


void BinaryModel__GetNum(uint8_t arr[MAX_POLLING][MAX_REG]){

	for (int d = 0; d < MAX_POLLING; d++)
	{
		struct NumOfPoll *pt;

		if		(d == 0) { pt = &myLowPoll;}
		else if (d == 1) { pt = &myHighPoll;}
		else		     { pt = &myAlarmPoll;}

		for (int c = 0; c < MAX_REG; c++)
		{
			if (c == 0)		 { arr[d][c] = pt->numOfCOIL;}
			else if (c == 1) { arr[d][c] = pt->numOfDISC;}
			else if (c == 2) { arr[d][c] = pt->numOfHR;}
			else if (c == 3) { arr[d][c] = pt->numOfIR;}
		}
	}
}






uint8_t* BinaryModel__GetPtrSec(PollType_t polling_type, RegType_t reg_type){

	uint8_t* temp;
	switch (polling_type){
		case LOW_POLLING:
			switch(reg_type){
				case COIL:
					temp = p_coil_low_sect;
					break;
				case DI:
					temp = p_di_low_sect;
					break;
				case HR:
					temp = p_hr_low_sect;
					break;
				case IR:
					temp = p_ir_low_sect;
					break;

				default:
					return NULL;
			}
			break;


		case HIGH_POLLING:
			switch(reg_type){
				case COIL:
					temp = p_coil_high_sect;
					break;
				case DI:
					temp = p_di_high_sect;
					break;
				case HR:
					temp = p_hr_high_sect;
					break;
				case IR:
					temp = p_ir_high_sect;
					break;

				default:
					return NULL;
			}
			break;


		case ALARM_POLLING:
			switch(reg_type){
				case COIL:
					temp = p_coil_alarm_sect;
					break;
				case DI:
					temp = p_di_alarm_sect;
					break;
				case HR:
					temp = p_hr_alarm_sect;
					break;
				case IR:
					temp = p_ir_alarm_sect;
					break;

				default:
					return NULL;
			}
			break;

		default:
			return NULL;

	}
	return temp;
}
