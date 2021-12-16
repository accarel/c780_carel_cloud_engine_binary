/*
 * nvm.c
 *
 *  Created on: Jun 18, 2019
 *      Author: ataayoub
 *
 *      FW Ver 90.00.00
 */

#include "nvm_CAREL.h"
#include "nvm_IS.h"


/**
 * @brief NVM__ReadU8Value
 *        read a data of 8bit value from non volatile memory
 *
 * @param  const C_CHAR* var
 * @param  C_BYTE* val
 *
 * @return C_RES
 */
C_RES NVM__ReadU8Value(const C_CHAR* var, C_BYTE* val)
{
	C_RES err =	NVM__Open();
	if (err == C_SUCCESS) {
		// Read
		PRINTF_DEBUG_NVM("Reading %s from NVS ... ", var);
		err = NVM__GetU8(var, val);
		NVM__Close();
	}
	P_COV_LN;
    return err;
}

/**
 * @brief NVM__ReadU32Value
 *        read a data of 32bit value from non volatile memory
 *
 * @param  const C_CHAR* var
 * @param  C_UINT32* val
 *
 * @return C_RES
 */
C_RES NVM__ReadU32Value(const C_CHAR* var, C_UINT32* val)
{
	C_RES err =	NVM__Open();
	if (err == C_SUCCESS) {
		// Read
		PRINTF_DEBUG_NVM("Reading %s from NVS ... ", var);
		err = NVM__GetU32(var, val);
		NVM__Close();
	}
	P_COV_LN;
	return err;
}

/**
 * @brief NVM__ReadString
 *        read a string value from non volatile memory
 *
 * @param  const C_CHAR* var
 * @param  C_CHAR* str
 * @param  size_t* len
 *
 * @return C_RES
 */
C_RES NVM__ReadString(const C_CHAR* var, C_CHAR* str, size_t* len)
{
	C_RES err =	NVM__Open();
	if (err == C_SUCCESS) {
		// Read
		PRINTF_DEBUG_NVM("Reading %s from NVS ... ",var);
		err = NVM__GetString(var, str, len);
		NVM__Close();
	}
	P_COV_LN;
    return err;
}

/**
 * @brief NVM__WriteU8Value
 *        write a data of 8bit value into non volatile memory
 *
 * @param  const C_CHAR* var
 * @param  C_BYTE val
 *
 * @return C_RES
 */
C_RES NVM__WriteU8Value(const C_CHAR* var, C_BYTE val)
{
	C_RES err =	NVM__Open();
	if (err == C_SUCCESS) {
		err = NVM__SetU8(var, val);
		NVM__Close();
	}
	P_COV_LN;
    return err;
}

/**
 * @brief NVM__WriteU32Value
 *        write a data of 32bit value into non volatile memory
 *
 * @param  const C_CHAR* var
 * @param  C_BYTE val
 *
 * @return C_RES
 */
C_RES NVM__WriteU32Value(const C_CHAR* var, C_UINT32 val)
{
	C_RES err =	NVM__Open();
	if (err == C_SUCCESS) {
		err = NVM__SetU32(var, val);
		NVM__Close();
	}
	P_COV_LN;
	return err;
}

/**
 * @brief NVM__WriteString
 *        write a string value into non volatile memory
 *
 * @param  const C_CHAR* var
 * @param  C_CHAR* str
 *
 * @return C_RES
 */
C_RES NVM__WriteString(const C_CHAR* var, C_CHAR* str)
{
	C_RES err =	NVM__Open();
	if (err == C_SUCCESS) {
		err = NVM__SetString(var, str);
		NVM__Close();
	}
	P_COV_LN;
	return err;
}

/**
 * @brief NVM__EraseKey
 *        Erase the key value in a non volatile memory
 *
 * @param  const C_CHAR* var
 *
 * @return C_RES
 */
C_RES NVM__EraseKey(const C_CHAR* var)
{
	C_RES err = NVM__Open();
	if (err == C_SUCCESS) {
		err = NVM__EraseK(var);
		NVM__Close();
	}
	P_COV_LN;
	return err;
}

/**
 * @brief NVM__EraseAll
 *        Erase All ???
 *
 * @param  none
 *
 * @return C_RES
 */
C_RES NVM__EraseAll(void)
{
	C_RES err = NVM__Open();
	if (err == C_SUCCESS) {
		err = NVM__Erase();
		NVM__Close();
	}
	P_COV_LN;
	return err;
}


/**
 * @brief NVM__ReadBlob
 *        Read a chunk of data from the non volatile memory
 *
 * @param  const C_CHAR* var,
 * @param  void* vec
 * @param  size_t* len
 *
 * @return C_RES
 */
C_RES NVM__ReadBlob(const C_CHAR* var, void* vec, size_t* len)
{
	C_RES err = NVM__Open();
	if (err == C_SUCCESS) {
		err = NVM__GetBlob(var, vec, len);
		NVM__Close();
	}
	P_COV_LN;
    return err;
}


/**
 * @brief NVM__WriteBlob
 *        write a chunk of data to the non volatile memory
 *
 * @param  const C_CHAR* var,
 * @param  void* vec
 * @param  size_t* len
 *
 * @return C_RES
 */
C_RES NVM__WriteBlob (const C_CHAR* var, void* vec, size_t len)
{
	C_RES err = NVM__Open();
    if (err == C_SUCCESS) {
    	err = NVM__SetBlob(var, vec, len);
        NVM__Close();
    }
    P_COV_LN;
    return err;
}
