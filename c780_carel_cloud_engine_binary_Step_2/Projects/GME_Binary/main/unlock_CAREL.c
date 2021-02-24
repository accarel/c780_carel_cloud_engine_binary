/**
 * @file    unlock_CAREl.c
 * @author  CAREL
 * @date    feb 2021
 * @brief   functions implementations specific related to the unlock device function
 */

/* Includes ------------------------------------------------------------------------ */

#include "unlock_CAREL.h"
#include "sys_IS.h"
#include "modbus_IS.h"
#include "CBOR_CAREL.h"
#include "ota_CAREL.h"
#include "binary_model.h"
#include "filelog_CAREL.h"

#include "ReportSlaveID_fields.h"



/* prototype */
uint32_t crc32(uint32_t crc, const char *buf, size_t len);
static C_UINT16 Decode_Variable_part(C_UINT16 len, C_BYTE * data);

/* Exported types ------------------------------------------------------------*/
#ifdef INCLUDE_PLATFORM_DEPENDENT
extern  CHAR     ucMBSlaveID[256];
extern  USHORT   usMBSlaveIDLen;
extern  USHORT   param_buffer[2];
#endif


/* --------------------------------------------------------------------------- */
/* Fixed MB address of registry dedicated to unlock some valued added features */
/* plus some useful registry masks values                                      */
/* Refernce document                                                           */
/* https://docs.google.com/document/d/1CrJS0_9Qj3wN0NtKzLcX3lbx9s_iJSVm/edit#  */
/* --------------------------------------------------------------------------- */

/* feature TODAY 20210108 defined */
#define RW_STATUS_LOCKED            0x0000
#define RW_STATUS_UNLOCKED          0x0001
#define RW_STATUS_KEY_WRITTEN       0x8000
#define RW_STATUS_GET_SERVICE       0x4000

/* feature TODAY 20210108 defined */
#define UNLOCK_BIT_STC   (1)
#define UNLOCK_BIT_LOGS  (1<<1)
#define UNLOCK_BIT_ALR   (1<<2)
#define UNLOCK_BIT_UPG   (1<<3)
#define UNLOCK_BIT_BLKRC (1<<4)

//all the feature unlocked
#define UNLOCK_BITS     (UNLOCK_BIT_STC | UNLOCK_BIT_LOGS | UNLOCK_BIT_ALR | UNLOCK_BIT_UPG | UNLOCK_BIT_BLKRC)

/* Modubus ADDRESS start here */
#define MB_ADDR_BASE                  60000

/* 4 byte */
#define MB_ADDR_SRV_VER_MAJ          (MB_ADDR_BASE + 0)
#define MB_ADDR_SRV_VER_MIN          (MB_ADDR_BASE + 1)
/* 18 byte */
#define MB_ADDR_PN_0                 (MB_ADDR_BASE + 2)
#define MB_ADDR_PN_1                 (MB_ADDR_BASE + 3)
#define MB_ADDR_PN_2                 (MB_ADDR_BASE + 4)
#define MB_ADDR_PN_3                 (MB_ADDR_BASE + 5)
#define MB_ADDR_PN_4                 (MB_ADDR_BASE + 6)
#define MB_ADDR_PN_5                 (MB_ADDR_BASE + 7)
#define MB_ADDR_PN_6                 (MB_ADDR_BASE + 8)
#define MB_ADDR_PN_7                 (MB_ADDR_BASE + 9)
#define MB_ADDR_PN_8                 (MB_ADDR_BASE + 10)
/* 4 byte */
#define MB_ADDR_RND_NUM_H            (MB_ADDR_BASE + 11)
#define MB_ADDR_RND_NUM_L            (MB_ADDR_BASE + 12)
/* 4 byte */
#define MB_ADDR_UNLOCK_BITS_H        (MB_ADDR_BASE + 13)
#define MB_ADDR_UNLOCK_BITS_L        (MB_ADDR_BASE + 14)
/* 2 byte */
#define MB_ADDR_RW_STATUS            (MB_ADDR_BASE + 15)
/* 4 byte */
#define MB_ADDR_SESSION_UNLOCK_KEY_H (MB_ADDR_BASE + 16)
#define MB_ADDR_SESSION_UNLOCK_KEY_L (MB_ADDR_BASE + 17)

/*
starting from (MB_ADDR_BASE + 18) there are the FW/SW version section not used byte
the gateway and for this reason left undefined
*/


/* --------------------------------------------------------------------------- */
#define PARTNUMBER_SIZE (18/2) //17 bytes but through register need to be rounded  BILATO TBC
#define OEM_ID_SIZE     16


C_BYTE secret_carel[]=
{0x39,0x57,0x61,0x13,0x72,0xd5,0x4e,0x44,0xa2,0x67,0x8f,0x72,0xce,0x4e,0x4f,0x37};

C_UINT32 oem_unlock_key;
C_UINT32 session_unlock_key;


C_BYTE ctrl_oem_id[OEM_ID_SIZE];


/**
 * @brief read_hd_reg
 * read an Holding register
 *
 * @param  void
 * @return C_RES
 */
static C_RES read_hd_reg(C_UINT16 reg, C_UINT16 * val)
{
    uint8_t retry = 0;

    eMBMasterReqErrCode errorReq = MB_MRE_NO_ERR;

	retry = 0;

	do {
		errorReq = app_holding_register_read(Modbus__GetAddress(), reg, 1);
		retry++;
	} while(errorReq != MB_MRE_NO_ERR && retry < 3);

	if(errorReq != MB_MRE_NO_ERR)
	{
	  P_COV_LN;
	  return C_FAIL;
	}

	*val = param_buffer[0];

	return C_SUCCESS;
}


/**
 * @brief unlock_feature_control
 * Function to unlock the device services
 *
 * @param  void
 * @return C_RES
 */
C_RES unlock_feature_control(void)
{
    C_RES res;

    C_UINT16 tmp;
    C_BYTE   l;
    C_BYTE 	 ctrl_part_number[PARTNUMBER_SIZE*2]; // we use a byte array!!!

	C_BYTE key[4];
	C_BYTE rnd[4];
	C_UINT16 session_h, session_l;
	C_UINT16 unlck_h, unlck_l;
	C_UINT16 rw_status_val;

	C_UINT16 prot_cap;

	C_BYTE data_rx[255];
	C_UINT16 len, retv;

	oem_unlock_key = 0;

	/* cmd 17 */
	if (app_report_slave_id_read(Modbus__GetAddress()) != MB_MRE_NO_ERR)
	{
		P_COV_LN;
		return C_FAIL;
	}

	// save the data
	len = usMBSlaveIDLen;
	data_rx[0] = (C_BYTE)Modbus__GetAddress();
	// get response
	for (C_INT16 i = 0; i < (len - 2); i++)  // crc not used, then we remove 2 bytes
	  data_rx[i + 1] = 	ucMBSlaveID[i];

	//
	// yellow part
	//
	// check if is locked or free
	prot_cap = ((C_UINT16)data_rx[CMD17_PROTOCOLCAP]<< 8) + ((C_UINT16)data_rx[CMD17_PROTOCOLCAP+1]);

	if((prot_cap & UNLOCK_SPECIAL_FEATURES_PRESENT_MASK) != UNLOCK_SPECIAL_FEATURES_PRESENT_MASK)
	{
		P_COV_LN;
		res = C_FAIL;
		return res;
	}

	//
	// manage variable part (blue, violet, orange, and so on...)
	// starting offset 35 (in bytes, after pink part)
	//
	retv = 35;
	while(retv < len)
	{
		retv = Decode_Variable_part(retv, data_rx);
	}

	// read holding registry 60000
	l = 0;
	for(int i = 0; i < PARTNUMBER_SIZE; i++)
	{
		if(read_hd_reg(MB_ADDR_PN_0 + i, &tmp) != C_SUCCESS)
		  return C_FAIL;

		ctrl_part_number[l] = (C_BYTE)(tmp>>8);
		ctrl_part_number[l+1] = (C_BYTE)tmp;

		l+=2;
	}

	/*
	   compute the PASSEPARTOUT unlock code
	   OEM_UNLOCK_KEY = CRC32(PN + OEM_ID + UNLOCK_BITS + SECRECT_CAREL)
	*/
	C_BYTE unlock_bit[4] = { 0xff, 0xff, 0xff, 0xff };

	oem_unlock_key = crc32(0, (const void*)ctrl_part_number, sizeof(ctrl_part_number));
	oem_unlock_key = crc32(oem_unlock_key, (const void*)ctrl_oem_id, sizeof(ctrl_oem_id));
	oem_unlock_key = crc32(oem_unlock_key, (const void*)unlock_bit, sizeof(unlock_bit));
	oem_unlock_key = crc32(oem_unlock_key, (const void*)secret_carel, sizeof(secret_carel));

	/* oem_unlock_key now contain the unlock key
	   split in 2 registry
	*/
	key[3] = (C_BYTE)(oem_unlock_key);
	key[2] = (C_BYTE)(oem_unlock_key>>8);
	key[1] = (C_BYTE)(oem_unlock_key>>16);
	key[0] = (C_BYTE)(oem_unlock_key>>24);

	/* read the RND number from the controller */
	l = 0;
	for(int i = 0; i < 2; i++)
	{
		if(read_hd_reg(MB_ADDR_RND_NUM_H + i, &tmp) != C_SUCCESS)
		{
		  P_COV_LN;
		  return C_FAIL;
		}

		rnd[l] = (C_BYTE)(tmp>>8);
		rnd[l+1] = (C_BYTE)tmp;

		l+=2;
	}

	/* compute the session key */
	/* SESSION_UNLOCK_KEY = CRC32(RND_NUM + OEM_UNLOCK_KEY)*/
	session_unlock_key = 0;
	session_unlock_key = crc32(0,(const void*)rnd, sizeof(rnd));
	session_unlock_key = crc32(session_unlock_key, (const void*)key, sizeof(key));

	session_h = (session_unlock_key & 0xFFFF0000) >> 16;
	session_l = (session_unlock_key & 0x0000FFFF);

	/* write the session key */
	if(app_hr_write(Modbus__GetAddress(), MB_ADDR_SESSION_UNLOCK_KEY_H, 1, &session_h, SINGLE) != C_SUCCESS) { P_COV_LN; return C_FAIL; }
	if(app_hr_write(Modbus__GetAddress(), MB_ADDR_SESSION_UNLOCK_KEY_L, 1, &session_l, SINGLE) != C_SUCCESS) { P_COV_LN; return C_FAIL; }

	/* write the unlock bits mask */
	unlck_h = 0xFFFF;
	unlck_l = 0xFFFF;

	if(app_hr_write(Modbus__GetAddress(), MB_ADDR_UNLOCK_BITS_H, 1, &unlck_h, SINGLE) != C_SUCCESS) { P_COV_LN; return C_FAIL; }
	if(app_hr_write(Modbus__GetAddress(), MB_ADDR_UNLOCK_BITS_L, 1, &unlck_l, SINGLE) != C_SUCCESS) { P_COV_LN; return C_FAIL; }


	/* notify the control that all the unlock data are ready */
	rw_status_val = RW_STATUS_KEY_WRITTEN;
	if(app_hr_write(Modbus__GetAddress(), MB_ADDR_RW_STATUS, 1, &rw_status_val, SINGLE) != C_SUCCESS) { P_COV_LN; return C_FAIL; }

	/* wait a little to be sure that the control processed the data */
	Sys__Delay(250);

	rw_status_val = RW_STATUS_GET_SERVICE;
	if(app_hr_write(Modbus__GetAddress(), MB_ADDR_RW_STATUS, 1, &rw_status_val, SINGLE) != C_SUCCESS) { P_COV_LN; return C_FAIL; }

	Sys__Delay(1000);

	/* check if the device is correctly unlocked */
	if(read_hd_reg(MB_ADDR_RW_STATUS, &rw_status_val) != C_SUCCESS)
	{
	  P_COV_LN;
	  return C_FAIL;
	}

	if ((rw_status_val & RW_STATUS_UNLOCKED) == RW_STATUS_UNLOCKED)
	{
		/* SUCCESS the device is unlocked*/
		res = C_SUCCESS;
		P_COV_LN;
	}
	else
	{
		/* FAIL the device is still locked*/
		res = C_FAIL;
		P_COV_LN;
	}

	return res;
}


/**
 * @brief decode_cpCO_area
 * Function retrieve the OEM ID from cpCO area
 *
 * @param  C_BYTE * val
 *
 * @return void
 */
static void decode_cpCO_area(C_BYTE * val)
{
    C_BOOL is_empty;

	is_empty = true;
	//extract OEM ID and check that is not empty
	for(C_INT16 i = 0; i < OEM_ID_SIZE; i++)
	{
		ctrl_oem_id[i] = val[CMD17_OV_OEM_ID+i];

		if(ctrl_oem_id[i] != 0)
			is_empty = false;
	}

	// if OV_OEM_ID is empty, we use the OEM_ID
	if(is_empty)
	{
		for(C_INT16 i = 0; i < OEM_ID_SIZE; i++)
			  ctrl_oem_id[i] = val[CMD17_OEM_ID+i];
		P_COV_LN;
	}
}




/**
 * @brief Decode_Variable_part
 * Function that dynamically move inside the non-fixed part of CMD-17
 *
 * @param  C_UINT16 len
 * @param  C_BYTE * data
 *
 * @return C_UINT16 retval
 */
static C_UINT16 Decode_Variable_part(C_UINT16 len, C_BYTE * data)
{
	C_UINT16 retval;
	C_BYTE value = data[len+1];

	// calc the next offset
	retval = len + data[len];

	switch(value)
	{
		case 1:
		{
			decode_cpCO_area(data);
			P_COV_LN;
			break;
		}

		case 2:
		{
			// ACU/BCU area
			P_COV_LN;
			break;
		}

		case 3:
		{
			//cpCO area
			P_COV_LN;
			break;
		}

		case 4:
		{
			//pCO area
			P_COV_LN;
			break;
		}

		case 5:
		{	//unknown
			P_COV_LN;
			break;
		}

		case 6:
		{   //unknown
			P_COV_LN;
			break;
		}

		case 7:
		{	//unknown
			P_COV_LN;
			break;
		}

		default:
			P_COV_LN;
			break;

	}
	return retval;
}




// ----------------------------- crc32b --------------------------------
/**
 *  source code ---> https://rosettacode.org/wiki/CRC-32#C
 */
uint32_t crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;

	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}

	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}
